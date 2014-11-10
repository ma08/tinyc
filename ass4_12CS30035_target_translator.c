#include "y.tab.h"
#include<string.h>
#include<algorithm>
#include<vector>
#include <stdio.h>
#include <stdlib.h>

#include "ass4_12CS30035_translator.h"
int str_const=0;
extern char* yytext;
char* lastfun;
struct symrow* param_stack[20];
int param_size=0;


bool isNumber(char* a){
  int i=0;
  while(a[i]!='\0'){
    if(a[i]>='0'&&a[i]<='9'){
      i++;
      continue;
    }
    else{
      return false;
    }
  }
  return true;
}
enum Tp innerType(struct Type* t){

  //printf("\n\t------%d-------",t->typ);
  if(t->typ==T_POINTER)
    return T_POINTER;

  while(t->typ==T_POINTER||t->typ==T_ARRAY){
    t=t->next;
  }
  return t->typ;

}

void Quad::conv2x86(int x,vector<int>& labels,FILE* fp){
  //this->print();
  int i;
  Symboltable* st;
  if(find(labels.begin(),labels.end(),x)!=labels.end()){
    fprintf(fp,"\n.L%d:",x);
  }
  switch(this->op){
    case Q_FUNCSTART:
      st=globalSymbolTable.lookup(this->res)->nested_table;
      currentSymbolTable=st;
      for (i = 0; i < st->size; ++i){
        if(st->arr[i].type.typ==T_STRLIT){
          fprintf(fp,"\n.LC%d:",str_const);
          st->arr[i].strnum=str_const++;
          fprintf(fp,"\n\t.string %s",st->arr[i].initial.strval);
        }
      }
      fprintf(fp,"\n\t.text");
      fprintf(fp,"\n\t.globl %s",this->res);
      fprintf(fp,"\n\t.type %s, @function",this->res);
      lastfun=this->res;
      for (i = 0; i < st->size; ++i)
      {
        if(strcmp(st->arr[i].name,"retVal")==0||st->arr[i].type.typ==T_FUNCTION)
          continue;
        fprintf(fp,"\n\t_%s$ = %d",st->arr[i].name,st->arr[i].ebp_offset);
        
      }
      fprintf(fp,"\n%s:",this->res);
      fprintf(fp,"\n\tpushl %%ebp");
      fprintf(fp,"\n\tmovl %%esp, %%ebp");
      fprintf(fp,"\n\tsubl $%d, %%esp",-st->arr[st->size-1].ebp_offset+st->arr[st->size-1].size);
      //fprintf(fp,"\n\tsubl %%esp");
      break;
    case Q_FUNCEND:
      st=globalSymbolTable.lookup(this->res)->nested_table;
      fprintf(fp,"\n.L1ex%s:",this->res);
      if(strcmp(this->res,"main")!=0){
        fprintf(fp,"\n\taddl $%d, %%esp",-st->arr[st->size-1].ebp_offset+st->arr[st->size-1].size);
      }
	    fprintf(fp,"\n\tleave");
      //http://stackoverflow.com/questions/1317081/gccs-assembly-output-of-an-empty-program-on-x86-win32
	    fprintf(fp,"\n\tret");
      fprintf(fp,"\n\t.size %s, .-%s",this->res,this->res);
      break;
    case Q_UNARYMINUS:
      break;
    case Q_MULT:
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\timull $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
        break;
      }
      fprintf(fp,"\n\timull _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    case Q_DIVISION:
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcltd");
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tidivl $%s",this->arg2);
        fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
        break;
      }
      fprintf(fp,"\n\tidivl _%s$(%%ebp)",this->arg2);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    case Q_ARRACC:
      
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tmovl $%s, %%eax",this->arg2);
      }else{
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg2);
      }

      
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_POINTER){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%edx",this->arg1);
        if(currentSymbolTable->lookup(this->arg1)->type.next->typ==T_CHAR){
          fprintf(fp,"\n\tmovzbl (%%edx,%%eax,1), %%eax");
          fprintf(fp,"\n\tmovb %%al, _%s$(%%ebp)",this->res);
        }else{
          fprintf(fp,"\n\tmovl (%%edx,%%eax,1), %%eax");
          fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
        }
        break;
      }
      if(innerType(&currentSymbolTable->lookup(this->arg1)->type)==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp,%%eax,1), %%eax",this->arg1);
        fprintf(fp,"\n\tmovb %%al, _%s$(%%ebp)",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp,%%eax,1), %%edx",this->arg1);
      fprintf(fp,"\n\tmovl %%edx, _%s$(%%ebp)",this->res);
      break;
    case Q_ARR_COPY:
      if(isNumber(this->arg1)){
        fprintf(fp,"\n\tmovl $%s, %%edx",this->arg1);
      }else{
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%edx",this->arg1);
      }
      if(currentSymbolTable->lookup(this->res)->type.typ==T_POINTER){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->res);
        fprintf(fp,"\n\tleal (%%eax,%%edx,1), %%edx");
        if(currentSymbolTable->lookup(this->res)->type.next->typ==T_CHAR){
          fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg2);
          fprintf(fp,"\n\tmovb %%al, (%%edx)");
        }else{
          fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg2);
          fprintf(fp,"\n\tmovl %%eax, (%%edx)");
        }
        break;
      }
      if(innerType(&currentSymbolTable->lookup(this->arg1)->type)==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg2);
        fprintf(fp,"\n\tmovb %%al, _%s$(%%ebp,%%edx,1)",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp,%%edx,1)",this->res);
      break;
    case Q_DEREF:
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tmovl (%%eax), %%eax");
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    case Q_DEREF_COPY:
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%edx",this->arg1);
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->res);
      fprintf(fp,"\n\tmovl  %%edx, (%%eax)");
      break;
    case Q_POINT:
      fprintf(fp,"\n\tleal _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    case Q_MODULO:
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcltd");
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tidivl $%s",this->arg2);
        fprintf(fp,"\n\tmovl %%edx, _%s$(%%ebp)",this->res);
        break;
      }
      fprintf(fp,"\n\tidivl _%s$(%%ebp)",this->arg2);
      fprintf(fp,"\n\tmovl %%edx, _%s$(%%ebp)",this->res);
      break;

    case Q_PARAM:
      param_stack[param_size++]=currentSymbolTable->lookup(this->res);
      //fprintf(fp,"\n\tmovl	%%eax, (%%esp)");
      break;
    case Q_RET:
      if(this->res[0]!='\0'){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->res);
      }
      fprintf(fp,"\n\tjmp .L1ex%s",lastfun);
      break;
    case Q_FUNCALL:
      for(i=param_size-1 ; i>=0 ; i--){

        if(param_stack[i]->type.typ==T_ARRAY){
         fprintf(fp,"\n\tleal _%s$(%%ebp), %%eax",param_stack[i]->name);
        }else{
          if(param_stack[i]->type.typ==T_CHAR){
            fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",param_stack[i]->name);
            fprintf(fp,"movsbl	%%al, %%eax");
            fprintf(fp,"\n\tpush %%eal");
          }else
            fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",param_stack[i]->name);
        }
      fprintf(fp,"\n\tpush %%eax");  
      }
      param_size=0;
      fprintf(fp,"\n\tcall %s",this->arg1);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    case Q_COPY:
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_STRLIT){
          fprintf(fp,"\n\tmovl $.LC%d, _%s$(%%ebp)",currentSymbolTable->lookup(this->arg1)->strnum,this->res);
          break;
      }
      if(currentSymbolTable->lookup(this->res)->type.typ==T_CHAR){
        if(isNumber(this->arg1)){
          fprintf(fp,"\n\tmovb $%s, _%s$(%%ebp)",this->arg1,this->res);
          break;
        }
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tmovb %%al, _%s$(%%ebp)",this->res);
        break;
      }
      if(isNumber(this->arg1)){
        fprintf(fp,"\n\tmovl $%s, _%s$(%%ebp)",this->arg1,this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    case Q_GOTO:
      fprintf(fp,"\n\tjmp .L%s",this->res);
      break;
    case Q_REL_IFEQ:
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpb _%s$(%%ebp), %%al",this->arg2);
        fprintf(fp,"\n\tje .L%s",this->res);
        break;
      }
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tje .L%s",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcmpl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tje .L%s",this->res);
      break;
    case Q_REL_IFNEQ:
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpb _%s$(%%ebp), %%al",this->arg2);
        fprintf(fp,"\n\tjne .L%s",this->res);
        break;
      }
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tjne .L%s",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcmpl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tjne .L%s",this->res);
      break;
    case Q_REL_IFGT:
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpb _%s$(%%ebp), %%al",this->arg2);
        fprintf(fp,"\n\tjg .L%s",this->res);
        break;
      }
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tjg .L%s",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcmpl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tjg .L%s",this->res);
      break;
    case Q_REL_IFLT:
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpb _%s$(%%ebp), %%al",this->arg2);
        fprintf(fp,"\n\tjl .L%s",this->res);
        break;
      }
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tjl .L%s",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcmpl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tjl .L%s",this->res);
      break;
    case Q_REL_IFGTE:
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_CHAR){ fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpb _%s$(%%ebp), %%al",this->arg2);
        fprintf(fp,"\n\tjge .L%s",this->res);
        break;
      }
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tjge .L%s",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcmpl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tjge .L%s",this->res);
      break;
    case Q_REL_IFLTE:
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpb _%s$(%%ebp), %%al",this->arg2);
        fprintf(fp,"\n\tjle .L%s",this->res);
        break;
      }
      if(isNumber(this->arg2)){
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tcmpl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tjle .L%s",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tcmpl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tjle .L%s",this->res);
      break;
    case Q_PLUS:
      if(isNumber(this->arg2)){
        if(strcmp(this->res,this->arg1)==0){
          fprintf(fp,"\n\taddl $%s, _%s$(%%ebp)",this->arg2,this->res);
          break;
        }
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\taddl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\taddl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    case Q_MINUS:
      if(isNumber(this->arg2)){
        if(strcmp(this->res,this->arg1)==0){
          fprintf(fp,"\n\tsubl $%s, _%s$(%%ebp)",this->arg2,this->res);
          break;
        }
        fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
        fprintf(fp,"\n\tsubl $%s, %%eax",this->arg2);
        fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
        break;
      }
      fprintf(fp,"\n\tmovl _%s$(%%ebp), %%eax",this->arg1);
      fprintf(fp,"\n\tsubl _%s$(%%ebp), %%eax",this->arg2);
      fprintf(fp,"\n\tmovl %%eax, _%s$(%%ebp)",this->res);
      break;
    
    default:
      fprintf(fp,"\n------\n");
      this->print();
      break;
  }
}
void QuadArr::gen2x86(FILE *fp){
  vector<int> labels;
  int i;
  for (i = 0; i < globalSymbolTable.size; ++i)
  {
    enum Tp x=globalSymbolTable.arr[i].type.typ;
    if(x==T_INT||x==T_POINTER||x==T_CHAR||x==T_ARRAY){
      if(globalSymbolTable.arr[i].name[0]=='t')
        continue;
      fprintf(fp,"\n\t.comm %s,%d",globalSymbolTable.arr[i].name,globalSymbolTable.arr[i].size);
    }
  }
  fprintf(fp,"\n\t.text");
  enum Opcode op;

  for(i=0;i<this->size;++i){
    op=this->arr[i].op;
    if(op==Q_GOTO||op==Q_REL_IFEQ||op==Q_REL_IFGT||op==Q_REL_IFLT||op==Q_REL_IFLTE||op==Q_REL_IFGTE||op==Q_REL_IFNEQ){
      labels.push_back(atoi(this->arr[i].res));
    }
  }
  for (i = 0; i < this->size; ++i)
  {
    this->arr[i].conv2x86(i,labels,fp);
    
  }
}
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
