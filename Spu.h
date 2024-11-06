#ifndef SPU_H
#define SPU_H

#include "Stack.h"

struct registers
    {
    stack_type ax;
    stack_type bx;
    stack_type cx;
    stack_type dx;
    stack_type ex;
    stack_type fx;
    stack_type gx;
    stack_type hx;
    };

struct spu
    { 
    int           ip;
    bool         hlt;
    size_t size_code;
    stack_type* code;
    stack_type    spu_registers[16] = {0}; 
    struct stack                Spu_stack;
    struct registers   Only_spu_registers;
    };

enum : stack_type
    {
    CMD_PUSH = 0x01,
    CMD_ADD_ = 0x02,
    CMD_SUB_ = 0x03,
    CMD_MUL_ = 0x04,
    CMD_DIV_ = 0x05,
    CMD_OUT_ = 0x06,
    CMD_IN__ = 0x07,
    CMD_SQRT = 0x08,
    CMD_SIN_ = 0x09,
    CMD_COS_ = 0x0A,
    CMD_DUMP = 0x0B,
    CMD_HLT_ = 0x0C,
    };
int         Ctor_Spu_Struct     ( struct spu* Spu_struct );
int         Read_Spu_Code       ( struct spu* Spu_struct );
void        Dtor_Spu_Struct     ( struct spu* Spu_struct );
int         Spu_Run             ( struct spu* Spu_struct );
int         Spu_Switch          ( struct spu* Spu_struct );

//-------------Cmd_####_Func----------------//
void Cmd_Push_Func ( struct spu* Spu_struct );
void Cmd_Add__Func ( struct spu* Spu_struct );
void Cmd_Sub__Func ( struct spu* Spu_struct );
void Cmd_Mul__Func ( struct spu* Spu_struct );
void Cmd_Div__Func ( struct spu* Spu_struct );
void Cmd_In___Func ( struct spu* Spu_struct );
void Cmd_Out__Func ( struct spu* Spu_struct );
void Cmd_Sqrt_Func ( struct spu* Spu_struct );
void Cmd_Sin__Func ( struct spu* Spu_struct );
void Cmd_Cos__Func ( struct spu* Spu_struct );
void Cmd_Dump_Func ( struct spu* Spu_struct );
//------------------------------------------//

#endif