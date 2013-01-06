#ifdef DEBUG

#ifndef __MINX_DEBUG_H__
#define __MINX_DEBUG_H__

#define EXPLAIN_FUNCTION()  do{                                     \
                                printf("minx vm func: %s, line %s\n"\
                                        , __func__, __LINE__);      \
                            } while(0)

#define EXPLAIN_COMMAND()   do {                                    \
                                printf("minx vm cmd: %s, line %s\n" \
                                        , __func__, __LINE__);      \
                            } while(0)

#define EXPLAIN_COMMAND_VERBOSE(cmd,string)                                 \
                                    do{                                     \
                                        printf("minx command: %s\n\t%s\n",  \
                                                cmd, string );              \
                                    } while( 0 );
#endif //__MINX_DEBUG_H__

#endif //DEBUG
