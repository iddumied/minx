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


void minx_kernel_module_init(void) {

    if(minx_config_get(CONF_FAST)->b) {
        modules_size = KERNEL_MODULE_PREALLOC;
    }
    else {
        modules_size = 1;
    }

    module_count = 0;
    modules         = malloc(sizeof(Module) * modules_size);
}

void minx_kernel_module_shutdown(void) {
    uint64_t i;
    for(i = 0; i < modules_size; i++) {
        /*
         * call the force-unload function here, on both minx shutdown and abort.
         */ 
        modules[i]->funload_func()
    }

    free(modules);
}

/*
 * 
 *
 *
 */

uint64_t minx_kernel_module_load(char *module_path) {
}

void minx_kernel_module_unload(uint64_t moduleID) {
}

void minx_kernel_module_call(uint64_t moduleID, uint64_t opc, HeapNode *memory) {
}

void minx_kernel_module_call_noparam(uint64_t moduleID, uint64_t opc) {
}

int minx_kernel_module_gets_params(uint64_t moduleID) {
}

void minx_kernel_module_set_config(ConfigurationType type,
                                    ConfigurationValue *vals,
                                    unsigned int len) {
}

uint64_t minx_kernel_module_get_status(uint64_t moduleID) {
}

/*
 * static function implementations
 *
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

static Module* new_module(char *name) {
    Module *mod = malloc(sizeof(Module));

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

    return mod;
}
