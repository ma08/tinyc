#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

#include "ass4_12CS30035_translator.h"
extern char* yytext;
int main() { 
	yyparse();
  currentSymbolTable->print();
  quads.print();
	return 0;
}
