#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semanticParser.h"

int numTemp;
symbolTable st;
analysisQueue aq;
char* semanticClassNames[4];
char* semanticTypeNames[4];

void initSymbolTable()
{
	st.actualSize = 0;
	st.maxSize = ST_INITIAL_SIZE;
	st.entries = malloc(sizeof(symbolTableItem) * ST_INITIAL_SIZE);
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
	printf("symbolTable{\n\tactualSize: %d,\n\tmaxSize: %d,\n\tentries: [",st.actualSize,st.maxSize);
	if(st.actualSize > 0)
	{
		for (int i = 0; i < st.actualSize; i++) {
			printf("\n\t {\n\t\tlexVal: %s,\n\t\tlexValSize: %d,\n\t\ttokenType: %s,\n\t\tclass: %s,\n\t\ttype: %s,\n\t }",
				   st.entries[i].val, st.entries[i].valSize,
				   tokenTypesNames[st.entries[i].tokenType], semanticClassNames[st.entries[i].class],
				   semanticTypeNames[st.entries[i].type]);
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

int reallocSymbolTable()
{
	printf("oi\n");
	if(realloc(st.entries,st.maxSize + ST_INITIAL_SIZE) == NULL)
	{
		printf(RED"Não foi possível alocar mais memória para tabela de símbolos ¯\\_(ツ)_/¯"RESET);
		return 0;
	}
	st.maxSize += ST_INITIAL_SIZE;
	return 1;
}

symbolTableItem* addSymbolTable()
{
	symbolTableItem *result = NULL;
	if(lookup(token.lexVal) == NULL)
	{
		if (st.actualSize == st.maxSize) {
			if (!reallocSymbolTable()) {
				return NULL;
			}
		}
		result = &st.entries[st.actualSize];
		st.entries[st.actualSize].tokenType = token.type;
		strcpy(st.entries[st.actualSize].val, token.lexVal);
		st.entries[st.actualSize].valSize = token.size;
		st.entries[st.actualSize].class = SEMANTIC_CLASS_VAR;
		st.entries[st.actualSize++].type = SEMANTIC_TYPE_EMPTY;
	}
	return result;
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

symbolTableItem* lookup(char *lexVal)
{
	symbolTableItem* result = NULL;
	for(int i = 0; i < st.actualSize; i++)
	{
		if((!strcmp(lexVal,st.entries[i].val)) && st.entries[i].class == SEMANTIC_CLASS_VAR)
		{
			result = &st.entries[i];
			break;
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
 * -1 = Erro não tem elementos
 * 0 = Não faz coerção
 * 1 = float to int
 * 2 = int to float
 * */
int doTypeCoercion()
{
	if(aq.size > 0)
	{
		int doCoercion = 0;
		semanticType targetType = aq.head->item->type;
		analysisQueueItem *aux = NULL;
		if(aq.head->prox != NULL)
		{
			for(aux = aq.head->prox; aux != NULL; aux = aux->prox)
			{
				if(aux->item->type != targetType)
				{

					if(targetType == SEMANTIC_TYPE_INTEGER)
					{
						doCoercion = 1;
					} else
					{
						doCoercion = 2;
					}
					break;
				}
			}
		}
		cleanAnalysisQueue();
		return doCoercion;
	}
	else
	{
		printf(RED"A analysis queue está vazia!\n"RESET);
	}
	return -1;
}

symbolTableItem* addSymbolTableTag(int tagVal)
{
	char aux[32];
	sprintf(aux,"%d",tagVal);
	symbolTableItem *result = lookup(aux);
	if(result == NULL)
	{
		if (st.actualSize == st.maxSize)
		{
			if (!reallocSymbolTable())
			{
				return NULL;
			}
		}
		result = &st.entries[st.actualSize];
		st.entries[st.actualSize].tokenType = IDLE;
		strcpy(st.entries[st.actualSize].val,aux);
		st.entries[st.actualSize].valSize = strlen(st.entries[st.actualSize].val);
		st.entries[st.actualSize].class = SEMANTIC_CLASS_TAG;
		st.entries[st.actualSize++].type = SEMANTIC_TYPE_ADDRESS;
	}
	return result;
}

symbolTableItem* addTempSymbolTable(semanticType type)
{
	if (st.actualSize == st.maxSize)
	{
		if (!reallocSymbolTable())
		{
			return NULL;
		}
	}
	symbolTableItem* result = &st.entries[st.actualSize];
	st.entries[st.actualSize].tokenType = IDLE;
	//Seta o nome da entrada para t%d
	sprintf(st.entries[st.actualSize].val,"t%d",numTemp++);
	st.entries[st.actualSize].valSize = strlen(st.entries[st.actualSize].val);
	st.entries[st.actualSize].class = SEMANTIC_CLASS_TEMP;
	st.entries[st.actualSize++].type = type;
	return result;
}

void initSemanticParser()
{
	initSemanticClassNames();
	initSemanticTypeNames();
	initSymbolTable();
	initAnalysisQueue();

}