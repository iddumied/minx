#ifndef __MINX_CONFIG_H__
#define __MINX_CONFIG_H__

#include <stdlib.h>
#include <string.h>

#include "util/error.h"
#include "util/debug.h"
#include "util/config_types.h"

void                    minx_config_init    (void);
void                    minx_config_shutdown(void);
void                    minx_config_parse   (unsigned int argc, char ** argv);
ConfigurationValue*     minx_config_get     (ConfigurationType ct);
int                     minx_config_is_set  (ConfigurationType ct);

#endif // __MINX_CONFIG_H__
