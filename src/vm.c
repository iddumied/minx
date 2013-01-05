#include "vm.h"

/*
 *
 */
#define DEFAULT_TYPES_CNT   2
struct type {
    uint64_t ident;
    uint64_t included_types_cnt;
    uint64_t *included_types_idents; 
};
typedef struct type Type;

struct variable { /* used as argument, too */
    uint64_t ident;
    uint64_t type_ident;
    void * value;
}
typedef struct variable Variable
   
#define DEFAULT_FUNCTIONS_CNT 15
struct function {
    uint64_t ident;
    uint64_t return_type_ident;
    uint64_t arg_cnt;
    Variable * variables; /* used for arguments, too */
};
typedef struct function Function;

static Function * functions;

/*
 * Static functions
 */
static void allocate_basic_types(void);
static void read_header(void);
static void read_types(void);
static void read_functions(void);

/*
 * Static variables 
 */

static char * version;
static char * length;
static char * function_count;
static char * function_def_ptr; // not supported yet
static char * type_count;
static char * type_def_ptr; // not supported yet

static Type * types;

/*
 * Functions
 */

void minx_vm_init() {
    allocate_basic_types();

    read_header();
    read_types();
}

static void allocate_basic_types() {
    types = (Type*) malloc( sizeof(Type) * DEFAULT_TYPES_CNT );

    /* basic type 0: Byte */
    types[0]->ident                 = 0;
    types[0]->included_types_cnt    = 0;
    types[0]->included_types        = NULL;

    /* basic type 1: Function */
    types[1]->ident                 = 1;
    types[1]->included_types_cnt    = 0;
    types[1]->included_types        = NULL;

}

static void allocate_basic_functions() {
    /* TODO */
}

static void read_header() {
    version          = minx_consume_bytes(L_VERSION);
    length           = minx_consume_bytes(L_LENGTH);
    function_count   = minx_consume_bytes(L_FUNCTION_COUNT);
    function_def_ptr = minx_consume_bytes(L_FUNCTION_DEF_PTR);
    type_count       = minx_consume_bytes(L_TYPES_COUNT);
    type_def_ptr     = minx_consume_bytes(L_TYPES_DEF_PTR);
    
    if( minx_option_is_set(OPT_VERBOSE) ) {
        printf(
                "minx binary version:    %c\n"
                "minx binary length:     %s\n"
                "minx function count:    %s\n"
                "minx function def ptr:  %s\n"
                "minx type count:        %s\n"
                "minx type def ptr:      %s\n",

                version,
                length,
                function_count,
                function_def_ptr,
                type_count,
                type_def_ptr);
    }
}

static void read_types() {
    types = (Type *) malloc( sizeof(Type) * (type_count + DEFAULT_TYPES_CNT) );
    
    uint64_t i;
    uint64_t j;

#define curr_t (&(types[i]))
    for( i = 0 ; i < (uint64_t) *type_count; i++ ) {
        curr_t->ident              = minx_consume_bytes( L_TYPE_IDENTIFICATION );
        curr_t->included_types_cnt = minx_consume_bytes( L_TYPE_DEF_INCLUDED_TYPES );
        
        curr_t->types_idents = (uint64_t *) malloc( 
                uint64_t * curr_t->included_types_cnt );

        for( j = 0 ; j < curr_t->included_types_cnt ; j++ ) {
            curr_t->included_types_idents[j] = minx_consume_bytes( L_TYPE_IDENTIFICATION );    
        }
    }
#undef

}

static void read_functions() {
    uint64_t i, j;
    functions = (Function*) malloc(sizeof(Function) * (function_count + DEFAULT_FUNCTIONS_CNT));

#define curr_f (&(functions[i]))
    for( i = 0 ; i < function_count ; i++ ) {
        curr_f->ident               = minx_consume_bytes( L_FUNCTION_IDENTIFICATION );
        curr_f->return_type_ident   = minx_consume_bytes( L_TYPE_IDENTIFICATION );
        curr_f->arg_cnt             = minx_consume_bytes( L_FUNCTION_ARGUMENT_COUNT );

        /* arguments */

        curr_t->variables = (Variable*) malloc( sizeof(Variable)*curr_f->arg_cnt );

#define curr_v (&(variables[j]))
        for( j = 0 ; j < curr_f->arg_cnt ; j++ ) {
            curr_f->curr_v->ident               = minx_consume_bytes( L_FUNCTION_ARGUMENT_NUMBER );
            curr_f->curr_v->return_type_ident   = minx_consume_bytes( L_TYPE_IDENTIFICATION );
            curr_f->curr_v->value               = NULL;
        }
#undef curr_v

        /* block */

    }
#undef curr_f
}
