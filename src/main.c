#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "lexParser.h"
#include "syntaxParser.h"
#include "semanticParser.h"

/*
 * arg[0] = nome do programa
 * arg[1] = path do arquivo de entrada
 * */

FILE *entrada;

void init()
{
	lines = 0;
	col = 0;
	consumed = 1;
	numTemp = 0;

	initTokenTypesNames();
	initTokenClassNames();
	initSemanticClassNames();
	initSemanticTypeNames();
	initThreeAddressOpNames();

	initSymbolTable();
	initAnalisysQueue();
	initCode();
}


int main(int argc, char *argv[])
{
    if(argc >= 2)
    {
        entrada = fopen(argv[1],"r");
        if(entrada == NULL)
        {
            printf("Path inválido tente novamente!\n");
            return 2;
        }
        printf("O path inserido é %s\n",argv[1]);
        init();
		if(Z())
		{
			printf("Parabéns seu programa é aceito pela gramática!\n");
			printCode();
			for(int i = 0; i < argc; i++)
			{
				if(!strcmp(argv[i],"-st"))
				{
					printSymbolTable();
				}
			}
		}
        fclose(entrada);
		for(int i = 0; i < code.actualSize; i++)
		{
			free(code.code[i]);
		}
    	free(code.code);
    	free(st.entries);
    }
    else
    {
        printf("Número de argumentos inválidos, por favor tente novamente.\n");
        return 1;
    }
    return 0;
}
