#ifndef COMPILADOR_LEXPARSER_H
#define COMPILADOR_LEXPARSER_H

#include "main.h"

#define LEX_VAL_SIZE 64

typedef enum{ERROR,IDLE,VAR,COLON,ID,COMMA,INTEGER,REAL,SEMICOLON,ASSIGNMENT,IF,THEN,PLUS} tokenClassification;

typedef struct tokenStruct
{
    char lexVal[LEX_VAL_SIZE];
    int size;
    tokenClassification type;
	int isConsumed;
}tokenType;

extern int lines;
extern int col;
extern tokenType token;
extern char* tokenTypesNames[13];


void initLexParser();
void printToken();
int getNextToken();

#endif //COMPILADOR_LEXPARSER_H
