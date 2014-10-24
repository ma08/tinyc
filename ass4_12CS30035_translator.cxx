#include<stdio.h>
#include<vector>
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


Symboltable globalSymbolTable;
Symboltable* currentSymbolTable = &globalSymbolTable;
QuadArr quads;


void Symboltable::print(){
  int i;
  for (i = 0; i < this->size; ++i)
  {
    this->arr[i].print();
  }

};
void symrow::makeArray(int length){
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

void QuadArr::emit(int op,char *r,int num){
  this->arr[this->size].op=(Opcode)op;
  strcpy(this->arr[this->size].res,r);
  sprintf(this->arr[this->size].arg1,"%d",num);
  this->arr[this->size].arg2[0]='\0';
  this->size++;
};
void QuadArr::emit(int op,int num){
  this->arr[this->size].op=(Opcode)op;
  sprintf(this->arr[this->size].res,"%d",num);
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


vector<int>* makelist(int i){
  vector<int>* list=new vector<int>();
  list->push_back(i);
  return list;
};

vector<int>* merge(const vector<int>& p1,const vector<int>& p2){
  vector<int>* final=new vector<int>(p1);
  int i;
  for (i = 0; i < p2.size(); ++i) 
    final->push_back(p2[i]);

  return final;
}

void backpatch(const vector<int>& p, int label){
  int i;
  for (i = 0; i < p.size(); ++i) 
    sprintf( quads.arr[i].res,"%d",label);
};
