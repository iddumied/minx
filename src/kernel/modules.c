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
 *
 * Anyway, the bytecode has only read-access to the known size, so still if the
 * module writes to the other part of the memory, the bytecode is not able to
 * read and it will be overridden, soon. So it is dead memory for this module
 * and thereon useless.
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
 *
 * Returns the module ID or zero on failure
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

        MOpcodes *mopc = mod->get_opcodes_func();
        mod->opcodes = (ModuleOpcode*) malloc(sizeof(ModuleOpcode) * mopc->len);

        unsigned int i;
        for(i = 0 ; i < mopc->len; i++ ) {
            mod->opcodes[i].opcode = mopc->m_opcodes[i];
            mod->opcodes[i].gets_params = 
                mod->opcode_gets_params_func(mod->opcodes[i].opcode);
        }
        mod->opcodes_count = mopc->len;
    }
    else {
        return 0;
    }

    return mod->module_id;
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
    Module *mod = find_module(moduleID);
    mod->call_func(opc, memory_helper, memory_data_size);
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
    ModuleOpcode *mop   = find_module_opcode(mod, op);
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

/**
 * @brief Get number of required parameters for this opcode
 *
 * @param moduleID The ID of the module
 * @param op The opcode to get the parameter-count for
 *
 * @return Number of parameters, zero if module/opcode was not found
 */
uint64_t minx_kernel_module_get_parameter_count(uint64_t moduleID, uint64_t op) {
    return find_module(moduleID)->get_opc_param_count_func(op);
}

/**
 * @brief 
 *
 * @param moduleID
 * @param op
 * @param heap
 *
 * @return 
 */
uint64_t minx_kernel_module_call_multiparameter_opcode( uint64_t moduleID, 
                                                        uint64_t op, 
                                                        HeapNode *metaheap) {

    /*
     * Each heap-node-address has sizeof(uint64_t) Bytes.
     * If there are bytes left, this causes the VPU to error hard.
     */
    if( metaheap->size % sizeof(uint64_t) != 0 ) {
        FATAL_F_ERROR("Cannot call mutliparameter opcode with multi-parameters"
                ", as there are to much byte (%"PRIu64") to identify them "
                "(must be multible of %lu",
                metaheap->size, sizeof(uint64_t)
                );
    }

    unsigned int    heapcount   = metaheap->size / 8;
    HeapNode        **heaps     = (HeapNode**)   malloc(sizeof(HeapNode*) * heapcount);
    char            **memories  = (char**)      malloc(sizeof(char*) * heapcount);
    uint64_t        *sizes      = (uint64_t*)   malloc(sizeof(uint64_t) * heapcount);
    uint64_t        curr_heapid;
    uint64_t        ret;
    unsigned int    i;

    for(i = 0; i < heapcount; i++) {
        curr_heapid     = ((uint64_t*)metaheap->memory)[i];
        heaps[i]        = minx_kernel_heap_get(curr_heapid);
        memories[i]     = heaps[i]->memory;
        sizes[i]        = heaps[i]->size;
    }

    ret = find_module(moduleID)->call_multiparam_func(op, memories, sizes, heapcount);

    free(heaps);
    free(memories);
    free(sizes);

    return ret;
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

    for(i = 0; i < m->opcodes_count && res == NULL; i++) {
        if( m->opcodes[i].opcode == opc )
            res = &(m->opcodes[i]);
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
        mod->opcodes_count              = 0;
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
            FATAL_F_ERROR("Could not reallocate memory (%"PRIu64" Bytes) helper for passing memory to module",
                    memory_helper_size);
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
        Module **save = modules;
        modules = realloc(modules, sizeof(Module*) * (modules_size+1));
        
        if( modules == NULL ) {
            modules = save;
            ret = 1;
        }
        else {
            modules[modules_size] = mod;
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
