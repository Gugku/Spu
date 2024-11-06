#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "EnumColors.h"
#include "Stack.h"
#include "Asm.h"

int main ()
    {
    /*FILE* CodeForAsm = fopen ( "CodeForSpu.txt", "r" );
    if (  CodeForAsm == NULL )
        {
        perror ( RED( "ERROR open file\n" ) );
        return 1;
        }



    if ( fclose( CodeForAsm ) != 0 )
        {
        perror ( RED( "ERROR close file\n" ) );
        return 1;
        }*/
    size_t len = 7;
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
    }

///int Asm_Run ()
///int Asm_Switch ()
