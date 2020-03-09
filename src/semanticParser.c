#include <stdlib.h>
#include <string.h>
#include "semanticParser.h"

symbolTable st;
analisysQueue aq;
char* semanticClassNames[2];
char* semanticTypeNames[3];
char* analisysTypeNames[2];

void initSymbolTable()
{
	st.acutalSize = 0;
	st.maxSize = TS_INITIAL_SIZE;
	st.entries = malloc(sizeof(symbolTableItem) * TS_INITIAL_SIZE);
}

void initSemanticClassNames()
{
	semanticClassNames[0] = "SEMANTIC_CLASS_EMPTY";
	semanticClassNames[1] = "SEMANTIC_CLASS_VAR";
}

void initSemanticTypeNames()
{
	semanticTypeNames[0] = "SEMANTIC_TYPE_EMPTY";
	semanticTypeNames[1] = "SEMANTIC_TYPE_INTEGER";
	semanticTypeNames[2] = "SEMANTIC_TYPE_REAL";
}

void initAnalisysTypeNames()
{
	analisysTypeNames[0] = "TARGET";
	analisysTypeNames[1] = "OPERAND";
}

void printSymbolTable()
{
	printf("symbolTable{\n\tactualSize: %d,\n\tmaxSize: %d,\n\tentries: [",st.acutalSize,st.maxSize);
	if(st.acutalSize > 0)
	{
		for (int i = 0; i < st.acutalSize; i++) {
			printf("\n\t {\n\t\tlexVal: %s,\n\t\tlexValSize: %d,\n\t\ttokenClass: %s,\n\t\ttokenType: %s,\n\t\tclass: %s,\n\t\ttype: %s\n\t }",
				   st.entries[i].lexVal, st.entries[i].lexValSize, tokenClassNames[st.entries[i].tokenClass],
				   tokenTypesNames[st.entries[i].tokenType], semanticClassNames[st.entries[i].class],
				   semanticTypeNames[st.entries[i].type]);
		}
	}
	printf("\n\t]\n}\n");
}

void printAnalisysQueue()
{
	printf("analisysQueue{\n\tsize: %d,\n\thead: [",aq.size);
	if(aq.size > 0)
	{
		for (analisysQueueItem *aux = aq.head; aux != NULL; aux = aux->prox)
		{
			printf("\n\t {\n\t\tlexVal: %s,\n\t\tlexValSize: %d,\n\t\ttokenClass: %s,\n\t\ttokenType: %s,\n\t\tclass: %s,\n\t\ttype: %s\n\t }",
				   aux->item->lexVal, aux->item->lexValSize, tokenClassNames[aux->item->tokenClass],
				   tokenTypesNames[aux->item->tokenType], semanticClassNames[aux->item->class],
				   semanticTypeNames[aux->item->type]);
		}
	}
	printf("\n\t]\n}\n");
}

int reallocSymbolTable()
{
	if(realloc(st.entries,st.maxSize + TS_INITIAL_SIZE) == NULL)
	{
		printf(RED"Não foi possível alocar mais memória para tabela de símbolos ¯\\_(ツ)_/¯"RESET);
		return 0;
	}
	st.maxSize += TS_INITIAL_SIZE;
	return 1;
}

symbolTableItem* addSymbolTable()
{
	symbolTableItem *result = NULL;
	if(lookup(token.lexVal) == NULL)
	{
		if (st.acutalSize == st.maxSize) {
			if (!reallocSymbolTable()) {
				return NULL;
			}
		}
		result = &st.entries[st.acutalSize];
		st.entries[st.acutalSize].tokenClass = token.class;
		st.entries[st.acutalSize].tokenType = token.type;
		strcpy(st.entries[st.acutalSize].lexVal, token.lexVal);
		st.entries[st.acutalSize].lexValSize = token.size;
		st.entries[st.acutalSize].class = SEMANTIC_CLASS_VAR;
		st.entries[st.acutalSize++].type = SEMANTIC_TYPE_EMPTY;
	}
	return result;
}

void initAnalisysQueue()
{
	aq.size = 0;
	aq.head = NULL;
}

void addTypeRange()
{
	semanticType type = token.type == INTEGER ? SEMANTIC_TYPE_INTEGER : SEMANTIC_TYPE_REAL;
	for(analisysQueueItem* actual = aq.head; actual != NULL; actual = actual->prox)
	{
		actual->item->type = type;
	}
	cleanAnalisysQueue();
}

symbolTableItem* lookup(char *lexVal)
{
	symbolTableItem* result = NULL;
	for(int i = 0; i < st.acutalSize; i++)
	{
		if(!strcmp(lexVal,st.entries[i].lexVal))
		{
			result = &st.entries[i];
			break;
		}
	}
	return result;
}

void addAnalisysQueue(symbolTableItem *item)
{
	analisysQueueItem *new = calloc(1,sizeof(analisysQueueItem));
	new->item = item;
	new->prox = NULL;
	if(aq.head == NULL)
	{
		aq.head = new;
	}
	else
	{
		analisysQueueItem *aux = NULL;
		for(aux = aq.head; aux->prox != NULL; aux = aux->prox);
		aux->prox = new;
	}
	++aq.size;
}

void cleanAnalisysQueue()
{
	if(aq.head != NULL)
	{
		analisysQueueItem *atual = aq.head;
		analisysQueueItem *prox = aq.head->prox;
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
		initAnalisysQueue();
	}
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
		analisysQueueItem *aux = NULL;
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
		cleanAnalisysQueue();
		return doCoercion;
	}
	else
	{
		printf(RED"A analisys queue está vazia!\n"RESET);
	}
	return -1;
}
