#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include <sys/stat.h>

#include "EnumColors.h"
#include "Stack.h"
#include "Spu.h"

//##------------------------------------MAIN------------------------------------##//

int main ()
    {
    ///struct spu* Spu_struct = Ctor_Spu_Struct();
    struct spu Spu_struct;
    Ctor_Spu_Struct ( &Spu_struct );
    //fprintf ( stderr, "CTOR OK\n" );
    if ( Read_Spu_Code ( &Spu_struct ) == FAILURE )
        {
        fprintf ( stderr, "READ SPU CODE BAD\n" );
        return 1;
        }
    //fprintf ( stderr, "READ SPU CODE OK\n" );
    Spu_Run ( &Spu_struct );
    //fprintf ( stderr, "SPU RUN OK\n" );
    Dtor_Spu_Struct ( &Spu_struct );
     //fprintf ( stderr, "SPU DTOR OK\n" );
    
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

    //fprintf ( stderr, "FILE* CodeFOpen OK\n" );

    if (  CodeFOpen == NULL )
        {
        perror ( RED( "ERROR open file\n" ) );
        return FAILURE;
        }

    //fprintf ( stderr, "CodeFOpen == NULL OK\n" );

    /*struct stat data = {};
    assert( stat("CodeForSpu.bin", &data) != -1);
    char *buf = (char *) calloc (data.st_size + 1, sizeof(char));
    assert(buf);*/

    /*fprintf ( stderr, "Size code %llu\n", &Spu_struct->size_code );
    fprintf(stderr, "begin fread");
    size_t a = fread ( &Spu_struct->size_code, sizeof(size_t), 1, CodeFOpen );
    size_t a = fread(buf, sizeof(char), data.st_size, CodeFOpen);
    fprintf ( stderr,  "Size code %llu\n", &Spu_struct->size_code );
    fprintf ( stderr, "a %lu\n", a );
    if ( a != 8 )
        {
        perror ( RED( "ERROR fread size_code" ) );
        return FAILURE;
        }*/
    if ( fread ( &Spu_struct->size_code, sizeof(size_t), 1, CodeFOpen ) != 1 )
        {
        perror ( RED( "ERROR fread size_code" ) );
        return FAILURE;
        }

    //fprintf ( stderr, "Fread size code OK\n" );

    Spu_struct->code = (stack_type*) calloc ( Spu_struct->size_code, sizeof(stack_type) );  

    //fprintf ( stderr, "Calloc for code OK\n" );

    if ( Spu_struct->code == NULL )
        {
        perror ( RED( "ERROR calloc for code" ) );
        return FAILURE;
        }

    //fprintf ( stderr, "Spu_struct->code == NULL OK\n" );

    if ( fread ( Spu_struct->code, sizeof(stack_type), Spu_struct->size_code, CodeFOpen ) != Spu_struct->size_code )
        {
        perror ( RED( "ERROR fread code" ) );
        return FAILURE;
        }

    //fprintf ( stderr, "1 OK\n" );

    if ( fclose ( CodeFOpen ) != 0 )
        {
        perror ( RED( "ERROR close file\n" ) );
        return FAILURE;
        }  

    //fprintf ( stderr, "2 OK\n" );

    return SUCCESSUFUL;
    }

//##----------------------------------------------------------------------------##//

//##----------------------------------SPU_RUN-----------------------------------##//
int Spu_Run ( struct spu* Spu_struct )
    {
    Spu_struct->hlt = false;
    while( Spu_struct->hlt == false )
        {
        Spu_Switch ( Spu_struct );
        }
        return SUCCESSUFUL;
    }

//##----------------------------------------------------------------------------##//

//##----------------------------------SPU_SWITCH--------------------------------##//

int Spu_Switch ( struct spu* Spu_struct )
    {
    //printf("%d %d\n", Spu_struct->ip, Spu_struct->code[Spu_struct->ip]);
    switch ( Spu_struct->code [ Spu_struct->ip ] )
            {
            case CMD_PUSH:  { 
                            Cmd_Push_Func ( Spu_struct ); 
                            break; 
                            }
            case CMD_ADD_:  {
                            Cmd_Add__Func ( Spu_struct );
                            break;
                            }
            case CMD_SUB_:  {                                                                                    //// Sub last num in stack
                            Cmd_Sub__Func ( Spu_struct );
                            break;
                            }
            case CMD_MUL_:  {
                            Cmd_Mul__Func ( Spu_struct );
                            break;
                            }
            case CMD_DIV_:  {                                                                                   //// Div on last num in stack
                            Cmd_Div__Func ( Spu_struct );
                            break;
                            }
            case CMD_IN__:  {
                            Cmd_In___Func ( Spu_struct );
                            break;
                            }
            case CMD_OUT_:  {
                            Cmd_Out__Func ( Spu_struct );
                            break;
                            }
            case CMD_SQRT:  {
                            Cmd_Sqrt_Func ( Spu_struct );
                            break;
                            }
            case CMD_SIN_:  {                                                                                   //// Sin %lld
                            Cmd_Sin__Func ( Spu_struct );
                            break;
                            }
            case CMD_COS_:  {                                                                                   //// Cos %lld
                            Cmd_Cos__Func ( Spu_struct );
                            break;
                            }
            case CMD_DUMP:  
                            Cmd_Dump_Func ( Spu_struct );
                            break;
            case CMD_HLT_:  {
                            Spu_struct->hlt = true;
                            return SUCCESSUFUL;
                            break;
                            }
            default:        {
                            printf ( RED( "ERORR: not a command\n" ) );
                            return FAILURE;
                            break;
                            }
            }
    return SUCCESSUFUL;
    }

void Cmd_Push_Func ( struct spu* Spu_struct )
    {
    Stack_Push ( &Spu_struct->Spu_stack, Spu_struct->code [ Spu_struct->ip + 1 ] * 1000 );
    Spu_struct->ip += 2;
    return;    
    }

void Cmd_Add__Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.ax = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.bx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.ax + Spu_struct->Only_spu_registers.bx ) * 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Sub__Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.cx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.dx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.dx - Spu_struct->Only_spu_registers.cx ) * 1000 );
    Spu_struct->ip ++; 
    return;       
    }

void Cmd_Mul__Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.ex = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.fx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.ex * Spu_struct->Only_spu_registers.fx ) * 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Div__Func ( struct spu* Spu_struct )
    {
    Spu_struct->Only_spu_registers.gx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Spu_struct->Only_spu_registers.hx = Stack_Pop ( &Spu_struct->Spu_stack ) / 1000;
    Stack_Push ( &Spu_struct->Spu_stack, ( Spu_struct->Only_spu_registers.hx / Spu_struct->Only_spu_registers.gx ) * 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_In___Func ( struct spu* Spu_struct )
    {
    stack_type scan_param;
    scanf  ( "%lld\n", &scan_param );
    scan_param *= 1000;
    Stack_Push ( &Spu_struct->Spu_stack, scan_param );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Out__Func ( struct spu* Spu_struct )
    {
    printf ( "%lld\n", Stack_Pop ( &Spu_struct->Spu_stack ) / 1000 );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Sqrt_Func ( struct spu* Spu_struct )
    {
    stack_type sqrt_result = (stack_type) sqrt ( Stack_Pop ( &Spu_struct->Spu_stack ) * 1000 );
    Stack_Push ( &Spu_struct->Spu_stack, sqrt_result );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Sin__Func ( struct spu* Spu_struct )
    {
    stack_type sin_result = (stack_type) sin ( Stack_Pop ( &Spu_struct->Spu_stack ) * 1000 );
    Stack_Push ( &Spu_struct->Spu_stack, sin_result );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Cos__Func ( struct spu* Spu_struct )
    {
    stack_type cos_result = (stack_type) cos ( Stack_Pop ( &Spu_struct->Spu_stack ) *1000 );
    Stack_Push ( &Spu_struct->Spu_stack, cos_result );
    Spu_struct->ip ++;
    return;
    }

void Cmd_Dump_Func ( struct spu* Spu_struct )
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
