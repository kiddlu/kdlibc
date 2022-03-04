#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <pthread.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <getopt.h> 

#define ETH_DEVCTRL_PROTOCOL_H2D        (0x7711)
#define ETH_DEVCTRL_PROTOCOL_D2H        (0x7712)

#define NAME_OF_INTERFACE            "lo"

char network_dev_name[16] = NAME_OF_INTERFACE;

bool global_debugPrint_flag =true;

pthread_t tid_recv =-1;

int r_sockfd =-1;
int s_sockfd =-1;

unsigned char ifaddr[6]={0};
unsigned char bcaddr[6]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

typedef struct ETH_HEADER
{
	uint8_t   dest_mac[6];
	uint8_t   src_mac[6];
	uint16_t  etype;
}__attribute__((packed))ETH_HEADER;

enum eth_cmd_id{
	ETH_CMD_ID_NULL = 0,
	ETH_CMD_ID_SEARCH_DEV,
	ETH_CMD_ID_GET_DEV_NETCFG,
	ETH_CMD_ID_SET_DEV_NETCFG,
	ETH_CMD_ID_SET_DEV_REBOOT,
};

typedef struct CMD_HEADER
{
    uint8_t   cmd_id;
    uint16_t  cmd_payload_len;
}__attribute__((packed))CMD_HEADER;

#define print(format,...)  if(global_debugPrint_flag){printf("%s--%s--%d: ",__FILE__,__FUNCTION__,__LINE__), printf(format,##__VA_ARGS__), printf("\n");}


void dumphex(void *data, uint32_t size)
{
#if 1
#define dbg_printf printf
#else 
#define dbg_printf {;}
#endif
	char ascii[17];
	unsigned int i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		if (i % 16 == 0) {
		    //dbg_printf("%p: ", data + i);
            dbg_printf("%08x: ", i);
		}

		dbg_printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			dbg_printf(" ");
			if ((i+1) % 16 == 0) {
				dbg_printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					dbg_printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					dbg_printf("   ");
				}
				dbg_printf("|  %s \n", ascii);
			}
		}
	}
}

static int get_interface_mac(const char *interface_name, unsigned char* mac_buf, int buf_size)
{
	struct ifreq interface;
	int fd=socket(AF_INET,SOCK_DGRAM,0);
	strncpy(interface.ifr_name, interface_name, sizeof(interface.ifr_name));
	if (ioctl(fd, SIOCGIFHWADDR, &interface) < 0) 
	{
		printf("[%s-%d]:ioctl(SIOCGIFifaddr) failed!", __FUNCTION__, __LINE__);
		return -1;
	}
	close(fd);
	memcpy(mac_buf, interface.ifr_hwaddr.sa_data, buf_size);
	return 0;
}

int create_raw_socket(char const *ifname, unsigned short type,unsigned char *ifaddr, int *ifindex)
{
	int optval=1;
	int fd;
	struct ifreq ifr;
	int domain, stype;
	struct sockaddr_ll sa;

	memset(&sa, 0, sizeof(sa));

	domain = PF_PACKET;
	stype = SOCK_RAW;

	if ((fd = socket(domain, stype, htons(type))) < 0) {
		print("Create socket failed!");
		return -1;
	}

	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0) {
		print("setsockopt SO_BROADCAST failed!");
	}
	
	if (ifaddr) {
		strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
		if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) 
		{
			print("ioctl(SIOCGIFifaddr) failed!");
			return -2;
		}

		memcpy(ifaddr, ifr.ifr_hwaddr.sa_data,6);
	}

	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	if (ioctl(fd, SIOCGIFMTU, &ifr) < 0) {
		print("ioctl(SIOCGIFMTU) failed!");
		return -2;
	}

	if (ifr.ifr_mtu < ETH_DATA_LEN) {
		char buffer[256];
		print(buffer, "Interface %.16s has MTU of %d -- should be %d.  You may have serious connection problems.",
			ifname, ifr.ifr_mtu, ETH_DATA_LEN);
	}

	sa.sll_family = AF_PACKET;
	sa.sll_protocol = htons(type);
	strncpy(ifr.ifr_name,ifname,sizeof(ifr.ifr_name));

	if (ioctl(fd,SIOCGIFINDEX,&ifr)<0)
	{
		print("ioctl(SIOCFIGINDEX):Could not get interface index");
		return -2;
	}

	sa.sll_ifindex = ifr.ifr_ifindex;
	
    if (ifindex) {
	    *ifindex = ifr.ifr_ifindex;
	}

	if (bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		print("bind socket failed!");
		return -3;
	}

	return fd;
}

static int eth_xfer(char *dstmac, uint8_t cmd_id, char *buf, uint16_t payload_len)
{

	int ret = 0;
	uint8_t sbuf[1024]={0};
	memset(sbuf, 0, sizeof(sbuf));

	ETH_HEADER eth_header={0};
	memcpy(eth_header.dest_mac,  dstmac,   sizeof(eth_header.dest_mac));
	memcpy(eth_header.src_mac,   ifaddr,    sizeof(eth_header.src_mac));
	eth_header.etype = htons(ETH_DEVCTRL_PROTOCOL_D2H);
	memcpy(sbuf, &eth_header, sizeof(eth_header));

	CMD_HEADER cmd_header={0};
	cmd_header.cmd_id = cmd_id;
	cmd_header.cmd_payload_len = htons(payload_len);
	memcpy(sbuf + sizeof(ETH_HEADER), &cmd_header, sizeof(CMD_HEADER));

	if(payload_len != 0 && buf != NULL) {
	    memcpy(sbuf + sizeof(ETH_HEADER) + sizeof(CMD_HEADER), buf, payload_len);
	}

	ret = send(s_sockfd, sbuf, sizeof(ETH_HEADER) + sizeof(CMD_HEADER) + payload_len, 0);

	return ret;
}

static int resp_search_dev(char *hostmac)
{
	return eth_xfer(hostmac, ETH_CMD_ID_SEARCH_DEV, 
	"Seach Dev OK", strlen("Seach Dev OK"));
}

static int resp_get_netcfg_cmd(char *hostmac)
{
	char netcfg[256] = {0xdd};
	return eth_xfer(hostmac, ETH_CMD_ID_GET_DEV_NETCFG, netcfg, 256);
}

static int resp_set_netcfg_cmd(char *hostmac)
{
	return eth_xfer(hostmac, ETH_CMD_ID_SET_DEV_NETCFG, 
	"Set Netcfg OK", strlen("Set Netcfg OK"));
}

static int resp_reboot_cmd(char *hostmac)
{
	return eth_xfer(hostmac, ETH_CMD_ID_SET_DEV_REBOOT, 
	"Now Reboot Dev", strlen("Now Reboot Dev"));
}

int main(int argc, char *argv[])
{
	if(0 != get_interface_mac(network_dev_name, ifaddr,sizeof(ifaddr)))
	{
		printf("get_interface_mac failed!\n");
	}

	s_sockfd = create_raw_socket(network_dev_name, ETH_DEVCTRL_PROTOCOL_D2H, NULL, NULL);
	if(s_sockfd < 0)
	{
		print("create_raw_socket s_sockfd failed!ret=%d", s_sockfd);
	}

	r_sockfd = create_raw_socket(network_dev_name, ETH_DEVCTRL_PROTOCOL_H2D, NULL, NULL);
	if(r_sockfd < 0)
	{
		print("create_raw_socket r_sockfd failed!ret=%d", r_sockfd);
	}

	int ret=0;
	int data_len;
	unsigned char rbuf[1024];
	memset(rbuf, 0, sizeof(rbuf));

	while(1)
	{
		print("%s","start recv...");

		data_len = recv(r_sockfd, rbuf, 1024,0);

		if(data_len >= sizeof(ETH_HEADER) + sizeof(CMD_HEADER))
		{
			ETH_HEADER *pHeader = (ETH_HEADER*)rbuf;
            CMD_HEADER *pCmd    = (CMD_HEADER *)(rbuf + sizeof(ETH_HEADER));

			if(pHeader->etype == ntohs(ETH_DEVCTRL_PROTOCOL_H2D))
			{
				print("%s","receive packet:");
				print("dest_mac:%02X:%02X:%02X:%02X:%02X:%02X", 
					pHeader->dest_mac[0], pHeader->dest_mac[1], pHeader->dest_mac[2],
					pHeader->dest_mac[3], pHeader->dest_mac[4], pHeader->dest_mac[5]);
				print("src_mac:%02X:%02X:%02X:%02X:%02X:%02X", 
					pHeader->src_mac[0], pHeader->src_mac[1], pHeader->src_mac[2],
					pHeader->src_mac[3], pHeader->src_mac[4], pHeader->src_mac[5]);
				print("etype=%X", ntohs(pHeader->etype));
			}

			if(pHeader->etype == ntohs(ETH_DEVCTRL_PROTOCOL_H2D))
			{
				if(memcmp(pHeader->dest_mac, ifaddr, 6) == 0)
				{
    				switch(pCmd->cmd_id)
    				{
    					case ETH_CMD_ID_GET_DEV_NETCFG:
    						print("%s", "receive ETH_CMD_ID_GET_DEV_NETCFG");
							resp_get_netcfg_cmd(pHeader->src_mac);
    						break;
    					case ETH_CMD_ID_SET_DEV_NETCFG:
    						print("%s", "receive ETH_CMD_ID_SET_DEV_NETCFG");
							resp_set_netcfg_cmd(pHeader->src_mac);
    						break;
    					case ETH_CMD_ID_SET_DEV_REBOOT:
    						print("%s", "receive ETH_CMD_ID_SET_DEV_REBOOT");
							resp_reboot_cmd(pHeader->src_mac);
    						break;
    					default:
    						print("receive unkown cmd: %u", pCmd->cmd_id);
    						break;
    				}
				} else if (memcmp(pHeader->dest_mac, bcaddr, 6) == 0) {
    				switch(pCmd->cmd_id)
    				{
    					case ETH_CMD_ID_SEARCH_DEV:
    						print("%s", "receive ETH_CMD_ID_SEARCH_DEV");
							resp_search_dev(pHeader->src_mac);
    						break;
#if 1//for lo debug
    					case ETH_CMD_ID_GET_DEV_NETCFG:
    						print("%s", "receive ETH_CMD_ID_GET_DEV_NETCFG");
							resp_get_netcfg_cmd(pHeader->src_mac);
    						break;
    					case ETH_CMD_ID_SET_DEV_NETCFG:
    						print("%s", "receive ETH_CMD_ID_SET_DEV_NETCFG");
							resp_set_netcfg_cmd(pHeader->src_mac);
    						break;
    					case ETH_CMD_ID_SET_DEV_REBOOT:
    						print("%s", "receive ETH_CMD_ID_SET_DEV_REBOOT");
							resp_reboot_cmd(pHeader->src_mac);
    						break;
#endif
    					default:
    						print("receive unkown cmd: %u", pCmd->cmd_id);
    						break;
    				}
				} else {
					//not our mac addr, do nothing
					;
				}
                print("\n");
			} 
		}
	}
}
