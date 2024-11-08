#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "EnumColors.h"
#include "Stack.h"
#include "Spu.h"

//##------------------------------------MAIN------------------------------------##//

int main ()
    {
    struct spu Spu_struct;
    Ctor_Spu_Struct ( &Spu_struct );
    if ( Read_Spu_Code ( &Spu_struct ) == FAILURE )
        {
        fprintf ( stderr, "READ SPU CODE BAD\n" );
        return 1;
        }

    for ( size_t i = 0; i < Spu_struct.size_code; i++)
        {
        printf ("Ip code: %lld Element: %lld\n", i, Spu_struct.code[i] );
        }
    printf ( "\n" );

    Spu_Run ( &Spu_struct );
    Dtor_Spu_Struct ( &Spu_struct );
    return 0;
    }

//##----------------------------------------------------------------------------##//

//##----------------------------------CONSTRUCTION_SPU_STRUCT-------------------##//

int Ctor_Spu_Struct ( struct spu* Spu_struct )
    {
    Spu_struct->ip = 0;
    Spu_struct->Only_spu_registers = {0};
    STACK_CTOR( &Spu_struct->Spu_stack, 16, "code_stack" );
    return SUCCESSUFUL;
    }

//##----------------------------------------------------------------------------##//

//##----------------------------------OPEN_FILE---------------------------------##//

int Read_Spu_Code ( struct spu* Spu_struct )
    {
    FILE* CodeFOpen = fopen ( "CodeForSpu.bin", "r" );   /////////rb

    if (  CodeFOpen == NULL )
        {
        perror ( RED( "ERROR open file\n" ) );
        return FAILURE;
        }

    if ( fread ( &Spu_struct->size_code, sizeof(size_t), 1, CodeFOpen ) != 1 )
        {
        perror ( RED( "ERROR fread size_code" ) );
        return FAILURE;
        }

    Spu_struct->code = (stack_type*) calloc ( Spu_struct->size_code, sizeof(stack_type) );  

    if ( Spu_struct->code == NULL )
        {
        perror ( RED( "ERROR calloc for code" ) );
        return FAILURE;
        }

    if ( fread ( Spu_struct->code, sizeof(stack_type), Spu_struct->size_code, CodeFOpen ) != Spu_struct->size_code )
        {
        perror ( RED( "ERROR fread code" ) );
        return FAILURE;
        }

    if ( fclose ( CodeFOpen ) != 0 )
        {
        perror ( RED( "ERROR close file\n" ) );
        return FAILURE;
        }  

    return SUCCESSUFUL;
    }

//##----------------------------------------------------------------------------##//

//##----------------------------------SPU_RUN-----------------------------------##//
int Spu_Run ( struct spu* Spu_struct )
    {
    Spu_struct->hlt = false;
    while( Spu_struct->hlt == false )
        {
        if ( Spu_Switch ( Spu_struct ) == FAILURE )
            {
            return FAILURE;
            }
        }
        return SUCCESSUFUL;
    }

//##----------------------------------------------------------------------------##//

//##----------------------------------SPU_SWITCH--------------------------------##//

int Spu_Switch ( struct spu* Spu_struct )
    {
    //printf("Ip: %lld Code[ip]: %lld\n", Spu_struct->ip, Spu_struct->code[Spu_struct->ip]);
    switch ( Spu_struct->code [ Spu_struct->ip ] )
            {
            case CMD_PUSH_: { 
                            Cmd_Push__Func ( Spu_struct ); 
                            break; 
                            }
            case CMD_PUSHR: { 
                            Cmd_Pushr_Func ( Spu_struct ); 
                            break; 
                            }
            case CMD_POP__: { 
                            Cmd_Pop___Func ( Spu_struct ); 
                            break; 
                            }
            case CMD_ADD__: {
                            Cmd_Add___Func ( Spu_struct );
                            break;
                            }
            case CMD_SUB__: {                                                                                   //// Sub last num in stack
                            Cmd_Sub___Func ( Spu_struct );
                            break;
                            }
            case CMD_MUL__: {
                            Cmd_Mul___Func ( Spu_struct );
                            break;
                            }
            case CMD_DIV__: {                                                                                   //// Div on last num in stack
                            Cmd_Div___Func ( Spu_struct );
                            break;
                            }
            case CMD_IN___: {
                            Cmd_In____Func ( Spu_struct );
                            break;
                            }
            case CMD_OUT__: {
                            Cmd_Out___Func ( Spu_struct );
                            break;
                            }
            case CMD_SQRT_: {
                            Cmd_Sqrt__Func ( Spu_struct );
                            break;
                            }
            case CMD_SIN__: {                                                                                   //// Sin %lld
                            Cmd_Sin___Func ( Spu_struct );
                            break;
                            }
            case CMD_COS__: {                                                                                   //// Cos %lld
                            Cmd_Cos___Func ( Spu_struct );
                            break;
                            }
            case CMD_DUMP_: {
                            Cmd_Dump__Func ( Spu_struct );
                            break;
                            }
            case CMD_HLT__: {
                            Spu_struct->hlt = true;
                            return SUCCESSUFUL;
                            break;
                            }
            default:        {
                            fprintf ( stderr, RED( "ERORR: not a command\n" ) );
                            fprintf ( stderr, "Ip code: %lld Element: %lld\n", Spu_struct->ip-2, Spu_struct->code[Spu_struct->ip-2] );
                            fprintf ( stderr, "Ip code: %lld Element: %lld\n", Spu_struct->ip-1, Spu_struct->code[Spu_struct->ip-1] );
                            fprintf ( stderr, RED( "Ip code: %d Element: %d\n" ), Spu_struct->ip, Spu_struct->code[Spu_struct->ip] );
                            fprintf ( stderr, "Ip code: %lld Element: %lld\n", Spu_struct->ip+1, Spu_struct->code[Spu_struct->ip+1] );
                            fprintf ( stderr, "Ip code: %lld Element: %lld\n", Spu_struct->ip+2, Spu_struct->code[Spu_struct->ip+2] );
                            return FAILURE;
                            break;
                            }
            }
    return SUCCESSUFUL;
    }

void Cmd_Push__Func ( struct spu* Spu_struct )
    {
    Stack_Push ( &Spu_struct->Spu_stack, Spu_struct->code [ Spu_struct->ip + 1 ] * 1000 );
    Spu_struct->ip += 2;
    return;    
    }

void Cmd_Pushr_Func ( struct spu* Spu_struct )
    {
    Spu_struct->spu_registers[Spu_struct->code[Spu_struct->ip+1]] = Stack_Pop ( &Spu_struct->Spu_stack );
    Spu_struct->ip += 2;
    return;    
    }

void Cmd_Pop___Func ( struct spu* Spu_struct )
    {
    Stack_Push ( &Spu_struct->Spu_stack, Spu_struct->spu_registers[Spu_struct->code[Spu_struct->ip+1]] );   ////Spu_struct->spu_registers[Spu_struct->ip+1]
    Spu_struct->ip += 2;
    return;    
    }

void Cmd_Add___Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.ax = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.bx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.ax + Spu_struct->Only_spu_registers.bx ) * 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Sub___Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.cx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.dx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.dx - Spu_struct->Only_spu_registers.cx ) * 1000 );
    Spu_struct->ip ++; 
    return;       
    }

void Cmd_Mul___Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.ex = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.fx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.ex * Spu_struct->Only_spu_registers.fx ) * 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Div___Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.gx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.hx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.hx / Spu_struct->Only_spu_registers.gx ) * 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_In____Func ( struct spu* Spu_struct )
    {
    stack_type scan_param = 0;
    scanf  ( "%lld", &scan_param );
    scan_param *= 1000;
    Stack_Push ( &Spu_struct->Spu_stack, scan_param );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Out___Func ( struct spu* Spu_struct )
    {
    printf ( "%lld\n", Stack_Pop ( &Spu_struct->Spu_stack ) / 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Sqrt__Func ( struct spu* Spu_struct )
    {
    stack_type sqrt_result = (stack_type) sqrt ( Stack_Pop ( &Spu_struct->Spu_stack ) * 1000 );
    Stack_Push ( &Spu_struct->Spu_stack, sqrt_result );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Sin___Func ( struct spu* Spu_struct )
    {
    stack_type sin_result = (stack_type) sin ( Stack_Pop ( &Spu_struct->Spu_stack ) * 1000 );
    Stack_Push ( &Spu_struct->Spu_stack, sin_result );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Cos___Func ( struct spu* Spu_struct )
    {
    stack_type cos_result = (stack_type) cos ( Stack_Pop ( &Spu_struct->Spu_stack ) *1000 );
    Stack_Push ( &Spu_struct->Spu_stack, cos_result );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Dump__Func ( struct spu* Spu_struct )
    {
    /* code */
    return;
    }

//##----------------------------------------------------------------------------##//

//##---------------------------------DISTRUCT_SPU_STRUCT------------------------##//

void Dtor_Spu_Struct ( struct spu* Spu_struct )
    {
    Stack_Dtor ( &Spu_struct->Spu_stack );
    return;
    }

//##----------------------------------------------------------------------------##//
