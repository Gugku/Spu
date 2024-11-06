#ifndef ASM_H
#define ASM_H

#include "Stack.h"

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

#endif