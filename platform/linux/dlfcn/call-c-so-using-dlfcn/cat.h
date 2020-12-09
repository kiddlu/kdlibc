typedef struct {
 const char *module;
 int  (*get_weight)(unsigned int *weight);
 int  (*speak)(const char *name);
} cat_modapi_t;