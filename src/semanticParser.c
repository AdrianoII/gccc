#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semanticParser.h"

symbolTable st;
analysisQueue aq;
char* semanticClassNames[4];
char* semanticTypeNames[4];

void initSymbolTable()
{
	st.actualSize = 0;
	st.numTemp = 0;
	st.head = NULL;
}

void initSemanticClassNames()
{
	semanticClassNames[0] = "SEMANTIC_CLASS_EMPTY";
	semanticClassNames[1] = "SEMANTIC_CLASS_VAR";
	semanticClassNames[2] = "SEMANTIC_CLASS_TEMP";
	semanticClassNames[3] = "SEMANTIC_CLASS_TAG";
}

void initSemanticTypeNames()
{
	semanticTypeNames[0] = "SEMANTIC_TYPE_EMPTY";
	semanticTypeNames[1] = "SEMANTIC_TYPE_INTEGER";
	semanticTypeNames[2] = "SEMANTIC_TYPE_REAL";
	semanticTypeNames[3] = "SEMANTIC_TYPE_ADDRESS";
}

void printSymbolTable()
{
	printf("symbolTable{\n\tactualSize: %d,\n\tnumTemp: %d,\n\tentries: [",st.actualSize,st.numTemp);
	if(st.actualSize > 0)
	{
		for (symbolTableItem *i = st.head; i != NULL; i = i->prox)
		{
			printf("\n\t {\n\t\tlexVal: %s,\n\t\tlexValSize: %d,\n\t\ttokenType: %s,\n\t\tclass: %s,\n\t\ttype: %s,\n\t }",
				   i->val, i->valSize,
				   tokenTypesNames[i->tokenType], semanticClassNames[i->class],
				   semanticTypeNames[i->type]);
		}
	}
	printf("\n\t]\n}\n");
}

void printAnalysisQueue()
{
	printf("analysisQueue{\n\tsize: %d,\n\thead: [",aq.size);
	if(aq.size > 0)
	{
		for (analysisQueueItem *aux = aq.head; aux != NULL; aux = aux->prox)
		{
			printf("\n\t {\n\t\tlexVal: %s,\n\t\tlexValSize: %d,\n\t\ttokenType: %s,\n\t\tclass: %s,\n\t\ttype: %s,\n\t }",
				   aux->item->val, aux->item->valSize, tokenTypesNames[aux->item->tokenType],
				   semanticClassNames[aux->item->tokenType], semanticTypeNames[aux->item->class]);
		}
	}
	printf("\n\t]\n}\n");
}

symbolTableItem* addSymbolTable()
{
	symbolTableItem *new = NULL;
	if(lookup(token.lexVal, SEMANTIC_CLASS_VAR) == NULL)
	{
		new = calloc(1,sizeof(symbolTableItem));
		new->tokenType = token.type;
		strcpy(new->val, token.lexVal);
		new->valSize = token.size;
		new->class = SEMANTIC_CLASS_VAR;
		new->type = SEMANTIC_TYPE_EMPTY;
		if(st.head == NULL)
		{
			st.head = new;
		} else
		{
			symbolTableItem *aux = NULL;
			for(aux = st.head; aux->prox != NULL; aux = aux->prox);
			aux->prox = new;
		}
		++st.actualSize;
	}
	return new;
}

void initAnalysisQueue()
{
	aq.size = 0;
	aq.head = NULL;
}

void cleanAnalysisQueue()
{
	if(aq.head != NULL)
	{
		analysisQueueItem *atual = aq.head;
		analysisQueueItem *prox = aq.head->prox;
		if(prox != NULL)
		{
			while(prox != NULL)
			{
				free(atual);
				atual = prox;
				prox = prox->prox;
			}
		}
		free(atual);
		initAnalysisQueue();
	}
}

void addTypeRange()
{
	semanticType type = token.type == INTEGER ? SEMANTIC_TYPE_INTEGER : SEMANTIC_TYPE_REAL;
	for(analysisQueueItem* actual = aq.head; actual != NULL; actual = actual->prox)
	{
		actual->item->type = type;
	}
	cleanAnalysisQueue();
}

symbolTableItem* lookup(char *lexVal,semanticClass class)
{
	symbolTableItem* result = NULL;
	if(st.actualSize > 0)
	{
		for (symbolTableItem *aux = st.head; aux != NULL; aux = aux->prox)
		{
			if((!strcmp(lexVal,aux->val)) && aux->class == class)
			{
				result = aux;
				break;
			}
		}
	}
	return result;
}

void addAnalysisQueue(symbolTableItem *item)
{
	analysisQueueItem *new = calloc(1,sizeof(analysisQueueItem));
	new->item = item;
	new->prox = NULL;
	if(aq.head == NULL)
	{
		aq.head = new;
	}
	else
	{
		analysisQueueItem *aux = NULL;
		for(aux = aq.head; aux->prox != NULL; aux = aux->prox);
		aux->prox = new;
	}
	++aq.size;
}

/*
 * 0 -> tipos diferentes
 * 1 -> tipos iguais
 * */

int doTypeCheck()
{
	int isTypeEquals = 1;
	if(aq.head->prox != NULL)
	{
		analysisQueueItem *aux = NULL;
		for(aux = aq.head->prox; aux != NULL; aux = aux->prox)
		{
			if(aux->item->type != aq.head->item->type)
			{
				isTypeEquals = 0;
				printf(RED"ERRO SEMÂNTICO: (%d:%d) TIPOS DE %s(%s) E %s(%s) SÃO DIFERENTES!\n"RESET,input.line,input.col,aq.head->item->val,semanticTypeNames[aq.head->item->type],aux->item->val,semanticTypeNames[aux->item->type]);
				break;
			}
		}
		if(isTypeEquals)
		{
			printf(GREEN"ANÁLISE SEMÂNTICA: (%d:%d) TIPOS SÃO IGUAIS!\n"RESET,input.line,input.col);
		}
	}
	else if(aq.head == NULL)
	{
		printf(RED"A analysis queue está vazia!\n"RESET);
	}
	cleanAnalysisQueue();
	return isTypeEquals;
}

symbolTableItem* addSymbolTableTag(int tagVal)
{
	char aux[ST_VAL_SIZE];
	sprintf(aux,"%d",tagVal);
	symbolTableItem *result = lookup(aux, SEMANTIC_CLASS_TAG);
	symbolTableItem *new = NULL;
	if(result == NULL)
	{
		new = calloc(1,sizeof(symbolTableItem));
		new->tokenType = IDLE;
		strcpy(new->val,aux);
		new->valSize = strlen(new->val);
		new->class = SEMANTIC_CLASS_TAG;
		new->type = SEMANTIC_TYPE_ADDRESS;
		if(st.head == NULL)
		{
			st.head = new;
		} else
		{
			symbolTableItem *aux = NULL;
			for(aux = st.head; aux->prox != NULL; aux = aux->prox);
			aux->prox = new;
		}
		++st.actualSize;
		return new;
	}
	return result;
}

symbolTableItem* addTempSymbolTable(semanticType type)
{
	symbolTableItem *new = NULL;
	new = calloc(1,sizeof(symbolTableItem));
	new->tokenType = IDLE;
	//Seta o nome do entrada temporária para t${st.numTemp}
	sprintf(new->val,"t%d",st.numTemp++);
	new->valSize = strlen(new->val);
	new->class = SEMANTIC_CLASS_TEMP;
	new->type = type;
	if(st.head == NULL)
	{
		st.head = new;
	} else
	{
		symbolTableItem *aux = NULL;
		for(aux = st.head; aux->prox != NULL; aux = aux->prox);
		aux->prox = new;
	}
	++st.actualSize;
	return new;
}

void initSemanticParser()
{
	initSemanticClassNames();
	initSemanticTypeNames();
	initSymbolTable();
	initAnalysisQueue();
}

void terminateSymbolTable()
{
	symbolTableItem *atual = st.head;
	if(atual != NULL)
	{
		symbolTableItem *prox = st.head->prox;
		if(prox != NULL)
		{
			while(prox != NULL)
			{
				free(atual);
				atual = prox;
				prox = prox->prox;
			}
		}
	}
	free(atual);
}

void terminateAnalysisQueue()
{
	analysisQueueItem *atual = aq.head;
	if(atual != NULL)
	{
		analysisQueueItem *prox = aq.head->prox;
		if(prox != NULL)
		{
			while(prox != NULL)
			{
				free(atual);
				atual = prox;
				prox = prox->prox;
			}
		}
	}
	free(atual);
}
void terminateSemanticParser()
{
	terminateSymbolTable();
	terminateAnalysisQueue();
}