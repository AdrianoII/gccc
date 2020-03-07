#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "lexParser.h"

/*
 * arg[0] = nome do programa
 * arg[1] = path do arquivo de entrada
 * */
void init()
{
	lines = 0;
	tokenTypesNames[0] = "error";
	tokenTypesNames[1] = "idle";
	tokenTypesNames[2] = "var";
	tokenTypesNames[3] = "colon";
	tokenTypesNames[4] = "id";
	tokenTypesNames[5] = "comma";
	tokenTypesNames[6] = "integer";
	tokenTypesNames[7] = "real";
	tokenTypesNames[8] = "semicolon";
	tokenTypesNames[9] = "assignment";
	tokenTypesNames[10] = "if";
	tokenTypesNames[11] = "then";
	tokenTypesNames[12] = "plus";
}


int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        FILE *entrada = fopen(argv[1],"r");
        if(entrada == NULL)
        {
            printf("Path inválido tente novamente!\n");
            return 2;
        }
        init();
        while(getNextToken(entrada))
        {
            printToken();
        }
        printf("Lines = %d\n",lines);
        fclose(entrada);
    }
    else
    {
        printf("Número de argumentos inválidos, por favor tente novamente.\n");
        return 1;
    }
    return 0;
}
