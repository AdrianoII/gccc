#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intermediateCodeGeneration.h"

char* threeAddressOpNames[4];
codeType intermediateCode;

void initCode()
{
	intermediateCode.head = NULL;
	intermediateCode.actualSize = 0;
}

void initThreeAddressOpNames()
{
	threeAddressOpNames[0] = "-";
	threeAddressOpNames[1] = ":=";
	threeAddressOpNames[2] = "+";
	threeAddressOpNames[3] = "JF";
}

void insertCode(char* codeLine, int pos)
{
	codeLineType *aux = intermediateCode.head;
	if(aux != NULL)
	{
		while(aux->line != pos && aux->prox != NULL)
		{
				aux = aux->prox;
		}
		if(aux->line == pos)
		{
			strcpy(aux->code,codeLine);
		}
		else
		{
			codeLineType *new = calloc(1, sizeof(codeLineType));
			strcpy(new->code,codeLine);
			new->line = pos;
			new->prox = NULL;
			new->prox = aux->prox;
			aux->prox = new;
		}
	}
	else
	{
		codeLineType *new = calloc(1, sizeof(codeLineType));
		strcpy(new->code,codeLine);
		new->line = pos;
		new->prox = NULL;
		intermediateCode.head = new;
	};
	free(codeLine);
}

void generateCode(quadType quad)
{
	char* newCode = generateThreeAddresCode(quad);
	insertCode(newCode, intermediateCode.actualSize++);
}
char* generateThreeAddresCode(quadType quad)
{
	char* result = calloc(256,sizeof(char));
	sprintf(result, "[%s %s %s %s]", threeAddressOpNames[quad.operator], (quad.operand1 == NULL ? "-" : quad.operand1->val), (quad.operand2 == NULL ? "-" : quad.operand2->val), (quad.result == NULL ? "-" : quad.result->val));
	return result;
}

void patch(int hole,quadType fill)
{
	insertCode(generateThreeAddresCode(fill),hole);
}

void printCode()
{
	printf("Label [Operador Operando1 Operando2 Resultado]\n");
	for(codeLineType* i = intermediateCode.head; i != NULL; i = i->prox)
	{
		printf("%d : %s\n",i->line,i->code);
	}
	printf("%d : [...]\n", intermediateCode.actualSize);
}

int generateEmptyCodeLine()
{
	quadType quad;
	quad.operator = THREE_ADDRESS_EMPTY;
	quad.operand1 = NULL;
	quad.operand2 = NULL;
	quad.result =  NULL;
	int pos = intermediateCode.actualSize;
	generateCode(quad);
	return pos;
}
void initIntermediateCodeGeneration()
{
	initThreeAddressOpNames();
	initCode();
}

void terminateCode()
{
	codeLineType *atual = intermediateCode.head;
	if(atual != NULL)
	{
		codeLineType *prox = intermediateCode.head->prox;
		if(prox != NULL)
		{
			while(prox != NULL)
			{
				free(atual);
				atual = prox;
				prox = prox->prox;
			}
		}
	}
	free(atual);
}

void terminateIntermediateCodeGeneration()
{
	terminateCode();
}
void printCodeFile(FILE *out)
{
	fprintf(out,"Label [Operador Operando1 Operando2 Resultado]\n");
	for(codeLineType* i = intermediateCode.head; i != NULL; i = i->prox)
	{
		fprintf(out,"%d : %s\n",i->line,i->code);
	}
	fprintf(out, "%d : [...]\n", intermediateCode.actualSize);
}