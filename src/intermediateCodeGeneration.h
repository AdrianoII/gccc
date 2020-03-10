#ifndef COMPILADOR_INTERMEDIATECODEGENERATION_H
#define COMPILADOR_INTERMEDIATECODEGENERATION_H

#include "semanticParser.h"

#define CODE_INITIAL_SIZE 100

typedef enum{THREE_ADDRESS_EMPTY,THREE_ADDRESS_ASSIGNMENT,THREE_ADDRESS_PLUS,THREE_ADDRESS_JF}threeAddressOp;

typedef struct
{
	threeAddressOp operator;
	symbolTableItem* operand1;
	symbolTableItem* operand2;
	symbolTableItem* result;
}quadType;

typedef struct
{
	char** code;
	int actualSize;
	int maxSize;
}codeType;

typedef struct
{
	symbolTableItem* esq;
	symbolTableItem* dir;
	int quadAddress;
}nonTerminalType;

extern char* threeAddressOpNames[4];
extern codeType code;

void initCode();
void initThreeAddressOpNames();
int codeRealloc();
void generateCode(quadType quad);
char* generateThreeAddresCode(quadType quad);
void patch(int hole,quadType fill);
void printCode();

#endif //COMPILADOR_INTERMEDIATECODEGENERATION_H
