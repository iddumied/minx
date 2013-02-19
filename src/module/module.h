#ifndef __MINX_MODULE_H__
#define __MINX_MODULE_H__

#include "util/config_types.h"
#include "module/module_opcodes.h"

/*
 * -----------------------------------------------------------------------------
 *
 *              This is the Header, a module has to implement.
 *
 * -----------------------------------------------------------------------------
 *
 * What modules CAN'T do (or not supposed to be able to):
 * -----------------------------------------------------
 *  - modify VPU internals
 *  - modify program pointer 
 *  - modify registers 
 *  - modify memory (except the passed one)
 *  - modify arguments of the vpu
 *  - modify program arguments (not implemented at all)
 *  - resize the passed memory
 *  - free the passed memory (it is a copy)
 *
 * What modules CAN do:
 * --------------------
 *  - modify the passed memory (but not its size, just the value)
 *  - read program arguments (not implemented at all)
 *
 *  A module can implement several opcodes. Each opcode can get params (A piece
 *  of memory) or not. Nothing more, nothing less.
 *
 */

/*
 * gets called if the module should load itself.
 */
void        minx_module_load            (void);

/*
 * gets called if the module should shutdown itself
 */
void        minx_module_unload          (void);

/*
 * gets called if the module should shutdown as fast as possible. Now. No delay!
 * Just unload the main things, don't care about memory and so on!
 *
 * (used for later multithreading maybe, to stop threads and so on)
 */
void        minx_module_force_unload    (void);

/*
 * returns the opcodes for the module 
 */
MOpcodes*    minx_module_get_opcodes     (void);

/*
 * return a non-zero value if the passed opcode inside the module gets param,
 * else zero.
 */
int         minx_module_opcode_gets_params(uint64_t opcode);

/*
 * call the module
 *
 * @param opcode call this specific module-opcode 
 * @param memory the passed memory from the program 
 * @param mem_size the number of bytes in memory
 */
uint64_t    minx_module_call            (   uint64_t opcode, 
                                            char *memory, 
                                            uint64_t mem_size);

/*
 * call the module
 *
 * @param opcode call this specific module-opcode
 *
 * no params here.
 */
uint64_t    minx_module_call_no_params  (uint64_t opcode);

/*
 * passes the configuration from the minx VPU to the module
 *
 * @param type the config the value stands for
 * @param *val a pointer to the array of values. Which union-entry to chose, the
 * module has to know on its own. But that's easy, its constant!
 * @param length number of configs
 */
void        minx_module_set_configs     (   ConfigurationType *types,
                                            ConfigurationValue *val,
                                            unsigned int length);

/*
 * same as return value of to call() functions. Can be called from a opcode
 * inside the running program.
 *
 * Valid statuses are all values above zero. Zero is returned automatically if
 * the module is not loaded / does not exist.
 *
 * If the module returns zero, minx will abort!
 */
uint64_t    minx_module_get_status      (void);

#endif //__MINX_MODULE_H__
