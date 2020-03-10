#ifndef COMPILADOR_SYNTAXPARSER_H
#define COMPILADOR_SYNTAXPARSER_H

#include "lexParser.h"
#include "semanticParser.h"
#include "intermediateCodeGeneration.h"
#include "main.h"

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

void syntaxError(tokenClassification expected);

#endif //COMPILADOR_SYNTAXPARSER_H
