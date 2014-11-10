#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

#include "ass4_12CS30035_translator.h"
extern char* yytext;
int main() { 
  FILE *fp;
	yyparse();
  /*printf("\n-------------GLOBAL SYMBOL TABLE--------------\n");*/
  fp=fopen("quadoutput.out","w");
  if(fp!=NULL){
    quads.print(fp);
  }
  fclose(fp);
  /*quads.print();*/
  currentSymbolTable->activationRecord();
  /*currentSymbolTable->print();*/
  fp=fopen("tcgoutput.s","w");
  if(fp!=NULL){
    quads.gen2x86(fp);
    fprintf(fp,"\n");
  }
  fclose(fp);
	return 0;
}
