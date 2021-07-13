//build with mingw:  i686-w64-mingw32-gcc lsuvc.c -o lsuvc.exe -lsetupapi

#include <windows.h>
#include <stdio.h>
#include <setupapi.h>
#include <stdbool.h>

static void ListupSerialPort(void)
{
	GUID class_guid[1];
	unsigned long required_size;
	bool ret;
	HDEVINFO devinfo_set = NULL;
	SP_DEVINFO_DATA devinfo_data;
	unsigned int member_index = 0;
	int i;

	devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);

	// Get ClassGuid from ClassName for PORTS class
	ret = SetupDiClassGuidsFromName("Camera", (LPGUID)&class_guid, 1,
		                          &required_size);
	if (ret != true) {
		printf("SetupDiClassGuidsFromName Error\n");
		goto cleanup;
	}

	// Get class devices
	devinfo_set = SetupDiGetClassDevs(&class_guid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);

	if (devinfo_set) {
		// Enumerate devices
		member_index = 0;
		while (SetupDiEnumDeviceInfo(devinfo_set, member_index++, &devinfo_data)) {
			char friendly_name[MAX_PATH];
			char hardware_id[MAX_PATH];
			char message[MAX_PATH];
			unsigned long req_size = 0;
			unsigned long prop_type;
			unsigned long type = REG_SZ;
			int port_nb;
			HKEY hKey = NULL;

			// Get friendly_name
			ret = SetupDiGetDeviceRegistryProperty(devinfo_set,
			                                        &devinfo_data,
			                                        SPDRP_FRIENDLYNAME,
			                                        &prop_type,
			                                        friendly_name,
			                                        sizeof(friendly_name),
			                                        &req_size);

            ret = SetupDiGetDeviceRegistryProperty(devinfo_set,
													&devinfo_data,
													SPDRP_HARDWAREID,
													&prop_type,
													(LPBYTE)hardware_id,
													sizeof(hardware_id),
													&req_size);

			printf("%s \t\t %s\n", friendly_name, hardware_id);
		}
	}
cleanup:
// Destroy device info list
	SetupDiDestroyDeviceInfoList(devinfo_set);
}

int main(int argc, char* argv[])
{
	ListupSerialPort();
    return 0;
}