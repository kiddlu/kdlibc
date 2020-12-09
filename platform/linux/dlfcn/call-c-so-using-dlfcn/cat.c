#include <stdio.h>
#include "cat.h"

static unsigned int cat_weight = 5; //5KG

int cat_speak(const char *name)
{
    printf("Meow! I'm %s\n", name);
}

int cat_get_weight(unsigned int *weight)
{
	*weight = cat_weight;
}

const cat_modapi_t CatMod = 
{
	.module = "cat",
	cat_get_weight,
	cat_speak,
};