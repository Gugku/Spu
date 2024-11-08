#ifndef SPU_H
#define SPU_H

#include "Stack.h"

static const int    num_registers = 8;
static const char*  spu_registers_for_asm[num_registers] = { "ax", "bx", "cx", "dx", "ex", "fx", "gx", "hx" };

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
    stack_type    spu_registers[num_registers] = {0}; 
    struct stack                Spu_stack;
    struct registers   Only_spu_registers;
    };

enum : stack_type
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
    };
int         Ctor_Spu_Struct     ( struct spu* Spu_struct );
int         Read_Spu_Code       ( struct spu* Spu_struct );
void        Dtor_Spu_Struct     ( struct spu* Spu_struct );
int         Spu_Run             ( struct spu* Spu_struct );
int         Spu_Switch          ( struct spu* Spu_struct );

//-------------Cmd_#####_Func----------------//
void Cmd_Push__Func ( struct spu* Spu_struct );
void Cmd_Pushr_Func ( struct spu* Spu_struct );
void Cmd_Pop___Func ( struct spu* Spu_struct );
void Cmd_Add___Func ( struct spu* Spu_struct );
void Cmd_Sub___Func ( struct spu* Spu_struct );
void Cmd_Mul___Func ( struct spu* Spu_struct );
void Cmd_Div___Func ( struct spu* Spu_struct );
void Cmd_In____Func ( struct spu* Spu_struct );
void Cmd_Out___Func ( struct spu* Spu_struct );
void Cmd_Sqrt__Func ( struct spu* Spu_struct );
void Cmd_Sin___Func ( struct spu* Spu_struct );
void Cmd_Cos___Func ( struct spu* Spu_struct );
void Cmd_Dump__Func ( struct spu* Spu_struct );
//-------------------------------------------//

#endif