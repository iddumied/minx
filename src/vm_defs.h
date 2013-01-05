#ifndef __MINX_VM_DEFS_H__
#define __MINX_VM_DEFS_H__

/*
 * This macros define the length of the information from the binary.
 *
 * Example: The version information of the binary is 1 byte, so it's defined as
 * 1 byte here.
 *
 */

/* header information */
#define L_VERSION                                     1
#define L_LENGTH                                      256
#define L_FUNCTION_COUNT                              4
#define L_FUNCTION_DEF_PTR                            64
#define L_TYPES_COUNT                                 4
#define L_TYPES_DEF_PTR                               64

/* types information */
#define L_TYPE_IDENTIFICATION                         8
#define L_TYPE_DEF_INCLUDED_TYPES                     8

/* functions information */
#define L_FUNCTION_IDENTIFICATION                     8
/*
#define L_FUNCTION_RETURN_VALUE_TYPE_IDENTIFICATION   TYPE_IDENTIFICATION_LENGTH
*/

#define L_FUNCTION_ARGUMENT_COUNT                     8
#define L_FUNCTION_ARGUMENT_NUMBER                    8
/*
#define L_FUNCTION_ARGUMENT_TYPE_IDENTIFICATION       TYPE_IDENTIFICATION_LENGTH
*/

#define L_FUNCTION_BLOCK_LENGTH                       64
#define L_FUNCTION_BLOCK_VARIABLE_MAP_LENGTH          32
#define L_FUNCTION_BLOCK_VARIABLE_IDENTIFICATION      16
/*
#define L_FUNCTION_BLOCK_VARIABLE_TYPE_IDENTIFICATION TYPE_IDENTIFICATION
*/

/*
#define L_FUNCTION_BLOCK_FUNCTION_CALL_FUNCTION_IDENTIFICATION    FUNCTION_IDENTIFICATION 
*/

#endif //__MINX_VM_DEFS_H__

