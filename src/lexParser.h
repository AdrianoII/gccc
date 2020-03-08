#ifndef COMPILADOR_LEXPARSER_H
#define COMPILADOR_LEXPARSER_H

#define consumeToken() consumed = 1

typedef enum{ERROR,IDLE,VAR,COLON,ID,COMMA,INTEGER,REAL,SEMICOLON,ASSIGNMENT,IF,THEN,PLUS} tokenClassification;

typedef struct tokenStruct
{
    char lexVal[256];
    int size;
    tokenClassification type;
}tokenType;

extern int lines;
extern int col;
extern int consumed;
extern tokenType token;
extern char* tokenTypesNames[13];

void printToken();
char* getTokenClassificationString(tokenClassification tc);
int isIdentifier();
void tokenInit();
int getNextToken();
#endif //COMPILADOR_LEXPARSER_H
