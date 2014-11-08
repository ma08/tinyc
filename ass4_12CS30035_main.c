#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

#include "ass4_12CS30035_translator.h"
extern char* yytext;
int main() { 
	yyparse();
  printf("\n-------------GLOBAL SYMBOL TABLE--------------\n");
  currentSymbolTable->print();
  quads.print();
  /*quads.gen2x86();*/
	return 0;
}
