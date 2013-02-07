#ifndef __MINX_UTIL_MACROS_H__
#define __MINX_UTIL_MACROS_H__

#define         setbit(byte,bitnum)     do { byte |=  0x0001<<bitnum; } while(0)
#define         clrbit(byte,bitnum)     do { byte &= ~(0x0001<<bitnum); } while(0)

#endif // __MINX_UTIL_MACROS_H__
