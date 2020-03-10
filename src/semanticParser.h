#ifndef COMPILADOR_SEMANTICPARSER_H
#define COMPILADOR_SEMANTICPARSER_H

#include "lexParser.h"
#include "main.h"

#define ST_INITIAL_SIZE 300

typedef enum {SEMANTIC_CLASS_EMPTY,SEMANTIC_CLASS_VAR,SEMANTIC_CLASS_TEMP,SEMANTIC_CLASS_TAG} semanticClass;
typedef enum {SEMANTIC_TYPE_EMPTY,SEMANTIC_TYPE_INTEGER,SEMANTIC_TYPE_REAL,SEMANTIC_TYPE_ADDRESS} semanticType;

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
extern char* semanticClassNames[4];
extern char* semanticTypeNames[4];
extern int numTemp;

void initSemanticClassNames();
void initSemanticTypeNames();
void initSymbolTable();
void initAnalisysQueue();
void printSymbolTable();
void printAnalisysQueue();
int reallocSymbolTable();
symbolTableItem* addSymbolTable();
symbolTableItem* addSymbolTableTag(int tagVal);
symbolTableItem* addTempSymbolTable(semanticType type);
void addTypeRange();
symbolTableItem* lookup(char* lexVal);
void addAnalisysQueue(symbolTableItem *item);
void cleanAnalisysQueue();
int doTypeCoercion();

#endif //COMPILADOR_SEMANTICPARSER_H
