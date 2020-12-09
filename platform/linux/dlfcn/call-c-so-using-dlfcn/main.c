#include <stdio.h>
#include <dlfcn.h>
#include "cat.h"

int main()
{
	cat_modapi_t *pCatMod = NULL;
	void *handle = NULL;
	unsigned int cat_weight = 0;
	
	handle = dlopen("./libcat.so", RTLD_LAZY);
	if (!handle) {
		printf("%s,%d, NULL == handle\n", __FUNCTION__, __LINE__);
		return -1;
	}
	dlerror();

	pCatMod = (cat_modapi_t *)dlsym(handle, "CatMod");
	if (!pCatMod) {
		printf("%s,%d, NULL == pCatMod\n", __FUNCTION__, __LINE__);
		return -1;
	}
	if (pCatMod && pCatMod->speak)
		pCatMod->speak("Tom");
	if (pCatMod && pCatMod->get_weight) {
		pCatMod->get_weight(&cat_weight);
		printf("cat weight is %d\n", cat_weight);
	}

	if (pCatMod && pCatMod->module) {
		printf("module = %s\n", pCatMod->module);
	}

    dlclose(handle);
    return 0;
}
