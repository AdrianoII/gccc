#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "lexParser.h"
#include "syntaxParser.h"
#include "semanticParser.h"

/*
 * arg[0] = nome do programa
 * arg[1] = path do arquivo de entrada
 * */

inputType input;

void init()
{
	initLexParser();

	initSemanticParser();

	initIntermediateCodeGeneration();
}

void terminate()
{
	terminateSemanticParser();
	terminateIntermediateCodeGeneration();
}

int main(int argc, char *argv[])
{
    if(argc >= 2)
    {
        input.entrada = fopen(argv[1],"r");
        if(input.entrada == NULL)
        {
            printf("Path do arquivo de entrada inválido tente novamente!\n");
            return 2;
        }
        printf("O path inserido para a entrada é %s\n",argv[1]);
        init();
		if(startAnalysis())
		{
			printf("Parabéns seu programa é válido!\n");
			for(int i = 0; i < argc; i++)
			{
				if(!strcmp(argv[i],"-st"))
				{
					printSymbolTable();
				}
				if(!strcmp(argv[i],"-c"))
				{
					printCode();
				}
				if(!strcmp(argv[i],"-cf"))
				{
					FILE *output = fopen(argv[++i],"w");
					if(input.entrada == NULL)
					{
						printf("Path de sáida inválido tente novamente!\n");
						return 2;
					}
					printCodeFile(output);
				}
			}
		}
        fclose(input.entrada);
		terminate();
    }
    else
    {
        printf("Número de argumentos inválidos, por favor tente novamente.\n");
        return 1;
    }
    return 0;
}
