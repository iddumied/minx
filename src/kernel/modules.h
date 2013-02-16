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
     */
    void        (*unload_func)              (void);

    /*
     * force_unload function for the module
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

#endif //__MINX_KERNEL_MODULES_H__
