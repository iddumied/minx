#ifndef __MINX_CONFIG_H__
#define __MINX_CONFIG_H__

#include "map/map.h"

typedef enum {

} ConfigurationType;

void        minx_config_parse   (unsigned int len, char *str);
void *      minx_config_get     (ConfigurationType ct);

#endif // __MINX_CONFIG_H__
