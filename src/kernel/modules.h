#ifndef __MINX_KERNEL_MODULES_H__
#define __MINX_KERNEL_MODULES_H__

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
    int         (*opcode_gets_params_func)  (void);

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
    void        (*set_configs_func)         (ConfigurationType type,
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
    ModuleOpcode opcodes[];

} Module;

#define MEMORY_HELPER_INIT_SIZE 100 

void        minx_kernel_module_init             (void);
void        minx_kernel_module_shutdown         (void);

uint64_t    minx_kernel_module_load             (char *module_path);
void        minx_kernel_module_unload           (uint64_t moduleID);
void        minx_kernel_module_call             (uint64_t moduleID, 
                                                uint64_t opc, 
                                                HeapNode *memory);
void        minx_kernel_module_call_noparam     (uint64_t moduleID, 
                                                uint64_t opc);
int         minx_kernel_module_gets_params      (uint64_t moduleID);
void        minx_kernel_module_set_config       (ConfigurationType type,
                                                ConfigurationValue *vals,
                                                unsigned int len);
uint64_t    minx_kernel_module_get_status       (uint64_t moduleID);

#endif //__MINX_KERNEL_MODULES_H__
