#ifndef ASM_H
#define ASM_H

#include "Stack.h"

struct assembler 
    {
    FILE* code_for_asm;
    size_t size_code_for_asm;
    stack_type* data_bin_spu;
    size_t size_code_for_spu;
    };

/*enum : stack_type
    {
    CMD_PUSH_ = 0x01,
    CMD_PUSHR = 0x02,
    CMD_POP__ = 0x03,
    CMD_ADD__ = 0x04,
    CMD_SUB__ = 0x05,
    CMD_MUL__ = 0x06,
    CMD_DIV__ = 0x07,
    CMD_OUT__ = 0x08,
    CMD_IN___ = 0x09,
    CMD_SQRT_ = 0x0A,
    CMD_SIN__ = 0x0B,
    CMD_COS__ = 0x0C,
    CMD_DUMP_ = 0x0D,
    CMD_HLT__ = 0x0E,
    };*/ ///used from spu

int Convert_Cmd_Asm_to_bin ( struct assembler* AsmStruct, char arrchar_fscanf[16] );
int Finding_Name_Register ( char name_register[2] );

//---------------Convert_#####------------------//
int Convert_Push  ( struct assembler* AsmStruct );
int Convert_Pop   ( struct assembler* AsmStruct );
int Convert_Add   ( struct assembler* AsmStruct );
int Convert_Sub   ( struct assembler* AsmStruct );
int Convert_Mul   ( struct assembler* AsmStruct );
int Convert_Div   ( struct assembler* AsmStruct );
int Convert_Out   ( struct assembler* AsmStruct );
int Convert_In    ( struct assembler* AsmStruct );
int Convert_Sqrt  ( struct assembler* AsmStruct );
int Convert_Sin   ( struct assembler* AsmStruct );
int Convert_Cos   ( struct assembler* AsmStruct );
int Convert_Dump  ( struct assembler* AsmStruct );
int Convert_Hlt   ( struct assembler* AsmStruct );
//----------------------------------------------//
#endif