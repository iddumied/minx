#ifndef __MINX_OPTIONS_H__
#define __MINX_OPTIONS_H__

#define SETBIT(ary,c)   do {                        \
                            *ary &= 0x01<<c;        \
                        } while(0)

enum {
    OPTION_VERBOSE,
};

void        minx_option_init    (void);
void        minx_option_set     (signed int num);
void        minx_option_is_set  (signed int num);
void        minx_option_destroy (void);

#endif // __MINX_OPTIONS_H__
