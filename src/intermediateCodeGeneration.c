#include <stdio.h>
#include <stdlib.h>
#include "intermediateCodeGeneration.h"

char* threeAddressOpNames[4];
codeType code;

void initCode()
{
	code.code = calloc(CODE_INITIAL_SIZE,sizeof(char*));
	code.actualSize = 0;
	code.maxSize = CODE_INITIAL_SIZE;
}

void initThreeAddressOpNames()
{
//	threeAddressOpNames[0] = "THREE_ADDRESS_EMPTY";
//	threeAddressOpNames[1] = "THREE_ADDRESS_ASSIGNMENT";
//	threeAddressOpNames[2] = "THREE_ADDRESS_PLUS";
//	threeAddressOpNames[3] = "THREE_ADDRESS_JF";

	threeAddressOpNames[0] = "-";
	threeAddressOpNames[1] = ":=";
	threeAddressOpNames[2] = "+";
	threeAddressOpNames[3] = "JF";
}

int codeRealloc()
{
	if(code.actualSize == code.maxSize)
	{
		code.code = realloc(code.code, 2*code.maxSize);
		if(code.code == NULL)
		{
			printf(RED"Não foi possível alocar mais memória para a geração de código intermediário ¯\\_(ツ)_/¯"RESET);
			return 0;
		}
		code.maxSize *= 2;
	}
	return 1;
}
void generateCode(quadType quad)
{
	if(code.actualSize == code.maxSize)
	{
		if(!codeRealloc())
		{
			return;
		}
	}
	char* newCode = generateThreeAddresCode(quad);
	code.code[code.actualSize++] = newCode;
}
char* generateThreeAddresCode(quadType quad)
{
	char* result = calloc(32,sizeof(char));
	sprintf(result, "[%s %s %s %s]", threeAddressOpNames[quad.operator], (quad.operand1 == NULL ? "-" : quad.operand1->lexVal), (quad.operand2 == NULL ? "-" : quad.operand2->lexVal), (quad.result == NULL ? "-" : quad.result->lexVal));
	return result;
}

void patch(int hole,quadType fill)
{
	code.code[hole] = generateThreeAddresCode(fill);
}

void printCode()
{
	printf("Label [Operador Operando1 Operando2 Resultado]\n");
	for(int i = 0; i < code.actualSize; i++)
	{
		printf("%d : %s\n",i,code.code[i]);
	}
	printf("%d : [...]\n",code.actualSize);
}