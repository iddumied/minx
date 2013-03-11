#ifndef __MINX_UTIL_MACROS_H__
#define __MINX_UTIL_MACROS_H__

#define         setbit(byte,bitnum)     do { byte |=  0x0001<<bitnum; } while(0)
#define         clrbit(byte,bitnum)     do { byte &= ~(0x0001<<bitnum); } while(0)

#define         printminxasciiart()     do {    \
        printf(                                 \
"             _            \n"                  \
"            (_)           \n"                  \
" _ __ ___  _ _ __ __  __\n"                    \
"| '_ ` _ \\| | '_ \\\\ \\/ /\n"                \
"| | | | | | | | | |>  < \n"                    \
"|_| |_| |_|_|_| |_/_/\\_\\\n\n"                \
             );                                 \
    } while(0)

#endif // __MINX_UTIL_MACROS_H__
