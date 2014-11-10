#include<stdio.h>
#include<vector>
#include<algorithm>
#include<list>
#include<string.h>
#include<stdlib.h>
#include "ass4_12CS30035_translator.h"
int size_int=4;
int size_double=8;
int size_char=1;
int size_pointer=4;

enum Tp lastType;
//int pointed;
int offset=0;

struct symrow* param_stack[20];
int param_size=0;

enum Tp innerType(struct Type* t){

  //printf("\n\t------%d-------",t->typ);
  if(t->typ==T_POINTER)
    return T_POINTER;

  while(t->typ==T_POINTER||t->typ==T_ARRAY){
    t=t->next;
  }
  return t->typ;

}

int str_const=0;

char* lastFunction;
Symboltable globalSymbolTable;
Symboltable* currentSymbolTable = &globalSymbolTable;
Symboltable* lastSymbolTable ;
QuadArr quads;
int getsize(Type* type){
    
    Type* t = type;
    int x=1;
    if(t->typ==T_POINTER){
      switch(t->next->typ){
          case T_INT:
            x=size_int;
            break;
          case T_CHAR:
            x=size_char;
            break;
          case T_DOUBLE:
            x=size_double;
            break;
          case T_POINTER:
            x=size_pointer;
            break;
          default:
            break;
        }
      return x;
    }
    if(type->typ!=T_ARRAY)
      return -1;
    t=t->next;
    while(t!=NULL){
      if(t->typ!=T_ARRAY){
        switch(t->typ){
          case T_INT:
            x=x*size_int;
            break;
          case T_CHAR:
            x=x*size_char;
            break;
          case T_DOUBLE:
            x=x*size_double;
            break;
          case T_POINTER:
            x=x*size_pointer;
            break;
          default:
            break;
        }
        return x;
      }
      x=x*t->length;
      t=t->next;
    }
    return x;
  };
bool typec(struct Type* t1, struct Type* t2){
  if(t1==NULL&&t2==NULL)
    return true;
  if(t1==NULL&&t2!=NULL)
    return false;
  if(t1!=NULL&&t2==NULL)
    return false;
  if(t1->typ==t2->typ){
    return typec(t1->next,t2->next);
  }
  return false;
};

void Symboltable::activationRecord(){
  int i;int x=8;
  for (i = 0; i < this->params; ++i)
  {
    this->arr[i].ebp_offset=x;
    //x=x+this->arr[i].size;
    x=x+4;
  }
  if(strcmp(this->arr[i].name,"retVal")==0){
    i++;
  }
  x=0;
  int j;
  for (j=i;  j < this->size; j++)
  {
    if(this->arr[j].type.typ==T_FUNCTION){
      this->arr[j].nested_table->activationRecord();
      continue;
    }
    x=x-this->arr[j].size;
    this->arr[j].ebp_offset=x;
  }
   
}

void typecheck(struct symrow* e1, struct symrow* e2,struct symrow **t1, struct symrow **t2){
  if(e1->type.typ==T_INT){
    if(e2->type.typ==T_DOUBLE){
      *t1=xtoDouble(e1);
      *t2=e2;
      return;
    }
    if(e2->type.typ==T_CHAR){
      *t1=e1;
      *t2=xtoInt(e2);
      return;
    }
    if(e2->type.typ==T_INT){
      *t1=e1;
      *t2=e2;
      return;
    }
  }
  if(e1->type.typ==T_DOUBLE){
    if(e2->type.typ==T_DOUBLE){
      *t1=e1;
      *t2=e2;
      return;
    }
    if(e2->type.typ==T_CHAR){
      *t1=e1;
      *t2=xtoDouble(e2);
      return;
    }
    if(e2->type.typ==T_INT){
      *t1=e1;
      *t2=xtoDouble(e2);
      return;
    }
  }
  if(e1->type.typ==T_CHAR){
    if(e2->type.typ==T_DOUBLE){
      *t1=xtoDouble(e1);
      *t2=e2;
      return;
    }
    if(e2->type.typ==T_CHAR){
      *t1=e1;
      *t2=e2;
      return;
    }
    if(e2->type.typ==T_INT){
      *t1=xtoInt(e1);
      *t2=e2;
      return;
    }
  }
};

void Symboltable::print(){
  int i;
  printf("\nparams: %d \n",this->params);
  for (i = 0; i < this->size; ++i)
  {
    this->arr[i].print();
    
  }
    for (i = 0; i < this->size; ++i)
  {
    if(this->arr[i].type.typ==T_FUNCTION){

      printf("\n-----------------TABLE AT %s---------\n",this->arr[i].name);
      this->arr[i].nested_table->print();
      printf("\n--------------------------\n");
    }
  }
};
void symrow::makeArray(int length,Symboltable* currentSymbolTable){
  //this->printType(&this->type);
  //printf("\n---\n");
  //this->printType(&this->type);
  struct Type* dum=&this->type;
  struct Type* dum2=&this->type;
  enum Tp tp;
  tp=dum->typ;
  if(dum->typ==T_POINTER){
    //printf("\n---\n");
    //this->printType(dum);
    struct Type* n=new Type(this->type);
    this->type.typ=T_ARRAY;
    this->type.length=length;
    this->type.next=n;
    this->size=length*this->size;
    return;
  }
  while(dum!=NULL){
    tp=dum->typ;
  //printf("hhhhhhhhhhhhhh %d\n",tp);
    dum2=dum;
    dum=dum->next;
  }
  //printf("\n----ssssssss------%d-----------\n",tp);
  struct Type* t=new Type(tp,length,NULL);
  dum2->next=t;
  dum2->typ=T_ARRAY;
  dum2->length=length;
  //this->printType(dum2);
  //this->type.length=length;
  switch(this->type.typ){
   case T_INT:
    this->size=length*size_int;
    break;
   case T_CHAR:
    this->size=length*size_char;
    break;
   case T_DOUBLE:
    this->size=length*size_double;
    break;
   case T_POINTER:
    this->size=length*size_pointer;
    break;
   case T_ARRAY:
    this->size=length*this->size;
   default:
    break;
  }
  int k;
  for (k = 1; k < currentSymbolTable->size ; ++k)
  {
    currentSymbolTable->arr[k].offset = currentSymbolTable->arr[k-1].size+currentSymbolTable->arr[k-1].offset;
  }
};

void symrow::makeFunction(Symboltable* symTab){
  this->type.next=new Type(this->type);
  this->type.typ=T_FUNCTION;
  this->nested_table=symTab;
  struct symrow* s=symTab->gentemp(this->type.next->typ);
  this->type.next=NULL;
  strcpy(s->name,"retVal");
  this->size=0;
  return;
};

/*void Symboltable::makeArray(struct symrow* row,int length){
  //row->printType(&row->type);
  //printf("\n---\n");
  //row->printType(&row->type);
  struct Type* dum=&row->type;
  struct Type* dum2=&row->type;
  enum Tp tp;
  tp=dum->typ;
  if(dum->typ==T_POINTER){
    //printf("\n---\n");
    //row->printType(dum);
    struct Type* n=new Type(row->type);
    row->type.typ=T_ARRAY;
    row->type.length=length;
    row->type.next=n;
    row->size=length*row->size;
    return;
  }
  while(dum!=NULL){
    tp=dum->typ;
  //printf("hhhhhhhhhhhhhh %d\n",tp);
    dum2=dum;
    dum=dum->next;
  }
  //printf("\n----ssssssss------%d-----------\n",tp);
  struct Type* t=new Type(tp,length,NULL);
  dum2->next=t;
  dum2->typ=T_ARRAY;
  dum2->length=length;
  //row->printType(dum2);
  //row->type.length=length;
  switch(row->type.typ){
   case T_INT:
    row->size=length*size_int;
    break;
   case T_CHAR:
    row->size=length*size_char;
    break;
   case T_DOUBLE:
    row->size=length*size_double;
    break;
   case T_POINTER:
    row->size=length*size_pointer;
    break;
   case T_ARRAY:
    row->size=length*row->size;
   default:
    break;
  }
};
*/
void symrow::makePointer(int p){
   if(p){
    if(this->type.typ==T_ARRAY){
      while(p--){
        struct Type* dum=&this->type;
        struct Type* dum2=&this->type;
        enum Tp tp;
        tp=dum->typ;
        while(dum->next!=NULL){
          tp=dum->typ;
        //printf("hhhhhhhhhhhhhh %d\n",tp);
          dum2=dum;
          dum=dum->next;
        }
        struct Type* t=new Type(T_POINTER,0,dum2->next);
        t->next=dum2->next;
        dum2->next=t;
      }
      return;
    }
    if(this->type.typ==T_FUNCTION){
      struct symrow* s=this->nested_table->lookup("retVal");
      while(p--){
        struct Type* dum=new struct Type(s->type);
        s->type.next = dum;
        s->type.typ=T_POINTER;
        s->size=size_pointer;
      }      


      return;

    }
    //printf("\nrrrrrn");
    while(p--){
      struct Type* dum=new struct Type(this->type);
      this->type.next = dum;
      this->type.typ=T_POINTER;
      this->size=size_pointer;
    }
  }
};


/*void Symboltable::makePointer(int p,struct symrow* row){

  if(p){
    if(row->type.typ==T_ARRAY){
      while(p--){
        struct Type* dum=&row->type;
        struct Type* dum2=&row->type;
        enum Tp tp;
        tp=dum->typ;
        while(dum->next!=NULL){
          tp=dum->typ;
        //printf("hhhhhhhhhhhhhh %d\n",tp);
          dum2=dum;
          dum=dum->next;
        }
        struct Type* t=new Type(T_POINTER,0,dum2->next);
        t->next=dum2->next;
        dum2->next=t;
      }
      return;
    }
    //printf("\nrrrrrn");
    while(p--){
      struct Type* dum=new struct Type(row->type);
      row->type.next = dum;
      row->type.typ=T_POINTER;
      row->size=size_pointer;
    }
  }
};*/

void Quad::print(FILE *fp){
  switch(this->op){
    case Q_COPY:
      fprintf(fp,"%s = %s",this->res,this->arg1);
      break;
    case Q_PLUS:
      fprintf(fp,"%s = %s + %s",this->res,this->arg1,this->arg2);
      break;
    case Q_MINUS:
      fprintf(fp,"%s = %s - %s",this->res,this->arg1,this->arg2);
      break;
    case Q_MULT:
      fprintf(fp,"%s = %s * %s",this->res,this->arg1,this->arg2);
      break;
    case Q_MODULO:
      fprintf(fp,"%s = %s %% %s",this->res,this->arg1,this->arg2);
      break;
    case Q_DIVISION:
      fprintf(fp,"%s = %s / %s",this->res,this->arg1,this->arg2);
      break;
    case Q_AMPERSAND:
      fprintf(fp,"%s = %s & %s",this->res,this->arg1,this->arg2);
      break;
    case Q_POINT:
      fprintf(fp,"%s = &%s",this->res,this->arg1);
      break;

    case Q_AROR:
      fprintf(fp,"%s = %s | %s",this->res,this->arg1,this->arg2);
      break;
    case Q_XOR:
      fprintf(fp,"%s = %s ^ %s",this->res,this->arg1,this->arg2);
      break;
    case Q_LSH:
      fprintf(fp,"%s = %s << %s",this->res,this->arg1,this->arg2);
      break;
    case Q_RSH:
      fprintf(fp,"%s = %s >> %s",this->res,this->arg1,this->arg2);
      break;
    case Q_REL_IFLT:
      fprintf(fp,"if(%s < %s) goto %s",this->arg1,this->arg2,this->res);
      break;
    case Q_REL_IFEQ:
      fprintf(fp,"if(%s == %s) goto %s",this->arg1,this->arg2,this->res);
      break;
    case Q_REL_IFNEQ:
      fprintf(fp,"if(%s != %s) goto %s",this->arg1,this->arg2,this->res);
      break;
    case Q_REL_IFGT:
      fprintf(fp,"if(%s > %s) goto %s",this->arg1,this->arg2,this->res);
      break;
    case Q_REL_IFGTE:
      fprintf(fp,"if(%s >= %s) goto %s",this->arg1,this->arg2,this->res);
      break;
    case Q_REL_IFLTE:
      fprintf(fp,"if(%s <= %s) goto %s",this->arg1,this->arg2,this->res);
      break;
    case Q_GOTO:
      fprintf(fp,"goto %s",this->res);
      break;
    case Q_RET:
      if(this->res!=NULL)
        fprintf(fp,"return %s",this->res);
      else
        fprintf(fp,"return");
      break;
    case Q_ARRACC:
      fprintf(fp,"%s = %s[%s]",this->res,this->arg1,this->arg2);
      break;
    case Q_FUNCSTART:
      fprintf(fp,"%s start ",this->res);
      break;
    case Q_FUNCEND:
      fprintf(fp,"End %s  ",this->res);
      fprintf(fp,"\n");
      break;
    case Q_FUNCALL:
      fprintf(fp,"%s = call %s, %s",this->res,this->arg1,this->arg2);
      break;
    case Q_PARAM:
      fprintf(fp,"param %s",this->res);
      break;
    case Q_ARR_COPY:
      fprintf(fp,"%s[%s] = %s",this->res,this->arg1,this->arg2);
      break;
    case Q_INT2CHAR:
      fprintf(fp,"%s = int2Char(%s)",this->res,this->arg1);
      break;
    case Q_INT2DOUBLE:
      fprintf(fp,"%s = int2Double(%s)",this->res,this->arg1);
      break;
    case Q_CHAR2DOUBLE:
      fprintf(fp,"%s = char2Double(%s)",this->res,this->arg1);
      break;
    case Q_CHAR2INT:
      fprintf(fp,"%s = char2Int(%s)",this->res,this->arg1);
      break;
    case Q_DOUBLE2INT:
      fprintf(fp,"%s =double2Int(%s)",this->res,this->arg1);
      break;
    case Q_DOUBLE2CHAR:
      fprintf(fp,"%s =double2Char(%s)",this->res,this->arg1);
      break;
    case Q_DEREF:
      fprintf(fp,"%s =*%s",this->res,this->arg1);
      break;
    case Q_DEREF_COPY:
      fprintf(fp,"*%s=%s",this->res,this->arg1);
      break;
    default:
      break;
  }
};

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
char* lastfun;

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
      if(currentSymbolTable->lookup(this->arg1)->type.typ==T_CHAR){
        fprintf(fp,"\n\tmovzbl _%s$(%%ebp), %%eax",this->arg1);
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

void QuadArr::print(FILE *fp){
  int i;
  fprintf(fp,"\n");
  for (i = 0; i < this->size; ++i)
  {
    fprintf(fp,"\n%d: ",i);
    this->arr[i].print(fp);
  }
    fprintf(fp,"\n");
}

bool Symboltable::exists(const char* s){
  int i=0;
  for (i = 0; i < this->size; ++i)
    if(strcmp(this->arr[i].name,s)==0)
      return true;
  return false;
}

struct symrow* Symboltable::lookup(const char* s){
  int i=0;
  for (i = 0; i < this->size; ++i)
    if(strcmp(this->arr[i].name,s)==0)
      return &this->arr[i];
  strcpy(this->arr[this->size].name,s);
  //printf("\n--%d---\n",pointed);

  /*if(pointed){
    this->arr[this->size].type.next = new struct Type();
    this->arr[this->size].type.next->typ=lastType;
    this->arr[this->size].type.typ=T_POINTER;
    lastType = T_POINTER;
  }
  else*/
  this->arr[this->size].type.typ=lastType;
  if(this->size!=0)
    this->arr[this->size].offset=this->arr[this->size-1].offset+this->arr[this->size-1].size;
  else{
    this->arr[this->size].offset=0;
  }
  switch(lastType){
    case T_VOID:
      this->arr[this->size].size=0;
      break;
    case T_INT:
      this->arr[this->size].size=size_int;
      break;
    case T_CHAR:
      this->arr[this->size].size=size_char;
      break;
    case T_DOUBLE:
      this->arr[this->size].size=size_double;
      break;
    case T_POINTER:
      this->arr[this->size].size=size_pointer;
      break;
    default:
      break;
  }
  this->size++;
  return &this->arr[this->size-1];
};

struct symrow* Symboltable::gentemp(struct Type type){
  this->arr[this->size].name[0]='t';
  sprintf(&this->arr[this->size].name[1],"%d",this->tempcount++);
  this->arr[this->size].type=type;
  if(this->size!=0)
    this->arr[this->size].offset=this->arr[this->size-1].offset+this->arr[this->size-1].size;
  else{
    this->arr[this->size].offset=0;
  }
  switch(type.typ){
    case T_VOID:
      this->arr[this->size].size=0;
      break;
    case T_INT:
      this->arr[this->size].size=size_int;
      break;
    case T_CHAR:
      this->arr[this->size].size=size_char;
      break;
    case T_DOUBLE:
      this->arr[this->size].size=size_double;
      break;
    case T_POINTER:
      this->arr[this->size].size=size_pointer;
      break;
    default:
      break;
  }
  this->size++;
  return &this->arr[this->size-1];
};


void QuadArr::emit(int op,char *r,char *a1,char *a2){
  if(a2==0)
    this->arr[this->size].arg2[0]='\0';
  else
    strcpy(this->arr[this->size].arg2,a2);
  (this->arr[this->size].op)=(Opcode)op;
  strcpy(this->arr[this->size].res,r);
  strcpy(this->arr[this->size].arg1,a1);
  this->size++;
};

void QuadArr::emit(int op,int r,char *a1,char *a2){
  /*if(a2==0)
    this->arr[this->size].arg2[0]='\0';
  else
    strcpy(this->arr[this->size].arg2,a2);*/

  strcpy(this->arr[this->size].arg2,a2);
  (this->arr[this->size].op)=(Opcode)op;
  if(r!=-1)
    sprintf(this->arr[this->size].res,"%d",r);
  else
    sprintf(this->arr[this->size].res,"...");
  strcpy(this->arr[this->size].arg1,a1);
  this->size++;
};


void QuadArr::emit(int op,char *r,int num){
  this->arr[this->size].op=(Opcode)op;
  strcpy(this->arr[this->size].res,r);
  sprintf(this->arr[this->size].arg1,"%d",num);
  this->arr[this->size].arg2[0]='\0';
  this->size++;
};
void QuadArr::emit(int op,char *r){
  this->arr[this->size].op=(Opcode)op;
  strcpy(this->arr[this->size].res,r);
  this->arr[this->size].arg1[0]='\0';
  this->arr[this->size].arg2[0]='\0';
  this->size++;
};
void QuadArr::emit(int op,int num){
  this->arr[this->size].op=(Opcode)op;
  if(num!=-1)
    sprintf(this->arr[this->size].res,"%d",num);
  else
    sprintf(this->arr[this->size].res,"...");

  this->arr[this->size].arg1[0]='\0';
  this->arr[this->size].arg2[0]='\0';
  this->size++;
};

void QuadArr::emit(char *r, char* a){
  this->arr[this->size].op=Q_COPY;
  strcpy(this->arr[this->size].res,r);
  strcpy(this->arr[this->size].arg1,a);
  this->arr[this->size].arg2[0]='\0';
  this->size++;
};
void QuadArr::emit(char *r, int num){
  this->arr[this->size].op=Q_COPY;
  strcpy(this->arr[this->size].res,r);
  sprintf(this->arr[this->size].arg1,"%d",num);
  this->arr[this->size].arg2[0]='\0';
  this->size++;
};

vector<int>* makelist(int i){
  vector<int>* list=new vector<int>();
  list->push_back(i);
  return list;
};

vector<int>* merge(const vector<int>* p1,const vector<int>* p2){
  vector<int>* final=new vector<int>(*p1);
  int i;
  for (i = 0; i < p2->size(); ++i) 
    final->push_back((*p2)[i]);

  return final;
}

void backpatch(const vector<int>* p, int label){
  int i;
  for (i = 0; i < p->size(); ++i) 
    sprintf( quads.arr[(*p)[i]].res,"%d",label);
};

void xtobool(struct d_bool* s){
  if(s->isBexp){
    return;
  }
  s->isBexp=true;
  s->truelist=makelist(quads.size);
  s->falselist=makelist(quads.size+1);
  char c[30];
  sprintf(c,"%d",FALSE_VAL);
  quads.emit(Q_REL_IFNEQ,-1,s->sym->name,c);
  quads.emit(Q_GOTO,-1);
}

struct symrow* xtoDouble(struct symrow* s){
  struct symrow* t; 
  if(s->type.typ==T_DOUBLE){
    return s;
  }
  if(s->type.typ==T_INT){
    t=currentSymbolTable->gentemp(T_DOUBLE);
    quads.emit(Q_INT2DOUBLE,t->name,s->name);
    return t;
  }
  if(s->type.typ==T_CHAR){
    t=currentSymbolTable->gentemp(T_DOUBLE);
    quads.emit(Q_CHAR2DOUBLE,t->name,s->name);
    return t;
  }
  return s;
}

struct symrow* xtoInt(struct symrow* s){
  struct symrow* t; 
  if(s->type.typ==T_INT){
    return s;
  }
  if(s->type.typ==T_DOUBLE){
    
    t=currentSymbolTable->gentemp(T_INT);
    quads.emit(Q_DOUBLE2INT,t->name,s->name);
    return t;
  }
  if(s->type.typ==T_CHAR){
    t=currentSymbolTable->gentemp(T_INT);
    quads.emit(Q_CHAR2INT,t->name,s->name);
    return t;
  }
  return s;
}
struct symrow* xtoChar(struct symrow* s){
  struct symrow* t; 
  if(s->type.typ==T_CHAR){
    return s;
  }
  /*if(s->type.typ==T_DOUBLE){
    t=currentSymbolTable->gentemp(T_CHAR);
    quads.emit(Q_DOUBLE2INT,t->name,s->name);
    return t;
  }*/
  if(s->type.typ==T_INT){
    t=currentSymbolTable->gentemp(T_CHAR);
    quads.emit(Q_INT2CHAR,t->name,s->name);
    return t;
  }
  return s;
}





