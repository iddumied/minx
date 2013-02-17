#include "kernel/module.h"

static Module*              find_module(uint64_t id);

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
static char                 *memory_helper
static unsigned int         memory_helper_size;

/*
 * init function for this part of the kernel
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
}

/*
 * unload function for removing a module by ID
 */
void minx_kernel_module_unload(uint64_t moduleID) {
}

/*
 * call a module opcode with parameters
 */
void minx_kernel_module_call_opcode(uint64_t moduleID, uint64_t opc, HeapNode *memory) {
}

/*
 * call a module opcode without parameters
 */
void minx_kernel_module_call_opcode_noparam(uint64_t moduleID, uint64_t opc) {
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
void minx_kernel_module_set_config(ConfigurationType type,
                                    ConfigurationValue *vals,
                                    unsigned int len) {
}

/*
 * get the status of the module
 */
uint64_t minx_kernel_module_get_status(uint64_t moduleID) {
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

/*
 * create and return a new Module
 *
 * initialize all fields
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
}
