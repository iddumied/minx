#include "kernel/modules.h"

/*
 * This file contains all the logic for the plugins (called "modules").
 * Currently, modules are loaded into address space of the minx VPU, but later,
 * the architecture can change to something more secure. The plugins should NOT
 * be able to call functions from within the VPU!
 *
 * When a module gets loaded, there is a Module (type) allocated and all stuff,
 * the kernel has to know is stored in it. After a module gets free'd, the
 * Module (type) is not free'd, because for later usage.
 *
 */

/*
 * static function prototypes
 */
static Module*              find_module(uint64_t id);
static ModuleOpcode*        find_module_opcode(Module *m, uint64_t opc);
static uint64_t             get_next_module_id(void);
static Module*              new_module(char *name);
static void                 save_memory(HeapNode *node);
static int                  add_module_to_list(Module *mod);
static void                 remove_module_from_list(Module *mod);

/*
 * static variables
 */
static Module               **modules;

/*
 * size of 'modules' in Module*
 */
static unsigned int         modules_size;

/*
 * How many Module* are set in 'modules'
 */
static unsigned int         module_count;

/*
 * Memory helper for passing arguments to modules
 */
static char                 *memory_helper;
static uint64_t             memory_helper_size;

/* 
 * Size of the data in the memory passed to a module.
 * The real size is more, but should be reinitialized to 0x00. Each module gets
 * more memory passed as program tells, but the overhead is reinitialized to
 * 0x00, so it does not care. Tell the module the size it needs to know.
 */
static uint64_t             memory_data_size;

/*
 *
 */
static uint64_t             module_id_counter;

/*
 * -----------------------------------------------------------------------------
 *
 *                          Function implementations
 *
 * -----------------------------------------------------------------------------
 */

/*
 * init function for this part of the kernel
 *
 * If the 'fast' flag is set, the kernel preallocates some Module (type) space,
 * for faster loading of modules. So the kernel does not need to call realloc()
 * on each module-loading. It is a very simple strategy, but may be effective.
 * The KERNEL_MODULE_PREALLOC constant is set and explained in the header file
 * kernel/modules.h
 */
void minx_kernel_module_init(void) {

    if(minx_config_get(CONF_FAST)->b) {
        modules_size = KERNEL_MODULE_PREALLOC;
    }
    else {
        modules_size = 1;
    }

    module_count        = 0;
    modules             = malloc(sizeof(Module) * modules_size);

    if(minx_config_get(CONF_SAVE)->b) {
        memory_helper_size  = MEMORY_HELPER_INIT_SIZE;
        memory_helper       = (char*) malloc(memory_helper_size);
    }

    module_id_counter = 0;
}

/*
 * shutdown function for this part of the kernel
 *
 * - shutdown all modules (force)
 * - free modules 
 * - free modules-ptr-array
 *
 */
void minx_kernel_module_shutdown(void) {
    uint64_t i;
    for(i = 0; i < modules_size; i++) {
        /*
         * call the force-unload function here, on both minx shutdown and abort.
         */ 
        modules[i]->funload_func();
        free(modules[i]);
    }

    free(modules);
}

/*
 * load function for loading a module
 */
uint64_t minx_kernel_module_load(char *module_path) {
    Module *mod = new_module(module_path);
    uint64_t ret = 1;

    mod->handle = dlopen(module_path, RTLD_LAZY);

    if(!mod->handle) {
        F_RUNTIME_ERROR(MINX_KERNEL_MODULE_PREFIX, 
                        "%s: %s", "cannot load library", module_path);
        free(mod);
        ret = 0;
    }

    ret = add_module_to_list(mod);

    /*
     * if no error occoured lookup all module functions and set in Module* type!
     */
    if (ret == 0) { 
#define SET_MOD_FUNC(field,funcname) mod->field = dlsym(mod->handle, funcname);
        SET_MOD_FUNC(load_func, MINX_MODULE_FUNCTION_LOAD);
        SET_MOD_FUNC(unload_func, MINX_MODULE_FUNCTION_UNLOAD);
        SET_MOD_FUNC(funload_func, MINX_MODULE_FUNCTION_FUNLOAD);
        SET_MOD_FUNC(get_opcodes_func, MINX_MODULE_FUNCTION_GET_OPCODES);
        SET_MOD_FUNC(opcode_gets_params_func, MINX_MODULE_FUNCTION_OPC_GETS_PARAMS);
        SET_MOD_FUNC(call_func, MINX_MODULE_FUNCTION_CALL);
        SET_MOD_FUNC(call_no_params_func, MINX_MODULE_FUNCTION_CALL_NO_PARAMS);
        SET_MOD_FUNC(set_configs_func, MINX_MODULE_FUNCTION_SET_CONFIGS);
        SET_MOD_FUNC(get_status_func, MINX_MODULE_FUNCTION_GET_STATUS);
#undef SET_MOD_FUNC

        mod->opcodes = mod->get_opcodes_func();
    }

    return ret;
}

/*
 * unload function for removing a module by ID
 */
void minx_kernel_module_unload(uint64_t moduleID) {
    Module *mod = find_module(moduleID);
    dlclose(mod->handle);
    remove_module_from_list(mod);
}

/*
 * call a module opcode with parameters
 */
void minx_kernel_module_call_opcode(uint64_t moduleID, uint64_t opc, HeapNode *memory) {
    save_memory(memory);
    find_module(moduleID)->call_func(opc, memory_helper, memory_helper_size, memory_data_size);
}

/*
 * call a module opcode without parameters
 */
void minx_kernel_module_call_opcode_noparam(uint64_t moduleID, uint64_t opc) {
    find_module(moduleID)->call_no_params_func(opc);
}

/*
 * read from module if an opcode gets parameters
 */
int minx_kernel_module_opcode_gets_params(uint64_t moduleID, uint64_t op) {
    Module *mod         = find_module(moduleID);
    ModuleOpcode mop    = find_module_opcode(mod, op);

    if(!mop)
        mop->gets_params = mod->opcode_gets_params_func(op);

    return mop->gets_params;
}

/*
 * pass the config to the module
 */
void minx_kernel_module_set_config( uint64_t moduleID,
                                    ConfigurationType *types,
                                    ConfigurationValue *vals,
                                    unsigned int len) {

    find_module(moduleID)->set_configs_func(types, vals, len);
}

/*
 * get the status of the module
 */
uint64_t minx_kernel_module_get_status(uint64_t moduleID) {
    return find_module(moduleID)->get_status_func();
}

/*
 * -----------------------------------------------------------------------------
 *                      static function implementations
 * -----------------------------------------------------------------------------
 */

/*
 * currently, modules are not ordered. Later on, they will be and this function
 * will do binary search!
 */
static Module* find_module(uint64_t id) {
    uint64_t i;
    Module *module = NULL;
    for(i = 0; i < modules_size && module == NULL; i++) {
        if(modules[i]->module_id == id) {
            module = modules[i];
        }
    }

#ifdef DEBUGGING 
    if(minx_config_get(CONF_SRC_DEBUGGING)->b) {
        if(module) {
            MINX_SRC_DEBUG(MINX_KERNEL_MODULE_PREFIX, 
                    "found %s, id %"PRIu64, 
                    module->module_name,
                    module->module_id);
        }
        else {
            MINX_SRC_DEBUG(MINX_KERNEL_MODULE_PREFIX,
                    "not found module %"PRIu64, id);
        }
    }
#endif 

    return module;
}

static ModuleOpcode* find_module_opcode(Module *m, uint64_t opc) {
    unsigned int i;
    ModuleOpcode *res = NULL;

    for(i = 0; i < mod->opcodes_count && res == NULL; i++) {
        if( mod->opcodes[i].opcode == opc )
            res = &(mod->opcodes[i]);
    }

    return res;
}

/*
 * get next module ID. currently just returns and increments a value, later it
 * could be randomized
 */
static uint64_t get_next_module_id(void) {
    return ++module_id_counter;
}

/*
 * create and return a new Module
 *
 * initialize all fields with NULL.
 */
static Module* new_module(char *name) {
    Module *mod = malloc(sizeof(Module));

    if(mod) {
        mod->module_id                  = get_next_module_id();
        mod->module_name                = name;
        mod->load_func                  = NULL;
        mod->unload_func                = NULL;
        mod->funload_func               = NULL;
        mod->get_opcodes_func           = NULL;
        mod->opcode_gets_params_func    = NULL;
        mod->call_func                  = NULL;
        mod->call_no_params_func        = NULL;
        mod->set_configs_func           = NULL;
        mod->get_status_func            = NULL;
        mod->opcodes_count              = NULL;
        mod->opcodes                    = NULL;
    }

    return mod;
}

/*
 * write memory in node to the memoryhelper
 * resize the memory_helper if required. Do only make it greater, not smaller!
 */
static void save_memory(HeapNode *node) {
    if(node->size < memory_helper_size) {
        memory_helper_size = node->size;
        memory_helper = realloc(memory_helper, memory_helper_size);

        if(memory_helper == NULL) {
            FATAL_F_ERROR("Could not reallocate memory helper for passing memory to module");
        }
    }

    memset(memory_helper, 0x00, node->size);
    memcpy(memory_helper, node->memory, node->size);
    memory_data_size = node->size;
}

static int add_module_to_list(Module *mod) {

    int ret = 0; 

    /*
     * Try to find NULL in 'modules'
     */
    uint64_t i;
    for(i = 0; i < modules_size && modules[i] != NULL; i++);
    if(i != modules_size) {
        modules[i] = mod;
    }
    else { /* none found, reallocate! */
        Modules **save = modules;
        modules = realloc(modules, sizeof(Module*), modules_size+1);
        
        if( modules == NULL ) {
            modules = save;
            ret = 1;
        }
        else {
            modules[module_size] = mod;
            modules_size++;
        }
    }

    return ret;
}

static void remove_module_from_list(Module *mod) {
    free(mod->opcodes);
    free(mod);

    /*
     * set the pointer in the 'modules' pointer array to NULL for later reuse
     */
    uint64_t i;
    for(i = 0; i < modules_size; i++) {
        if(modules[i] == mod) {
            modules[i] = NULL;
            i = modules_size;
        }
    }
}
