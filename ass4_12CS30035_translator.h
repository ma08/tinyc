#ifndef __TRANSLATOR_HXX	// Control inclusion of header files
#define __TRANSLATOR_HXX

#include<vector>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
#define TRUE_VAL 1
#define FALSE_VAL 0

#define MAX 100000
#define QUADMAX 1000

extern int size_int;
extern int size_double;
extern int size_pointer;
extern int size_char;

enum Opcode{
  Q_PLUS=1, Q_MINUS, Q_DIVISION, Q_MODULO, Q_MULT, Q_UNARYMINUS, Q_COPY, Q_ARR, Q_IF, Q_IFFALSE, Q_REL_IFEQ, Q_REL_IFLT,
	Q_REL_IFGT, Q_REL_IFLTE, Q_REL_IFGTE, Q_REL_IFNEQ, Q_ARRVAL, Q_ARRDEREF, Q_GOTO, Q_REL_LT , Q_REL_GT, Q_REL_GTE, Q_REL_LTE,
  Q_REL_EQ, Q_REL_NEQ, Q_AMPERSAND, Q_XOR, Q_AROR, Q_LSH,Q_RSH,Q_ARRACC,Q_FUNCSTART,Q_FUNCEND,Q_FUNCALL,Q_PARAM,Q_INT2DOUBLE,Q_CHAR2DOUBLE,Q_DOUBLE2INT,Q_CHAR2INT,
  Q_INT2CHAR, Q_DOUBLE2CHAR,Q_ARR_COPY,Q_DEREF


};
enum Tp{
  T_VOID = 0, T_INT = 1, T_DOUBLE = 2, T_CHAR = 3, T_FUNCTION =4, T_POINTER=5, T_ARRAY = 6
};


struct Type{
  int length;
  enum Tp typ;
  struct Type* next;

  Type():length(0),typ(T_INT),next(NULL){}
  Type(enum Tp t,int l=0):length(l),typ(t),next(NULL){ }
  Type(enum Tp t,int l,struct Type* next=NULL):length(l),typ(t),next(next){ }
  //Type(struct Type& t):length(t.length),typ(t.typ),next(t.next){}
};


union Init{
  int intval;
  double doubleval;
  char charval;
  void* pointval;
};

class Symboltable;


struct symrow{
  //int length;
  char name[15];
  struct Type type;
  int size;
  int offset;
  union Init initial;
  Symboltable* nested_table;

  symrow():type(),size(size_int),initial(){}

  void setInitial(enum Tp t,union Init x){
    switch(t){
      case T_INT:
        initial.intval=x.intval;
        break;
      case T_CHAR:
        initial.charval=x.charval;
        break;
      case T_DOUBLE:
        initial.doubleval=x.doubleval;
        break;
      case T_POINTER:
        initial.pointval=x.pointval;
        break;
      default:
        break;
    }
  }
  void makePointer(int p);
  void makeArray(int length);
  void makeFunction(Symboltable* symTab);

  static void printType(const struct Type *t){
    if(t==NULL){
      printf("daaaaaafaaaaaaaaaq");
      return;
    }

    if(t->typ==T_ARRAY){
      printf("ARRAY(");
      printType(t->next);
      printf(", %d)",t->length);
      t=t->next;
      return;
    }
    if(t->typ==T_POINTER){
      printf("POINTER(");
      printType(t->next);
      printf(")");
      t=t->next;
      return;
    }
    if(t->typ==T_FUNCTION){
      printf("FUNCTION");
      return;
    }
    switch(t->typ){
      case T_INT:
        printf("INT");
        break;
      case T_VOID:
        printf("VOID");
        break;
      case T_DOUBLE:
        printf("DOUBLE");
        break;
      case T_CHAR:
        printf("CHAR");
        break;
      case T_FUNCTION:
        printf("FUNCTION");
        break;
      default:
        break;
    }

  };

  void print(){
    printf("\n%-7s | ",name);
    printType(&type);
    switch(type.typ){
      case T_INT:
        printf("| %d",initial.intval);
        break;
      case T_DOUBLE:
        printf("| %lf",initial.doubleval);
        break;
      case T_CHAR:
        printf("| %c",initial.charval);
        break;
      default:
        printf("| NULL");
        break;
    }
    printf("| %d | %d",size,offset);
    if(type.typ==T_FUNCTION){
      printf("| %p",nested_table);
    }else{
      printf("| NULL");
    }
  };
};


class Symboltable{
  public:
    //void makePointer(int p,struct symrow* row);
    //void makeArray(struct symrow* row,int length);
    struct symrow* lookup(const char *s);
    struct symrow* gentemp(struct Type type=Type());
    void update(char* name,struct Type type,int size,int offset,union Init initial);
    void print();
    bool exists(const char * s);
    
  private:
    struct symrow arr[MAX];
    //int size = 0;
    int size;
    //int tempcount = 0;
    int tempcount ;
};

class Quad{
  public:
    enum Opcode op;

    char arg1[15];
    char arg2[15];
    char res[15];

    void print();
 
};

class QuadArr{
  public:
    void emit(int op, char *r, char *a1, char*a2=0);
    void emit(int op, int r, char *a1, char*a2=0);
    void emit(int op, char *r, int num);
    void emit(int op, char *r);
    void emit(int op, int num);
    void emit(char *r, char* a);
    void emit(char *r, int num);
    void print();

    friend void backpatch(const vector<int>* p, int label);

    int size;
  private:
    Quad arr[QUADMAX];
    //int size=0;

};

extern Symboltable globalSymbolTable;
extern char* lastFunction;
extern Symboltable* currentSymbolTable ;
extern Symboltable* lastSymbolTable ;
extern QuadArr quads;

extern enum Tp lastType;

//extern int pointed;
extern int offset;

void backpatch(const vector<int>* p, int label);

void typecheck(struct symrow* e1 , struct symrow* e2,struct symrow **t1, struct symrow **t2);

vector<int>* makelist(int i);
vector<int>* merge(const vector<int>* p1,const vector<int>* p2);

int getsize(Type* type);

struct d_bool{
    struct symrow* sym;
    vector<int>* truelist;
    vector<int>* falselist;
    bool isBexp;
    /*d_bool():sym(),truelist(),falselist(){
      truelist=new vector<int>();
      falselist=new vector<int>();

    }*/
  };
void xtobool(struct d_bool* s);
struct symrow* xtoDouble(struct symrow* s);
struct symrow* xtoInt(struct symrow* s);
struct symrow* xtoChar(struct symrow* s);

#endif // __TRANSLATOR_HXX
