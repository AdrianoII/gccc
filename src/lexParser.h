#ifndef COMPILADOR_LEXPARSER_H
#define COMPILADOR_LEXPARSER_H

#define consumeToken() consumed = 1

typedef enum{ERROR,IDLE,VAR,COLON,ID,COMMA,INTEGER,REAL,SEMICOLON,ASSIGNMENT,IF,THEN,PLUS} tokenClassification;
typedef enum{CLASS_ERROR,CLASS_IDLE,KEYWORD,SYMBOL,OPERATOR,IDENTIFIER,TYPE} tokenClass;

typedef struct tokenStruct
{
    char lexVal[256];
    int size;
    tokenClassification type;
	tokenClass class;
}tokenType;

extern int lines;
extern int col;
extern int consumed;
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
