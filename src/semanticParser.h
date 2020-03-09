//
// Created by adrianoii on 08/03/2020.
//

#ifndef COMPILADOR_SEMANTICPARSER_H
#define COMPILADOR_SEMANTICPARSER_H

#include "lexParser.h"
#include "main.h"

#define TS_INITIAL_SIZE 100

typedef enum {TARGET,OPERAND} analisysType;
typedef enum {SEMANTIC_CLASS_EMPTY,SEMANTIC_CLASS_VAR} semanticClass;
typedef enum {SEMANTIC_TYPE_EMPTY,SEMANTIC_TYPE_INTEGER,SEMANTIC_TYPE_REAL} semanticType;

typedef struct
{
	char lexVal[256];
	int lexValSize;
	tokenClassification tokenType;
	tokenClass tokenClass;
	semanticClass class;
	semanticType type;
}symbolTableItem;

typedef struct
{
	symbolTableItem *entries;
	int acutalSize;
	int maxSize;
}symbolTable;

typedef struct analisysQueueItem
{
	symbolTableItem *item;
	struct analisysQueueItem *prox;
} analisysQueueItem;

typedef struct
{
	analisysQueueItem *head;
	int size;
} analisysQueue;

extern symbolTable st;
extern analisysQueue aq;
extern char* semanticClassNames[2];
extern char* semanticTypeNames[3];
extern char* analisysTypeNames[2];

void initSemanticClassNames();
void initSemanticTypeNames();
void initAnalisysTypeNames();
void initSymbolTable();
void initAnalisysQueue();
void printSymbolTable();
void printAnalisysQueue();
int reallocSymbolTable();
symbolTableItem* addSymbolTable();
void addTypeRange();
symbolTableItem* lookup(char* lexVal);
void addAnalisysQueue(symbolTableItem *item);
void cleanAnalisysQueue();
int doTypeCoercion();

#endif //COMPILADOR_SEMANTICPARSER_H
