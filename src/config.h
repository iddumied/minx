#ifndef __MINX_CONFIG_H__
#define __MINX_CONFIG_H__

typedef enum {
    CONF_REGISTER_SIZE,
    CONF_ADDRESS_SIZE,

} ConfigurationType;

void *      minx_config_get     (ConfigurationType ct);

#endif // __MINX_CONFIG_H__
