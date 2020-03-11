#ifndef COMPILADOR_LEXPARSER_H
#define COMPILADOR_LEXPARSER_H

#include "main.h"

#define LEX_VAL_SIZE 64

typedef enum{ERROR,IDLE,VAR,COLON,ID,COMMA,INTEGER,REAL,SEMICOLON,ASSIGNMENT,IF,THEN,PLUS} tokenClassification;
typedef enum{CLASS_ERROR,CLASS_IDLE,KEYWORD,SYMBOL,OPERATOR,IDENTIFIER,TYPE} tokenClass;

typedef struct tokenStruct
{
    char lexVal[LEX_VAL_SIZE];
    int size;
    tokenClassification type;
	tokenClass class;
	int isConsumed;
}tokenType;

extern int lines;
extern int col;
extern tokenType token;
extern char* tokenTypesNames[13];
extern char* tokenClassNames[13];


void initTokenTypesNames();
void initTokenClassNames();
void printToken();
char* getTokenClassificationString(tokenClassification tc);
int isIdentifier();
void tokenInit();
int getNextToken();
#endif //COMPILADOR_LEXPARSER_H
