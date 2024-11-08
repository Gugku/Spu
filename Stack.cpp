#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "EnumColors.h"
#include "Stack.h"

#define Def_Verification_Stack( StructAdress ) Verification_Stack( StructAdress, __LINE__, __FILE__, __func__ )

    /*------------------------------Stack_PUSH_&_POP------------------------------------------------------*/

int Stack_Push ( struct stack *StackStruct, stack_type argument )
    {
    Def_Verification_Stack( StackStruct );

    if ( Recalloc_Up   ( StackStruct ) == FAILURE )
        {
        printf ( 
        RED( "\n----------ERROR!----------\n" )
               "   Recalloc_Up Failure"
        RED( "\n--------------------------\n\n" ));
        return FAILURE;
        }

    Poison_Check_For_Stack ( StackStruct );
    StackStruct->Data[ StackStruct->size ] = argument;
    StackStruct->size ++;                                                            
    StackStruct->hash = Hash_For_Stack ( StackStruct );

    Def_Verification_Stack( StackStruct );
    return SUCCESSUFUL;
    }

stack_type Stack_Pop ( struct stack *StackStruct )
    {
    Def_Verification_Stack( StackStruct );

    if ( Recalloc_Down ( StackStruct ) == FAILURE )
        {
        printf ( 
        RED( "\n----------ERROR!----------\n" )
               "   Recalloc_Down Failure"
        RED( "\n--------------------------\n\n" ));
        }

    if ( StackStruct->size < 1 )
        {
        ///Dump
        assert(0);
        }
    StackStruct->size --; 
    stack_type pop = StackStruct->Data[ StackStruct->size ];                                           
    Poison_Add_For_Pop ( StackStruct );
    StackStruct->hash = Hash_For_Stack ( StackStruct );  

    Def_Verification_Stack( StackStruct );
    return pop;
    }

    /*----------------------------------------------------------------------------------------------------*/

    /*------------------------------CTOR_&DTOR------------------------------------------------------------*/

void* Stack_Ctor ( struct stack *StackStruct, int capacity,  int line, const char* namestack, const char* namefile, const char* func )                                                                                     ///?
    {
    StackStruct->capacity = capacity;
    StackStruct->size = 0;
    StackStruct->Data = ( stack_type* ) calloc ( StackStruct->capacity, sizeof( stack_type ) );
    if ( StackStruct->Data == NULL )
        {
        free ( StackStruct );
        return NULL;    
        }
    Poison_Add_For_Recal  ( StackStruct );
    StackStruct->hash = Hash_For_Stack ( StackStruct );
    StackStruct->Line_ctor = line;
    StackStruct->Name_ctor = namestack;
    StackStruct->Name_file_ctor = namefile;
    StackStruct->Func_ctor = func;
    return StackStruct; /// был &
    }

void Stack_Dtor ( struct stack* StackStruct )
    {
    free ( StackStruct->Data );
    return;    
    }

    /*----------------------------------------------------------------------------------------------------*/

    /*------------------------------Recalloc_UP_&_DOWN----------------------------------------------------*/

int Recalloc_Up ( struct stack *StackStruct )
    {
    if ( StackStruct->capacity == StackStruct->size )
        {
        if ( StackStruct->capacity == 0 )
            {
            StackStruct->capacity = 16;   
            } 
        else 
            {
            StackStruct->capacity = StackStruct->capacity*2;    
            }

        void *intermediate_adress = Recalloc ( StackStruct->Data, StackStruct->size, StackStruct->capacity, sizeof( stack_type ) ); 

        if ( intermediate_adress == NULL || StackStruct->capacity < StackStruct->size )
            {
            Stack_Dtor ( StackStruct );
            return FAILURE;    
            }
        StackStruct->Data = ( stack_type* ) intermediate_adress;
        Poison_Add_For_Recal  ( StackStruct );  
        }
    return SUCCESSUFUL;
    }

int Recalloc_Down ( struct stack *StackStruct )
    {
    if ( StackStruct->capacity == 16 )
        {
        return SUCCESSUFUL;
        }
    else if ( ( StackStruct->capacity / 4 ) == StackStruct->size )
        {
        StackStruct->capacity   = ( StackStruct->capacity / 2 ); 
        void *intermediate_adress = Recalloc ( StackStruct->Data, StackStruct->size, StackStruct->capacity, sizeof( stack_type ) ); 

        if ( intermediate_adress == NULL || StackStruct->capacity < StackStruct->size )
            {
            Stack_Dtor ( StackStruct );
            return FAILURE;    
            }
        StackStruct->Data = ( stack_type* ) intermediate_adress;
        } 
    return SUCCESSUFUL;
    }

void* Recalloc ( void *Data, size_t oldnum, size_t newnum, size_t size )
    {
    Data = realloc ( Data, size*newnum );  // check
    if ( newnum <= oldnum )
        {
        memset ( Data + size*oldnum, 0, size* (newnum - oldnum ) );
        }
    return Data;
    }

    /*----------------------------------------------------------------------------------------------------*/

    /*------------------------------Poison_ADD_&_CHECK----------------------------------------------------*/

void Poison_Add_For_Recal  ( struct stack *StackStruct )
    {
    assert ( StackStruct->capacity > StackStruct->size );                                                   /// Капасити точно больше иначе бы функция не вызвалась

    for ( stack_type i = StackStruct->size; i < StackStruct->capacity; i++ )
        {
        StackStruct->Data[i] = POISON;                                                                      
        }
    return;
    }

void Poison_Add_For_Pop    ( struct stack *StackStruct )
    {
    StackStruct->Data[ StackStruct->size ] = POISON;                                                        
    return;
    }

int Poison_Check_For_Stack ( struct stack *StackStruct )
    {
    if ( StackStruct->Data[ StackStruct->size] != POISON && StackStruct->capacity != StackStruct->size )                                              
        {
        printf ( 
        VIOLET( "\n---------WARNING!---------\n" )
                  "   Poison is not right"
        VIOLET( "\n--------------------------\n\n" ));
        return FAILURE;    
        }
    return SUCCESSUFUL;
    }

    /*----------------------------------------------------------------------------------------------------*/

    /*------------------------------HASH_/_CHECKSUM-------------------------------------------------------*/

stack_type Hash_For_Stack ( struct stack *StackStruct )
    {
    stack_type hash = 0;
    for ( stack_type i = 0; i < StackStruct->capacity; i++ )
        {
        hash += StackStruct->Data[i];                                                                          
        }
    return hash;    
    }

    /*----------------------------------------------------------------------------------------------------*/

    /*------------------------------VERIFICATION----------------------------------------------------------*/

int Verification_Stack ( struct stack *StackStruct, int line, const char* namefile, const char* func )
    {
    int status_func = SUCCESSUFUL;
    if ( StackStruct->hash != Hash_For_Stack ( StackStruct ) )
        {
        printf ( 
        RED( "\n----------ERROR!----------\n" )
        "The hash did not match during verification"
        RED( "\n--------------------------\n\n" ));
        status_func = FAILURE;    
        }
    if ( StackStruct->capacity < StackStruct->size )
        {
        printf ( 
        RED( "\n----------ERROR!----------\n" )
        "The capacity is larger than the size"
        RED( "\n--------------------------\n\n" ));
        status_func = FAILURE;
        }
    Poison_Check_For_Stack ( StackStruct );
    return status_func;    
    }

    /*----------------------------------------------------------------------------------------------------*/

    /*------------------------------DUMP_PRINTFILE--------------------------------------------------------*/

/*void Dump ( struct stack* StackStruct, int line, int function, int code_error )
    {
    switch ( code_error )
        {
        case REALLOC_UP_ERROR:      printf ( RED( "\n----------ERROR!----------\n" )
                                                    "   Recalloc_Up Failure"                            нужно делать в файл
                                             RED( "\n--------------------------\n\n" ));
                                    break;
        default: 
                                    break;
        }
    return;
    }*/

    /*----------------------------------------------------------------------------------------------------*/

#undef Def_Verification_Stack