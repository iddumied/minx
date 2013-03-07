#ifndef __MINX_KERNEL_MODULES_H__
#define __MINX_KERNEL_MODULES_H__

#include "kernel/heap.h"
#include "module/module_opcodes.h"
#include "util/config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <dlfcn.h>

/*
 * Modules header for the minx kernel
 */

/*
 * ModuleOpcode type, each opcode has a bool, if it gets params
 */
typedef struct {
    uint64_t    opcode;
    int         gets_params;
} ModuleOpcode;

/*
 * Module
 * ------
 *
 *  All data for one module is located in this.
 */
typedef struct {

    /*
     * Identifier for the module, will be passed to the program.
     */
    uint64_t    module_id;

    /*
     * Module name, just internal, read from the load()-call!
     */
    char        *module_name;

    /*
     * void* from the load() call
     */
    void        *handle;

    /*
     * load function for the module
     */
    void        (*load_func)                (void);

    /*
     * unload function for the module
     *
     * gets called from the program, if the module should unload itself.
     */
    void        (*unload_func)              (void);

    /*
     * force_unload function for the module
     *
     * gets called if the minx VPU gets something like SIGINT, and gets called
     * if the minx is going to shutdown itself.
     */
    void        (*funload_func)             (void);

    /*
     * get-opcodes function for the module
     */
    MOpcodes*   (*get_opcodes_func)         (void);

    /*
     * opcode-gets-params function for the module
     */
    int         (*opcode_gets_params_func)  (uint64_t opcode);

    /*
     * call function for the module (with params)
     */
    uint64_t    (*call_func)                (uint64_t opcode, 
                                            char *memory, 
                                            uint64_t mem_size);

    /*
     * call function for the module (without params)
     */
    uint64_t    (*call_no_params_func)      (uint64_t opcode);

    /*
     * set config function for the module
     */
    void        (*set_configs_func)         (ConfigurationType *type,
                                            ConfigurationValue *val,
                                            unsigned int len);

    /*
     * get status function for the module
     */
    uint64_t    (*get_status_func)          (void);

    /*
     * number of opcodes
     */
    unsigned int opcodes_count;

    /*
     * opcodes
     */
    ModuleOpcode *opcodes;

} Module;

/*
 * Prealloc constant for the kernel to preallocate Module (type). 
 *
 * This may change in future. It does not affect the behaviour of the VPU, just
 * it's memory footprint. 
 */
#define KERNEL_MODULE_PREALLOC  25

/*
 * The kernel uses a memory helper to pass memory to the modules (parameters).
 * This memory-helper grows to fit the memory to pass, but never shrinks. 
 *
 * Because of speed, it can be initialized with more or less memory. If the
 * 'fast' flag is set, it starts with the size set in this constant.
 *
 * This value may change in future. I set it to this value, because I think it's
 * a good start.
 */
#define MEMORY_HELPER_INIT_SIZE 100 

void        minx_kernel_module_init                 (void);
void        minx_kernel_module_shutdown             (void);

uint64_t    minx_kernel_module_load                 (char *module_path);
void        minx_kernel_module_unload               (uint64_t moduleID);
void        minx_kernel_module_call_opcode          (uint64_t moduleID, 
                                                    uint64_t opc, 
                                                    HeapNode *memory);
void        minx_kernel_module_call_opcode_noparam  (uint64_t moduleID, 
                                                    uint64_t opc);
int         minx_kernel_module_opcode_gets_params   (uint64_t moduleID, uint64_t op);
void        minx_kernel_module_set_config           (uint64_t moduleID,
                                                    ConfigurationType *types,
                                                    ConfigurationValue *vals,
                                                    unsigned int len);
uint64_t    minx_kernel_module_get_status           (uint64_t moduleID);

/*
 * extern function names
 */
#define MINX_MODULE_FUNCTION_LOAD               "minx_module_load"
#define MINX_MODULE_FUNCTION_UNLOAD             "minx_module_unload"
#define MINX_MODULE_FUNCTION_FUNLOAD            "minx_module_force_unload"
#define MINX_MODULE_FUNCTION_GET_OPCODES        "minx_module_get_opcodes"
#define MINX_MODULE_FUNCTION_OPC_GETS_PARAMS    "minx_module_opcode_gets_params"
#define MINX_MODULE_FUNCTION_CALL               "minx_module_call"
#define MINX_MODULE_FUNCTION_CALL_NO_PARAMS     "minx_module_call_no_params"
#define MINX_MODULE_FUNCTION_SET_CONFIGS        "minx_module_set_configs"
#define MINX_MODULE_FUNCTION_GET_STATUS         "minx_module_get_status"

#endif //__MINX_KERNEL_MODULES_H__
