#ifndef __MINX_VPU_OPCODES_H__
#define __MINX_VPU_OPCODES_H__

#define         OPC_NOP			    0x00
#define         OPC_CALL            0x01
#define         OPC_RET			    0x02

#define         OPC_MOV			    0x03
#define         OPC_MOVI		    0x04

#define         OPC_NOT			    0x05
#define         OPC_NOTR		    0x06

#define         OPC_AND			    0x07
#define         OPC_ANDI		    0x08
#define         OPC_ANDR		    0x09
#define         OPC_ANDIR		    0x0A

#define         OPC_OR			    0x0B
#define         OPC_ORI			    0x0C
#define         OPC_ORR			    0x0D
#define         OPC_ORIR		    0x0E

#define         OPC_DEC			    0x0F
#define         OPC_INC			    0x10

#define         OPC_LSH			    0x11
#define         OPC_RSH			    0x12

#define         OPC_CMP             0x13
#define         OPC_CMPI            0x14
#define         OPC_EQL             0x15
#define         OPC_EQLI            0x16

#define         OPC_PUSH		    0x20
#define         OPC_POP			    0x21
#define         OPC_DROP		    0x22

#define         OPC_ADD			    0x30
#define         OPC_ADDI		    0x31
#define         OPC_ADDR		    0x32
#define         OPC_ADDIR		    0x33

#define         OPC_JMP			    0x40
#define         OPC_JMPIZ		    0x41
#define         OPC_JMPNZ		    0x42
#define         OPC_IFZJMP		    0x43
#define         OPC_EXIT            0x44

#define         OPC_PSTACK          0x50
#define         OPC_PREGS           0x51
#define         OPC_PPROG           0x52
#define         OPC_PMEMS           0x53
#define         OPC_PMEM            0x54

#define         OPC_ALLOC           0x60
#define         OPC_ALLOCI          0x61
#define         OPC_RESIZE          0x62
#define         OPC_RESIZEI         0x63
#define         OPC_FREE            0x64
#define         OPC_PUT             0x65
#define         OPC_READ            0x66
#define         OPC_GETSIZE         0x67

#endif // __MINX_VPU_OPCODES_H__

