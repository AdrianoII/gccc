#ifndef COMPILADOR_SEMANTICPARSER_H
#define COMPILADOR_SEMANTICPARSER_H

#include "lexParser.h"
#include "main.h"

#define ST_VAL_SIZE 256
#define ST_INITIAL_SIZE 300

typedef enum {SEMANTIC_CLASS_EMPTY,SEMANTIC_CLASS_VAR,SEMANTIC_CLASS_TEMP,SEMANTIC_CLASS_TAG} semanticClass;
typedef enum {SEMANTIC_TYPE_EMPTY,SEMANTIC_TYPE_INTEGER,SEMANTIC_TYPE_REAL,SEMANTIC_TYPE_ADDRESS} semanticType;

typedef struct
{
	char val[ST_VAL_SIZE];
	int valSize;
	tokenClassification tokenType;
	semanticClass class;
	semanticType type;
}symbolTableItem;

typedef struct
{
	symbolTableItem *entries;
	int actualSize;
	int maxSize;
	int numTemp;
}symbolTable;

typedef struct analysisQueueItem
{
	symbolTableItem *item;
	struct analysisQueueItem *prox;
} analysisQueueItem;

typedef struct
{
	analysisQueueItem *head;
	int size;
} analysisQueue;

extern symbolTable st;
extern analysisQueue aq;
extern char* semanticClassNames[4];
extern char* semanticTypeNames[4];

void initSemanticParser();
void printSymbolTable();
void printAnalysisQueue();
int reallocSymbolTable();
symbolTableItem* addSymbolTable();
symbolTableItem* addSymbolTableTag(int tagVal);
symbolTableItem* addTempSymbolTable(semanticType type);
void addTypeRange();
symbolTableItem* lookup(char* lexVal);
void addAnalysisQueue(symbolTableItem *item);
int doTypeCoercion();

#endif //COMPILADOR_SEMANTICPARSER_H
