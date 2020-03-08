#include "syntaxParser.h"
#include <stdio.h>

void syntaxError(tokenClassification expected)
{
	printf(RED "%d:%d obtained %s(%s) expected %s\n" RESET,lines,col,token.lexVal,tokenTypesNames[token.type],tokenTypesNames[expected]);
}

int Z()
{
	if(I())
	{
		if(S())
		{
			return 1;
		}
	}
	return 0;
}
int I()
{
	getNextToken(entrada);
	if(token.type == VAR)
	{
		consumeToken();
		if(D())
		{
			return 1;
		}
	}
	else
	{
		syntaxError(VAR);
	}
	return 0;
}

int D()
{
	if(L())
	{
		getNextToken(entrada);
		if(token.type == COLON)
		{
			consumeToken();
			if(K())
			{
				if(O())
				{
					return 1;
				}
			}
		}
		else
		{
			syntaxError(COLON);
		}
	}
	return 0;
}

int L()
{
	getNextToken(entrada);
	if(token.type == ID)
	{
		consumeToken();
		if(X())
		{
			return 1;
		}
	} else
	{
		syntaxError(ID);
	}
	return 0;
}

int X()
{
	getNextToken(entrada);
	if(token.type == COMMA)
	{
		consumeToken();
		if(L())
		{
			return 1;
		}
		else
		{
			return  0;
		}
	}
	//falta o erro de esperar o COMMA
	return 1;
}

int K()
{
	getNextToken(entrada);
	if(token.type == INTEGER || token.type == REAL)
	{
		consumeToken();
		return 1;
	}
	else
	{
		syntaxError(INTEGER);
		printf("OR\n");
		syntaxError(REAL);
	}
	return  0;
}

int O()
{
	getNextToken(entrada);
	if(token.type == SEMICOLON)
	{
		consumeToken();
		if(D())
		{
			return 1;
		} else
		{
			return 0;
		}
	}
	//Outro caso do vazio
	return 1;
}

int S()
{
	getNextToken(entrada);
	if(token.type == ID)
	{
		consumeToken();
		getNextToken(entrada);
		if(token.type == ASSIGNMENT)
		{
			consumeToken();
			if(E())
			{
				return 1;
			}
		}
		else
		{
			syntaxError(ASSIGNMENT);
		}
	}
	else if(token.type == IF)
	{
		consumeToken();
		if(E())
		{
			getNextToken(entrada);
			if(token.type == THEN)
			{
				consumeToken();
				if(S())
				{
					return 1;
				}
			}
			else
			{
				syntaxError(THEN);
			}
		}
	} else
	{
		syntaxError(ID);
		printf("OR\n");
		syntaxError(IF);
	}
	return 0;
}

int E()
{
	if(T())
	{
		if(R())
		{
			return 1;
		}
	}
	return 0;
}

int T()
{
	getNextToken(entrada);
	if(token.type == ID)
	{
		consumeToken();
		return  1;
	}
	else
	{
		syntaxError(ID);
	}
	return 0;
}

int R()
{
	getNextToken(entrada);
	if(token.type == PLUS)
	{
		consumeToken();
		if(T())
		{
			if(R())
			{
				return 1;
			}
		}
		return 0;
	}
	//Vazio...
	return 1;
}
