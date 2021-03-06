#include "syntaxParser.h"
#include <stdio.h>

int Z();
int I();
int D();
int L();
int X();
int K();
int O();
int S();
int E(nonTerminalType *e);
int R(nonTerminalType *r);
int T(nonTerminalType *t);

void syntaxError(tokenClassification expected)
{
	printf(RED "ERRO SINTÁTICO: (%d:%d) obtido %s(%s) esperado %s\n" RESET,input.line,input.col,token.lexVal,tokenTypesNames[token.type],tokenTypesNames[expected]);
}

int startAnalysis()
{
	return Z();
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
	getNextToken(input.entrada);
	if(token.type == VAR)
	{
		token.isConsumed =  1;
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
		getNextToken(input.entrada);
		if(token.type == COLON)
		{
			token.isConsumed =  1;
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
			syntaxError(COMMA);
			printf("OR\n");
			syntaxError(COLON);
		}
	}
	return 0;
}

int L()
{
	getNextToken(input.entrada);
	if(token.type == ID)
	{
		token.isConsumed =  1;
		symbolTableItem* idRef = addSymbolTable();
		if(idRef != NULL)
		{
			addAnalysisQueue(idRef);
			if(X())
			{
				return 1;
			}
		} else
		{
			printf(RED"ERRO SEMÂNTICO: (%d:%d) A VÁRIAVEL %s JÁ FOI DECLARADA ANTERIORMENTE!\n"RESET,input.line,input.col,token.lexVal);
		}
	} else
	{
		syntaxError(ID);
	}
	return 0;
}

int X()
{
	getNextToken(input.entrada);
	if(token.type == COMMA)
	{
		token.isConsumed =  1;
		if(L())
		{
			return 1;
		}
		else
		{
			return  0;
		}
	}
	return 1;
}

int K()
{
	getNextToken(input.entrada);
	if(token.type == INTEGER || token.type == REAL)
	{
		token.isConsumed =  1;
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
	getNextToken(input.entrada);
	if(token.type == SEMICOLON)
	{
		token.isConsumed =  1;
		if(D())
		{
			return 1;
		} else
		{
			return 0;
		}
	}
	return 1;
}

int S()
{
	getNextToken(input.entrada);
	nonTerminalType e;
	e.dir = NULL;
	e.esq = NULL;
	e.quadAddress = -1;
	quadType quad;
	if(token.type == ID)
	{
		token.isConsumed =  1;
		symbolTableItem* idRef = lookup(token.lexVal, SEMANTIC_CLASS_VAR);
		if(idRef != NULL)
		{
			addAnalysisQueue(idRef);
			getNextToken(input.entrada);
			if(token.type == ASSIGNMENT)
			{
				e.esq = idRef;
				token.isConsumed =  1;
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
			printf(RED"ERRO SEMÂNTICO: (%d:%d) A VÁRIAVEL %s NÃO FOI DECLARADA!\n"RESET,input.line,input.col,token.lexVal);
		}
	}
	else if(token.type == IF)
	{
		token.isConsumed =  1;
		if(E(&e))
		{
			getNextToken(input.entrada);
			if(token.type == THEN)
			{
				token.isConsumed =  1;
				nonTerminalType s1;
				s1.esq = NULL;
				s1.dir = NULL;
				s1.quadAddress = generateEmptyCodeLine();
				if(S())
				{
					quad.operator = THREE_ADDRESS_JF;
					quad.result = NULL;
					quad.operand1 = e.dir;
					quad.operand2 = addSymbolTableTag(intermediateCode.actualSize);
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
		syntaxError(SEMICOLON);
		printf("OR\n");
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
	int isIf = token.type == IF;
	if(T(&t))
	{
		nonTerminalType r;
		r.esq = t.dir;
		r.dir = NULL;
		r.quadAddress = -1;
		if(R(&r))
		{
			if(getNextToken())
			{
				if(token.type != THEN && token.type != IDLE)
				{
					if(isIf)
					{
						syntaxError(THEN);
					} else
					{
						syntaxError(IDLE);
					}
					//printf("OR\n");
					return 0;
				}
			}
			(*e).dir = r.dir;
			return 1;
		}
	}
	return 0;
}

int T(nonTerminalType *t)
{
	getNextToken(input.entrada);
	if(token.type == ID)
	{
		token.isConsumed =  1;
		symbolTableItem* idRef = lookup(token.lexVal, SEMANTIC_CLASS_VAR);
		if(idRef != NULL)
		{
			addAnalysisQueue(idRef);
			(*t).dir = idRef;
			return  1;
		}
		else
		{
			printf(RED"ERRO SEMÂNTICO: (%d:%d) A VÁRIAVEL %s NÃO FOI DECLARADA!\n"RESET,input.line,input.col,token.lexVal);
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
	getNextToken(input.entrada);
	if(token.type == PLUS)
	{
		nonTerminalType t;
		t.esq = NULL;
		t.dir = NULL;
		t.quadAddress = -1;
		token.isConsumed =  1;
		if(T(&t))
		{
			r1.esq = t.dir;
			if(R(&r1))
			{
				(*r).dir = addTempSymbolTable((*r).esq->type < r1.dir->type ? (*r).esq->type : r1.dir->type);
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
	if(doTypeCheck())
	{
		(*r).dir = (*r).esq;
		return 1;
	}
	return 0;
}
