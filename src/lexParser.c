#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexParser.h"

#ifndef isAlpha
#define isAlpha(c) ((c > 64 && c < 91) || (  c > 96 && c < 122))
#endif //isAlpha

#ifndef isNum
#define isNum(c) (c > 47 && c < 58)
#endif //isNum

#ifndef isAlphaNum
#define isAlphaNum(c) isAlpha(c) || isNum(c)
#endif //isAlphaNum

tokenType token;

int lines;
int col;
char* tokenTypesNames[13];

void tokenInit()
{
    memset(token.lexVal, '\0', 64);
    token.size = 0;
    token.type = IDLE;
    token.isConsumed = 0;
}


void printToken()
{
    printf("Token{\n\tlexval: \"%s\",\n\tsize: %d,\n\ttype: %s\n\tisConsumed: %d\n}\n",token.lexVal,token.size,tokenTypesNames[token.type],token.isConsumed);
}

void initTokenTypesNames()
{
	tokenTypesNames[0] = "lexical error";
	tokenTypesNames[1] = "idle";
	tokenTypesNames[2] = "var";
	tokenTypesNames[3] = "colon";
	tokenTypesNames[4] = "id";
	tokenTypesNames[5] = "comma";
	tokenTypesNames[6] = "integer";
	tokenTypesNames[7] = "real";
	tokenTypesNames[8] = "semicolon";
	tokenTypesNames[9] = "assignment";
	tokenTypesNames[10] = "if";
	tokenTypesNames[11] = "then";
	tokenTypesNames[12] = "plus";
}

int isIdentifier()
{
    if(isAlpha(token.lexVal[0]) && token.size > 0)
    {
        if(token.size > 1)
        {
            for (int i = 1; i < token.size; i++)
            {
                if(!(isAlphaNum(token.lexVal[i])))
                {
                    return 0;
                }
            }
        }
        return 1;
    }
    return 0;
}
void classifyToken()
{
    if(!strcmp(token.lexVal,"var"))
    {
        token.type = VAR;
    }
    else if(!strcmp(token.lexVal,":"))
    {
        token.type = COLON;
    }
    else if(!strcmp(token.lexVal,","))
    {
        token.type = COMMA;
    }
    else if(!strcmp(token.lexVal,"integer"))
    {
        token.type = INTEGER;
    }
    else if(!strcmp(token.lexVal,"real"))
    {
        token.type = REAL;
    }
    else if(!strcmp(token.lexVal,";"))
    {
        token.type = SEMICOLON;
    }
    else if(!strcmp(token.lexVal,":="))
    {
        token.type = ASSIGNMENT;
    }
    else if(!strcmp(token.lexVal,"if"))
    {
        token.type = IF;
    }
    else if(!strcmp(token.lexVal,"then"))
    {
        token.type = THEN;
    }
    else if(!strcmp(token.lexVal,"+"))
    {
        token.type = PLUS;
    }
    else if(isIdentifier())
    {
        token.type = ID;
    }else
	{
    	token.type = ERROR;
    }
}

int tryParseToken(char c)
{
    int parsed = 0;
    //Verifica se tem algo para classificar antes do novo caracter
    if(c == ',' || c == ';' || c == ':' || c == ' ' || c == '+' || c == '\t' || c == '\n')
    {
        if(token.size > 0)
        {
            classifyToken();
            parsed = 1;
        }
    }
	else if(token.size == 1 && (token.lexVal[0] == ':' || token.lexVal[0] == ',' || token.lexVal[0] == ';' || token.lexVal[0] == '+') && c != '=')
	{
		classifyToken();
		parsed = 1;
	} else if (token.lexVal[0] == ':' && token.lexVal[1] == '=')
	{
		classifyToken();
		parsed = 1;
	}

    if( c != ' ' && c != '\t' && c != '\n' && !parsed)
    {
        token.lexVal[token.size++] = c;
    }
    if (c == '\n' && !parsed)
    {
        ++lines;
        col = 0;
    }
    return parsed;
}


int getNextToken(FILE *input)
{
	if(token.isConsumed)
	{
		char c = 0;
		tokenInit();
		while ((c = fgetc(input)) != EOF) {
			++col;
			if (lines == 0) {
				lines = 1;
			}
			if (tryParseToken(c)) {
				fseek(input, -1, SEEK_CUR);
				--col;
				if(token.type == ERROR)
				{
					printf(RED"ERRO LÉXICO: (%d:%d) %s NÃO É RECONHECIDO.\n"RESET, lines, col, token.lexVal);
				}
				break;
			}
		}
		if (c == EOF) {
			if (token.size > 0) {
				classifyToken();
			}
			return 0;
		}
	}
    return  1;
}
