#ifndef STACK_H
#define STACK_H

#define STACK_CTOR( StructAdress, capacity, StackName ) Stack_Ctor( StructAdress, capacity, __LINE__, StackName, __FILE__, __func__ )
//STACK_CTOR( &first_stack, 16, "first_stack" );
//Stack_Ctor ( &first_stack, 16, __LINE__, "first_stack", "Main.cpp", "main" );

//#define Def_Verification_Stack( StructAdress ) Verification_Stack( StructAdress, __LINE__, __FILE__, __func__ )

//Dump ( struct stack* StackStruct, int line, int function, int code_error )

//#define VERIF Verification_Stack(...., __LINE__, __FILE__, __func__);

typedef long long int stack_type;                                                           ///In Spu printf, scanf %lld

struct stack 
    {
    stack_type *Data;
    stack_type size;
    stack_type capacity;
    stack_type hash;
    const char *Name_file_ctor;
    const char *Name_ctor;
    const char *Func_ctor;
    int Line_ctor;
    };

enum /// code errors for dump
    {
    REALLOC_UP_ERROR = 100,
    REALLOC_DOWN_ERROR = 101,
    CAPACITY_BIGGER_ERROR = 999,
    HASH_ERROR = 200,
    POISON_ELEMENT_ERROR = 201


    };

///Functions
int         Stack_Push              ( struct stack *StackStruct, stack_type argument );
stack_type  Stack_Pop               ( struct stack *StackStruct );
void*       Stack_Ctor              ( struct stack *StackStruct, int capacity,  int line, const char* namestack, const char* namefile, const char* func );
void        Stack_Dtor              ( struct stack* StackStruct );
int         Recalloc_Up             ( struct stack *StackStruct );
int         Recalloc_Down           ( struct stack *StackStruct );
void*       Recalloc                ( void *Data, size_t oldnum, size_t newnum, size_t size );
void        Poison_Add_For_Recal    ( struct stack *StackStruct );
void        Poison_Add_For_Pop      ( struct stack *StackStruct );
int         Poison_Check_For_Stack  ( struct stack *StackStruct );
stack_type  Hash_For_Stack          ( struct stack *StackStruct );
int         Verification_Stack      ( struct stack *StackStruct, int line, const char* namefile, const char* func );

#endif