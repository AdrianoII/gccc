#ifndef COMPILADOR_MAIN_H
#define COMPILADOR_MAIN_H

#define RED "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"
#include <stdio.h>

typedef struct{
	FILE *entrada;
	int line;
	int col;
}inputType;

extern inputType input;

#endif //COMPILADOR_MAIN_H
