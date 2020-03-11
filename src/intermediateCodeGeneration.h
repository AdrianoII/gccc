#ifndef COMPILADOR_INTERMEDIATECODEGENERATION_H
#define COMPILADOR_INTERMEDIATECODEGENERATION_H

#include "semanticParser.h"

#define CODE_SIZE 256

typedef enum{THREE_ADDRESS_EMPTY,THREE_ADDRESS_ASSIGNMENT,THREE_ADDRESS_PLUS,THREE_ADDRESS_JF}threeAddressOp;

typedef struct
{
	threeAddressOp operator;
	symbolTableItem* operand1;
	symbolTableItem* operand2;
	symbolTableItem* result;
}quadType;

typedef struct codeLineStruct
{
	char code[CODE_SIZE];
	int line;
	struct codeLineStruct* prox;
} codeLineType;

typedef struct
{
	codeLineType* head;
	int actualSize;
}codeType;

typedef struct
{
	symbolTableItem* esq;
	symbolTableItem* dir;
	int quadAddress;
}nonTerminalType;

extern char* threeAddressOpNames[4];
extern codeType intermediateCode;

void initCode();
void initThreeAddressOpNames();
int generateEmptyCodeLine();
void generateCode(quadType quad);
char* generateThreeAddresCode(quadType quad);
void patch(int hole,quadType fill);
void printCode();
void initIntermediateCodeGeneration();
void terminateIntermediateCodeGeneration();
void printCodeFile(FILE *out);

#endif //COMPILADOR_INTERMEDIATECODEGENERATION_H
