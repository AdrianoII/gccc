#include "syntaxParser.h"
#include <stdio.h>

void syntaxError(tokenClassification expected)
{
	printf(RED "ERRO SINTÁTICO: (%d:%d) obtido %s(%s) esperado %s\n" RESET,lines,col,token.lexVal,tokenTypesNames[token.type],tokenTypesNames[expected]);
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
		symbolTableItem* idRef = addSymbolTable();
		if(idRef != NULL)
		{
			addAnalisysQueue(idRef);
			//printSymbolTable();
			//printAnalisysQueue();
			if(X())
			{
				return 1;
			}
		} else
		{
			printf(RED"ERRO SEMÂNTICO: (%d:%d) A VÁRIAVEL %s JÁ FOI DECLARADA ANTERIORMENTE!\n"RESET,lines,col,token.lexVal);
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
		addTypeRange();
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
	nonTerminalType e;
	e.dir = NULL;
	e.esq = NULL;
	e.quadAddress = -1;
	quadType quad;
	if(token.type == ID)
	{
		consumeToken();
		symbolTableItem* idRef = lookup(token.lexVal);
		if(idRef != NULL)
		{
			addAnalisysQueue(idRef);
			getNextToken(entrada);
			if(token.type == ASSIGNMENT)
			{
				e.esq = idRef;
				consumeToken();
				if(E(&e))
				{
					quad.operator = THREE_ADDRESS_ASSIGNMENT;
					quad.result = e.esq;
					quad.operand1 = e.dir;
					quad.operand2 = NULL;
					generateCode(quad);
						return 1;
				}
			}
			else
			{
				syntaxError(ASSIGNMENT);
			}
		} else
		{
			printf(RED"ERRO SEMÂNTICO: (%d:%d) A VÁRIAVEL %s NÃO FOI DECLARADA!\n"RESET,lines,col,token.lexVal);
		}
	}
	else if(token.type == IF)
	{
		consumeToken();
		if(E(&e))
		{
			getNextToken(entrada);
			if(token.type == THEN)
			{
				consumeToken();
				nonTerminalType s1;
				s1.esq = NULL;
				s1.dir = NULL;
				s1.quadAddress = code.actualSize++;
				if(S())
				{
					quad.operator = THREE_ADDRESS_JF;
					quad.result = NULL;
					quad.operand1 = e.dir;
					quad.operand2 = addSymbolTableTag(code.actualSize);
					patch(s1.quadAddress,quad);
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

int E(nonTerminalType *e)
{
	nonTerminalType t;
	t.esq = NULL;
	t.dir = NULL;
	t.quadAddress = -1;
	if(T(&t))
	{
		nonTerminalType r;
		r.esq = t.dir;
		r.dir = NULL;
		r.quadAddress = -1;
		if(R(&r))
		{
			(*e).dir = r.dir;
			return 1;
		}
	}
	return 0;
}

int T(nonTerminalType *t)
{
	getNextToken(entrada);
	if(token.type == ID)
	{
		consumeToken();
		symbolTableItem* idRef = lookup(token.lexVal);
		if(idRef != NULL)
		{
			addAnalisysQueue(idRef);
			(*t).dir = idRef;
			return  1;
		}
		else
		{
			printf(RED"ERRO SEMÂNTICO: (%d:%d) A VÁRIAVEL %s NÃO FOI DECLARADA!\n"RESET,lines,col,token.lexVal);
		}
	}
	else
	{
		syntaxError(ID);
	}
	return 0;
}

int R(nonTerminalType *r)
{
	nonTerminalType r1;
	r1.esq = NULL;
	r1.dir = NULL;
	r1.quadAddress = -1;
	getNextToken(entrada);
	if(token.type == PLUS)
	{
		nonTerminalType t;
		t.esq = NULL;
		t.dir = NULL;
		t.quadAddress = -1;
		consumeToken();
		if(T(&t))
		{
			r1.esq = t.dir;
			if(R(&r1))
			{
				(*r).dir = addTempSymbolTable();
				quadType quad;
				quad.operator = THREE_ADDRESS_PLUS;
				quad.result = (*r).dir;
				quad.operand1 = (*r).esq;
				quad.operand2 = r1.dir;
				generateCode(quad);
				return 1;
			}
		}
		return 0;
	}
	int action = doTypeCoercion();
	if(action < 0)
	{
		printf(RED"ERRO SEMÂNTICO: (%d:%d) EXPRESSÂO NÂO POSSUI ELEMENTOS!\n"RESET,lines,col);
	}
	else if(action > 0)
	{
		if(action == 1)
		{
			printf(YELLOW"AÇÃO SEMÂNTICA: (%d:%d) PRECISA FAZER COERÇÂO PARA INTEGER\n"RESET,lines,col);
		} else
		{
			printf(YELLOW"AÇÃO SEMÂNTICA: (%d:%d) PRECISA FAZER COERÇÂO PARA FLOAT\n"RESET,lines,col);
		}
	}
	else
	{
		printf(GREEN"ANÁLISE SEMÂNTICA: (%d:%d) TIPOS DE OPERANDOS IGUAIS\n"RESET,lines,col);
	}
	(*r).dir = (*r).esq;
	//Vazio...
	return 1;
}
