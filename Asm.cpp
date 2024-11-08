#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "EnumColors.h"
#include "Stack.h"
#include "Spu.h"
#include "Asm.h"

int Asm_Run ( struct assembler* AssemblerStruct );

//##------------------------------------MAIN------------------------------------##//

int main ()
    {
    struct assembler AsmStruct;
    AsmStruct.code_for_asm = NULL;
    AsmStruct.data_bin_spu = NULL;
    AsmStruct.size_code_for_asm = 0;
    AsmStruct.size_code_for_spu = 0;
    
    AsmStruct.code_for_asm = fopen ( "CodeForAsm.txt", "r" );
    if ( AsmStruct.code_for_asm == NULL )
        {
        perror ( RED( "ERROR open file\n" ) );
        return 1;
        }

    fseek ( AsmStruct.code_for_asm, 0, SEEK_END );
    long int ftell_result_size_code_for_asm = ftell ( AsmStruct.code_for_asm );
    if ( ftell_result_size_code_for_asm == -1L )
        {
        perror ( RED( "ERROR ftell code_for_asm\n" ) );
        return 1;
        }
    AsmStruct.size_code_for_asm = ftell_result_size_code_for_asm;
    rewind ( AsmStruct.code_for_asm );

    AsmStruct.data_bin_spu = (stack_type*) calloc ( AsmStruct.size_code_for_asm , sizeof(stack_type) );
    
    Asm_Run ( &AsmStruct );
    AsmStruct.size_code_for_spu++;                                                                              ///Because he used which ip

    if ( fclose( AsmStruct.code_for_asm ) != 0 )
        {
        perror ( RED( "ERROR close file\n" ) );
        return 1;
        }

    FILE* spu_code_file = fopen ( "CodeForSpu.bin", "wb" );
    if (  spu_code_file == NULL )
        {
        perror ( RED( "ERROR open file\n" ) );
        return 1;
        }
    fwrite ( &AsmStruct.size_code_for_spu, sizeof(size_t), 1, spu_code_file );
    fwrite ( AsmStruct.data_bin_spu, sizeof(stack_type), AsmStruct.size_code_for_spu, spu_code_file );
     if ( fclose( spu_code_file ) != 0 )
        {
        perror ( RED( "ERROR close file\n" ) );
        return 1;
        }  
    return 0;

 /*   size_t len = 7;
    stack_type arr [7] = { 0x01, 1, 0x01, 1, 0x02, 0x06, 0x0C };
    FILE* CodeFOpen = fopen ( "CodeForSpu.bin", "wb" );
    if (  CodeFOpen == NULL )
        {
        perror ( RED( "ERROR open file\n" ) );
        return 1;
        }
    fwrite ( &len, sizeof(size_t), 1, CodeFOpen );
    fwrite ( arr, sizeof(stack_type), 7, CodeFOpen );
     if ( fclose( CodeFOpen ) != 0 )
        {
        perror ( RED( "ERROR close file\n" ) );
        return 1;
        }  
    return 0;
    */

    }

//##----------------------------------------------------------------------------##//

int Asm_Run ( struct assembler* AsmStruct )
    {
    while ( true )
        {
        char arrchar_fscanf[16] = {0}; 
        fscanf ( AsmStruct->code_for_asm, "%s" , arrchar_fscanf );

        int result_operation = Convert_Cmd_Asm_to_bin ( AsmStruct, arrchar_fscanf );
        if ( result_operation == FAILURE )
            {
            return FAILURE;
            }

        int skip_fgetc = fgetc ( AsmStruct->code_for_asm ); 
        while ( skip_fgetc != '\n')
            {
            if ( skip_fgetc == EOF )
                {
                return SUCCESSUFUL;
                }
            skip_fgetc = fgetc ( AsmStruct->code_for_asm );
            }
        }
    return FAILURE;
    }

int Convert_Cmd_Asm_to_bin ( struct assembler* AsmStruct, char arrchar_fscanf[16] )
    {
    if      ( strcmp ( arrchar_fscanf, "push" ) == 0 )
        {
        return Convert_Push ( AsmStruct );    
        }
    else if ( strcmp ( arrchar_fscanf, "pop" )  == 0 )
        {
        return Convert_Pop ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "add" )  == 0 )
        {
        return Convert_Add ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "sub" )  == 0 )
        {
        return Convert_Sub ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "mul" )  == 0 )
        {
        return Convert_Mul ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "div" )  == 0 )
        {
        return Convert_Div ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "out" )  == 0 )
        {
        return Convert_Out ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "in" )   == 0 )
        {
        return Convert_In ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "sqrt" ) == 0 )
        {
        return Convert_Sqrt ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "sin" )  == 0 )
        {
        return Convert_Sin ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "cos" )  == 0 )
        {
        return Convert_Cos ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "dump" ) == 0 )
        {
        return Convert_Dump ( AsmStruct );
        }
    else if ( strcmp ( arrchar_fscanf, "hlt" )  == 0 )
        {
        return Convert_Hlt ( AsmStruct );
        }
    else
        {
        return FAILURE;
        }
    }

int Convert_Push ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Push  -> " );

    while ( fgetc ( AsmStruct->code_for_asm ) == ' ' );
    fseek ( AsmStruct->code_for_asm, -1, SEEK_CUR );

    stack_type argument;
    char name_register[2] = {0};
    if ( fscanf ( AsmStruct->code_for_asm, "%lld" , &argument ) != 0 )
        { 
        printf ( "push %lld   -> ", argument );
        AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_PUSH_;
        AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = argument;
        printf ( "(total) %lld %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-2], AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
        }
    else if ( fscanf ( AsmStruct->code_for_asm, "%s" , name_register ) == 1 )
        {
        printf ( "pushr %s -> ", name_register );
        stack_type bin_name_register = Finding_Name_Register ( name_register );
        printf ( "pushr %lld -> ", bin_name_register );
        if ( bin_name_register == POISON )
            {
            printf ( RED( "FAILURE bin_name_register\n" ) );
            return FAILURE;
            }
        else
            {
            AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_PUSHR;
            AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = bin_name_register;
            printf ( "(total) %lld %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-2], AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
            }    
        }
     else
        {
        printf ( RED( "ERROR: push not argument or register\n" ) );
        return FAILURE;
        }

    return SUCCESSUFUL;
    }

int Finding_Name_Register ( char name_register[2] )
    {
    for ( int i = 0; i < num_registers; i++ )
        {
        if ( strcmp ( name_register, spu_registers_for_asm[i] ) == 0 )
            {
            return i;
            }
        }
    return POISON;
    }

int Convert_Pop ( struct assembler* AsmStruct )                                                                     /////printf->->->
    {
    printf ( "Start Convert_Pop   -> " );

    while ( fgetc ( AsmStruct->code_for_asm ) == ' ' );
    fseek ( AsmStruct->code_for_asm, -1, SEEK_CUR );

    char name_register[2] = {0};
    if ( fscanf ( AsmStruct->code_for_asm, "%s" , name_register ) == 1 )
        {
        stack_type bin_name_register = Finding_Name_Register ( name_register );
        printf ( "pop %lld    -> ", bin_name_register );
        if ( bin_name_register == POISON )
            {
            printf ( RED( "FAILURE bin_name_register\n" ) );
            return FAILURE;
            }
        else
            {
            AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_POP__;
            AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = bin_name_register;
            printf ( "(total) %lld %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-2], AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
            return SUCCESSUFUL;
            }    
        }
     else
        {
        return FAILURE;
        }
    }
int Convert_Add ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Add   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_ADD__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Sub ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Sub   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_SUB__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Mul ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Mul   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_MUL__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Div ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Div   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_DIV__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Out ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Out   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_OUT__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_In ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_In    -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_IN___;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Sqrt ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Sqrt  -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_SQRT_;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Sin ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Sin   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_SIN__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Cos ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Cos   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_COS__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Dump ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Dump   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_DUMP_;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }

int Convert_Hlt ( struct assembler* AsmStruct )
    {
    printf ( "Start Convert_Hlt   -> " );
    AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu++] = CMD_HLT__;
    printf ( "(total) %lld\n", AsmStruct->data_bin_spu[AsmStruct->size_code_for_spu-1] );
    return SUCCESSUFUL;
    }