#ifndef COMPILADOR_SYNTAXPARSER_H
#define COMPILADOR_SYNTAXPARSER_H

#include "lexParser.h"
#include "semanticParser.h"
#include "intermediateCodeGeneration.h"
#include "utils.h"

int startAnalysis();

void syntaxError(tokenClassification expected);

#endif //COMPILADOR_SYNTAXPARSER_H
