#include <stdio.h>
#include <stdint.h>

#define FAMILY_NAME "huang"
#define GIVEN_NAME  "xiaolu"

#define FNAME_LEN (sizeof(FAMILY_NAME) - 1)
#define GNAME_LEN (sizeof(GIVEN_NAME) - 1)

struct __attribute__((packed)) person_attr
{
   char family_name[FNAME_LEN];
   char given_id[GNAME_LEN];
   uint64_t id;
   char passwd[64];
};


int main()
{
    struct person_attr attr;
    printf("sizeof struct %ld\n", sizeof(struct person_attr));
}
