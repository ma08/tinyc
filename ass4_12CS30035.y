%{
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "ass4_12CS30035_translator.h"
extern int yylex();
extern char* yytext;
void yyerror(char *s);



%}
%code requires {
  #include "ass4_12CS30035_translator.h"
    #include<vector> 
  struct constant_val
  {
    
      enum Tp type;
      union
      {
          double dval;
          int   ival;
          char cval;
      };
  };
  struct array_type{

    Type* type;
    struct symrow* sym;
    struct symrow* id_sym;
  };
    
  
}
%union
{
  struct constant_val conval;
  Symboltable* symTab;
  struct d_bool boool;
  vector<int>* nextlist;
  struct array_type array_exp;
  
  enum Tp typeValue;
  int intValue; 
  double doubleValue;
  char charValue;
  char *stringValue;
  char charArray[30];
  struct symrow* symRow;
}

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token CHAR SHORT INT LONG SIGNED UNSIGNED DOUBLE CONST VOLATILE VOID
%token RESTRICT ENUM UNION BOOL IMAGINARY COMPLEX SIZEOF STRUCT
%token ENUMARATION_CONSTANT

%token <charArray> IDENTIFIER

%token <intValue> INT_CONST
%token <doubleValue> DOUBLE_CONST
%token <charValue> CHAR_CONST
%token <stringValue> STR_LITERAL
%token ARR 
%token INC 
%token DEC 
%token RSH 
%token LSH 
%token RANGB_EQ 
%token LANGB_EQ 
%token DOUBLE_EQ 
%token EXCL_EQ 
%token OR 
%token AND 
%token ELLIPSIS 
%token MULT_ASSIGN 
%token DIV_ASSIGN 
%token MOD_ASSIGN 
%token ADD_ASSIGN 
%token SUB_ASSIGN 
%token LSH_ASSIGN 
%token RSH_ASSIGN 
%token BIAND_ASSIGN 
%token BIOR_ASSIGN 
%token BIXOR_ASSIGN 


%token MULT_COMM
%token SING_COMM

%type <intValue> pointer_opt
%type <intValue> pointer
%type <conval> constant

%type <typeValue> type_specifier

%type <array_exp> array_expression

%nonassoc THEN 
%nonassoc ELSE


%start translation_unit
/*%type <symRow> assignment_expression*/

%type <boool> primary_expression postfix_expression unary_expression cast_expression
%type <boool> multiplicative_expression additive_expression shift_expression  
%type <symRow> init_declarator_list init_declarator_list_opt 
%type <symTab> funcdecstart
%type <symRow> declaration init_declarator
%type <symRow> declarator direct_declarator 
%type <intValue> argument_expression_list_opt argument_expression_list

%type <intValue> M

%type <boool> logical_AND_expression logical_OR_expression inclusive_OR_expression relational_expression equality_expression expression_opt
%type <boool> AND_expression exclusive_OR_expression  conditional_expression expression booexpression booexpression_opt assignment_expression assignment_expression_opt initializer


%type <nextlist> statement labeled_statement compound_statement jump_statement iteration_statement expression_statement selection_statement 
%type <nextlist> block_item_list block_item_list_opt block_item  N
%%
identifier_opt:
			  |IDENTIFIER
			  ;
	
primary_expression:
					IDENTIFIER 
  {if(currentSymbolTable->exists($1)){
        $$.sym=currentSymbolTable->lookup($1);
    }else{
        if(globalSymbolTable.exists($1)){
            $$.sym=globalSymbolTable.lookup($1);
        }else{
            $$.sym=currentSymbolTable->lookup($1);
        }
    }
         /*$1->print();*/}
          |constant
         {
            $$.sym=currentSymbolTable->gentemp(Type($1.type));

            switch($1.type){
              case T_INT:
                /*printf("kkkkkkkkkkkkk%d",$1.ival);*/
                $$.sym->initial.intval=$1.ival;
                break;
              case T_DOUBLE:
                $$.sym->initial.doubleval=$1.dval;
                break;
              case T_CHAR:
                $$.sym->initial.charval=$1.cval;
                break;
              default:
                break;
            }
         }
				 |STR_LITERAL{}
				 | '(' expression ')' {$$=$2;}
				  ;
constant:
		INT_CONST { $$.type=T_INT; $$.ival=$1;}
		|DOUBLE_CONST {$$.type=T_DOUBLE; $$.dval = $1;}
		/*|ENUMARATION_CONSTANT*/
		|CHAR_CONST {$$.type=T_CHAR; $$.cval=$1;}
		;
postfix_expression:
          primary_expression {$$=$1; /*$$->print();*/}
					/*|postfix_expression '[' expression ']'*/
          |array_expression{$$.sym=currentSymbolTable->gentemp($1.type->typ); quads.emit(Q_ARRACC,$$.sym->name,$1.id_sym->name,$1.sym->name);}
					|postfix_expression '(' argument_expression_list_opt ')' {
           $$.sym=currentSymbolTable->gentemp($1.sym->nested_table->lookup("retVal")->type);
            char c[30];
            sprintf(c,"%d",$3);
           quads.emit(Q_FUNCALL,$$.sym->name,$1.sym->name,c); }
				  |postfix_expression '.' IDENTIFIER
				  |postfix_expression ARR IDENTIFIER
				  |postfix_expression INC  
          {
            $$.sym=currentSymbolTable->gentemp(Type($1.sym->type.typ));
            $$.sym->setInitial($$.sym->type.typ,$1.sym->initial);
            quads.emit($$.sym->name,$1.sym->name);
            quads.emit(Q_PLUS,$1.sym->name,$$.sym->name,"1");

          }
          |postfix_expression DEC{
            $$.sym=currentSymbolTable->gentemp(Type($1.sym->type.typ));
            $$.sym->setInitial($$.sym->type.typ,$1.sym->initial);
            quads.emit($$.sym->name,$1.sym->name);
            quads.emit(Q_MINUS,$1.sym->name,$$.sym->name,"1");
          }
				  |'(' type_name ')' '{' initializer_list '}' {}
				  |'(' type_name ')' '{' initializer_list ',' '}'{}
				  ;

array_expression:
            IDENTIFIER '[' expression ']'  {$$.sym=currentSymbolTable->gentemp();
if(currentSymbolTable->exists($1)){
        $$.id_sym=currentSymbolTable->lookup($1);
    }else{
        if(globalSymbolTable.exists($1)){
            $$.id_sym=globalSymbolTable.lookup($1);
        }else{
            $$.id_sym=currentSymbolTable->lookup($1);
        }
    }

 $$.type=&($$.id_sym->type); $$.sym->initial.intval=getsize($$.type); char c[30]; quads.emit(Q_MULT,$$.sym->name,$$.sym->name,$3.sym->name);  $$.type=$$.type->next;}

           |array_expression '[' expression ']' {$$.sym=currentSymbolTable->gentemp();  $$.sym->initial.intval=getsize($$.type); char c[30]; quads.emit(Q_MULT,$$.sym->name,$$.sym->name,$3.sym->name);  $$.type=$$.type->next; quads.emit(Q_PLUS,$$.sym->name,$$.sym->name,$1.sym->name); $$.id_sym=$1.id_sym;}

argument_expression_list_opt:{$$=0;}
						  |argument_expression_list{$$=$1;}
						  ;
argument_expression_list:
            assignment_expression {$$=1; quads.emit(Q_PARAM,$1.sym->name);}
            |argument_expression_list ',' assignment_expression {$$=$1+1; quads.emit(Q_PARAM,$3.sym->name);}
					  ;
unary_expression:
				postfix_expression {$$=$1;}
        |INC unary_expression {
            $$.sym=currentSymbolTable->gentemp(Type($2.sym->type.typ));
            quads.emit(Q_PLUS,$2.sym->name,$2.sym->name,"1");
            quads.emit($$.sym->name,$2.sym->name);
        }
        |DEC unary_expression {
            $$.sym=currentSymbolTable->gentemp(Type($2.sym->type.typ));
            quads.emit(Q_MINUS,$2.sym->name,$2.sym->name,"1");
            quads.emit($$.sym->name,$2.sym->name);
         }
        |unary_operator cast_expression {}
				|SIZEOF unary_expression {}
				|SIZEOF '(' type_name ')' {}
        |'&' cast_expression  
        {
              /*$$=currentSymbolTable->gentemp(Type($2->type.next->typ));*/
              /*quads.emit(Q_DEREF,"c",$2->name);*/
          /*if($2->type.typ==T_POINTER){
            if($2->type.next->typ!=T_POINTER||$2->type.next->typ!=T_ARRAY){
              $$=currentSymbolTable->gentemp(Type(*$2->type.next));
              quads.emit(Q_DEREF,$$->name,$2->name);
            }else{
              $$=currentSymbolTable->gentemp(Type($2->type.next->typ));
            }
          }*/

        }
				;
unary_operator:
			  '*'
			  |'+'
			  |'-'
			  |'~'
			  |'!'
			  ;
cast_expression:
			   unary_expression {$$=$1;}
			   |'(' type_name ')' cast_expression {}
			   ;
multiplicative_expression:
						cast_expression {$$=$1;}
						|multiplicative_expression '*' cast_expression 
            {
              struct symrow *t1;
              struct symrow *t2;
              typecheck($1.sym,$3.sym,&t1,&t2);
                $$.sym=currentSymbolTable->gentemp(Type(t1->type.typ));
                quads.emit(Q_MULT,$$.sym->name,t1->name,t2->name);
 
            }
						|multiplicative_expression '/' cast_expression
            {
              struct symrow *t1;
              struct symrow *t2;
              typecheck($1.sym,$3.sym,&t1,&t2);
                $$.sym=currentSymbolTable->gentemp(Type(t1->type.typ));
                quads.emit(Q_DIVISION,$$.sym->name,t1->name,t2->name);
 
            }
						|multiplicative_expression '%' cast_expression
            {
               $$.sym=currentSymbolTable->gentemp(Type($1.sym->type.typ));
                quads.emit(Q_MODULO,$$.sym->name,$1.sym->name,$3.sym->name);
                
            }
						;
additive_expression:
				   multiplicative_expression {$$=$1;}
				   |additive_expression '+' multiplicative_expression
            { 
              struct symrow *t1;
              struct symrow *t2;
              typecheck($1.sym,$3.sym,&t1,&t2);
                $$.sym=currentSymbolTable->gentemp(Type(t1->type.typ));
                quads.emit(Q_PLUS,$$.sym->name,t1->name,t2->name);
 
            }
				   |additive_expression '-' multiplicative_expression
            { struct symrow *t1;
              struct symrow *t2;
              typecheck($1.sym,$3.sym,&t1,&t2);
                $$.sym=currentSymbolTable->gentemp(Type(t1->type.typ));
                quads.emit(Q_MINUS,$$.sym->name,t1->name,t2->name);
 
            }
				   ;
shift_expression:
				additive_expression {$$=$1;}
				|shift_expression LSH additive_expression
          { 
              struct symrow* t = xtoInt($3.sym);
                $$.sym=currentSymbolTable->gentemp($1.sym->type.typ);
                quads.emit(Q_LSH,$$.sym->name,$1.sym->name,t->name);
          }
				|shift_expression RSH additive_expression
          {
                struct symrow* t = xtoInt($3.sym);
                $$.sym=currentSymbolTable->gentemp($1.sym->type.typ);
                quads.emit(Q_RSH,$$.sym->name,$1.sym->name,t->name);

            }
				;
relational_expression:
					 shift_expression {$$=$1;}
					 |relational_expression '<' shift_expression
          { 
           
          
              /*if(typecheck($1.sym,$3)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                $$.truelist=makelist(quads.size);
                $$.falselist=makelist(quads.size+1);
                quads.emit(Q_REL_IFLT,-1,$1.sym->name,$3.sym->name);
                quads.emit(Q_GOTO,-1);
                $$.isBexp=true;
              /*}*/
          }
					 |relational_expression '>' shift_expression
          { 
           
          
              /*if(typecheck($1.sym,$3)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                $$.truelist=makelist(quads.size);
                $$.falselist=makelist(quads.size+1);

                quads.emit(Q_REL_IFGT,-1,$1.sym->name,$3.sym->name);
                quads.emit(Q_GOTO,-1);
                $$.isBexp=true;
                
              /*}*/
          }
					 |relational_expression RANGB_EQ shift_expression
           { 
              /*if(typecheck($1.sym,$3)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                $$.truelist=makelist(quads.size);
                $$.falselist=makelist(quads.size+1);
                quads.emit(Q_REL_IFGTE,-1,$1.sym->name,$3.sym->name);
                quads.emit(Q_GOTO,-1);
                $$.isBexp=true;
              /*}*/
            }
					 |relational_expression LANGB_EQ shift_expression
           { 
              /*if(typecheck($1.sym,$3)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                $$.truelist=makelist(quads.size);
                $$.falselist=makelist(quads.size+1);

                quads.emit(Q_REL_IFLTE,-1,$1.sym->name,$3.sym->name);
                quads.emit(Q_GOTO,-1);
                $$.isBexp=true;
              /*}*/
            }
					 ;
equality_expression:
				   relational_expression {$$=$1;}
				   |equality_expression DOUBLE_EQ relational_expression
           { 
              /*if(typecheck($1.sym,$3.sym)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                $$.truelist=makelist(quads.size);
                $$.falselist=makelist(quads.size+1);
                quads.emit(Q_REL_IFEQ,-1,$1.sym->name,$3.sym->name);
                quads.emit(Q_GOTO,-1);
                $$.isBexp=true;
              /*}*/
            }
				   |relational_expression EXCL_EQ relational_expression
           { 
              /*if(typecheck($1.sym,$3.sym)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                $$.truelist=makelist(quads.size);
                $$.falselist=makelist(quads.size+1);
                quads.emit(Q_REL_IFNEQ,-1,$1.sym->name,$3.sym->name);
                quads.emit(Q_GOTO,-1);
                $$.isBexp=true;
              /*}*/
            }
				   ;
AND_expression:
			  equality_expression {$$=$1;}
			  |AND_expression '&' equality_expression
          { 
              /*if(typecheck($1.sym,$3.sym)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                quads.emit(Q_AMPERSAND,$$.sym->name,$1.sym->name,$3.sym->name);
              /*}*/
            }
			  ;
exclusive_OR_expression:
					   AND_expression {$$=$1;}
					   |exclusive_OR_expression '^'  AND_expression
            { 
              /*if(typecheck($1.sym,$3.sym)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                quads.emit(Q_XOR,$$.sym->name,$1.sym->name,$3.sym->name);
              /*}*/
            }
					   ;
inclusive_OR_expression:
					   exclusive_OR_expression {$$=$1;}
					   |inclusive_OR_expression '|' exclusive_OR_expression
            { 
              /*if(typecheck($1.sym,$3.sym)){*/
                $$.sym=currentSymbolTable->gentemp(Type());
                quads.emit(Q_AROR,$$.sym->name,$1.sym->name,$3.sym->name);
              /*}*/
            }
					   ;
logical_AND_expression:
					  inclusive_OR_expression {$$=$1;}
					  |logical_AND_expression AND M inclusive_OR_expression M
            {
              /*printf("iiiiiiiiiiiiiiiiiiiiiiiiiiiiii %d %d %d",$3,$5,$1.isBexp);*/
              if(!($1.isBexp)){
                xtobool(&$1);
                backpatch($1.truelist,$5+2);
              }else{
                backpatch($1.truelist,$3);
              }
              xtobool(&$4);
              $$.truelist=$4.truelist;
              $$.falselist=merge($1.falselist,$4.falselist);
              $$.isBexp=true;
            }
					  ;
logical_OR_expression:
					 logical_AND_expression {$$=$1;}
					 |logical_OR_expression OR M logical_AND_expression M
          {
              /*if(!($1.isBexp)){
                xtobool(&$1);
                backpatch($1.falselist,$5+2);
              }else{
                backpatch($1.falselist,$5);
              }
              xtobool(&$4);*/
              backpatch($1.falselist,$3);
              $$.falselist=$4.falselist;
              $$.truelist=merge($1.truelist,$4.truelist);
              $$.isBexp=true;
          }
					 ;
conditional_expression:
					  logical_OR_expression {$$=$1;}
					  |logical_OR_expression N '?' M expression N ':' M conditional_expression{}
					  ;

assignment_expression_opt: {}
						 |assignment_expression {$$=$1;}
						 ;

assignment_expression:
					 conditional_expression {$$=$1;}
					 |unary_expression assignment_operator assignment_expression
            { $$.sym=$1.sym;
              if($3.isBexp){
                  backpatch($3.truelist,quads.size);
                  backpatch($3.falselist,quads.size+1);
                  quads.emit($1.sym->name,TRUE_VAL); 
                  quads.emit($1.sym->name,FALSE_VAL); 
              }else{

              struct symrow *t =$3.sym;
              if($1.sym->type.typ==T_INT){
                t=xtoInt($3.sym);
              }
              if($1.sym->type.typ==T_CHAR){
                t=xtoChar($3.sym);
              }
              if($1.sym->type.typ==T_DOUBLE){
                t=xtoDouble($3.sym);
              }
              quads.emit($1.sym->name,t->name);
            }
          }
           |array_expression assignment_operator assignment_expression 
           {
              if($3.isBexp){
                  backpatch($3.truelist,quads.size);
                  backpatch($3.falselist,quads.size+1);
                  quads.emit(Q_ARR_COPY,$1.id_sym->name,$1.sym->name,"TRUE_VAL"); 
                  quads.emit(Q_ARR_COPY,$1.id_sym->name,$1.sym->name,"FALSE_VAL"); 
                }
                else
                  quads.emit(Q_ARR_COPY,$1.id_sym->name,$1.sym->name,$3.sym->name); 
           }
					 ;
assignment_operator:
				   '='
				   |MULT_ASSIGN
				   |DIV_ASSIGN
				   |MOD_ASSIGN
				   |ADD_ASSIGN
				   |SUB_ASSIGN
				   |RSH_ASSIGN
				   |LSH_ASSIGN
				   |BIXOR_ASSIGN
				   |BIAND_ASSIGN
				   |BIOR_ASSIGN
				   ;
expression_opt:{$$.sym=NULL;}
			  |expression {$$=$1;}
			  ;

booexpression: expression {xtobool(&$1); $$=$1;}
             ;
booexpression_opt: {}
                 |booexpression
              ;


expression:
		  assignment_expression {$$=$1;}
		  |expression ',' assignment_expression
		  ;
constant_expression:
				   conditional_expression
				   ;


declaration:
		   declaration_specifiers init_declarator_list_opt ';' {}
		   ;
declaration_specifiers_opt:
						  |declaration_specifiers
						  ;

declaration_specifiers:
					  storage_class_specifier declaration_specifiers_opt
					  |type_specifier declaration_specifiers_opt
					  |type_qualifier declaration_specifiers_opt
					  |function_specifier declaration_specifiers_opt
					  ;
init_declarator_list_opt: {}
						|init_declarator_list
						;


init_declarator_list:
				init_declarator 
				|init_declarator_list ',' init_declarator
				;

init_declarator:
			   declarator {$$=$1;}
         |declarator '=' initializer {
          $$=$1;
          /*printf("xxxxxxxxx%dxxxxxxxxxx%lf",$3->type.typ,$3->initial.doubleval);*/
              if($3.isBexp){
                  backpatch($3.truelist,quads.size);
                  backpatch($3.falselist,quads.size+1);
                  quads.emit($1->name,TRUE_VAL); 
                  quads.emit($1->name,FALSE_VAL); 
              }else{

              struct symrow *t =$3.sym;
              if($1->type.typ==T_INT){
                t=xtoInt($3.sym);
              }
              if($1->type.typ==T_CHAR){
                t=xtoChar($3.sym);
              }
              if($1->type.typ==T_DOUBLE){
                t=xtoDouble($3.sym);
              }
              quads.emit($1->name,t->name);
            }
          /*if(typecheck($1,$3.sym)){
            if($3.isBexp){
                  backpatch($3.truelist,quads.size);
                  backpatch($3.falselist,quads.size+1);
                  quads.emit($1->name,TRUE_VAL); 
                  quads.emit($1->name,FALSE_VAL); 
                }
            else
              quads.emit($1->name,$3.sym->name);*/
            $$=$1;
          
          /*$$->setInitial($3->type.typ,$3->initial);*/

            /*switch($3.type){
              case T_INT:
                [>printf("kkkkkkkkkkkkk%d",$1.ival);<]
                $$->initial.intval=$3.ival;
                break;
              case T_DOUBLE:
                $$->initial.doubleval=$1.dval;
                break;
              case T_CHAR:
                $$->initial.charval=$1.cval;
                break;
              default:
                break;
            }*/
        
          }
			   ;
storage_class_specifier:
					   EXTERN
					   |STATIC
					   |AUTO
					   |REGISTER
					   ;
type_specifier:
			  VOID {$$=lastType = T_VOID;}
			  |CHAR {$$=lastType = T_CHAR;}
			  |SHORT {}
			  |INT {$$=lastType = T_INT; }
			  |LONG {}
			  |DOUBLE {$$=lastType = T_DOUBLE;}
			  |SIGNED {}
			  |UNSIGNED {}
			  |BOOL {}
			  |COMPLEX {}
			  |IMAGINARY {}
			  |enum_specifier {}
			  ;
specifier_qualifier_list_opt:
							|specifier_qualifier_list
							;


specifier_qualifier_list:
						type_specifier specifier_qualifier_list_opt
						|type_qualifier specifier_qualifier_list_opt
						;
enum_specifier:
			  ENUM identifier_opt '{' enumerator_list '}'
			  |ENUM identifier_opt '{' enumerator_list ',' '}'
			  |ENUM IDENTIFIER
			  ;
enumerator_list:
			   enumarator
			   | enumerator_list ',' enumarator
			   ;
enumarator:
		  ENUMARATION_CONSTANT
		  |ENUMARATION_CONSTANT '=' constant_expression
		  ;
type_qualifier:
			  CONST
			  |RESTRICT
			  |VOLATILE
			  ;
function_specifier:
				  INLINE
				  ;
declarator:
			pointer_opt direct_declarator { $2->makePointer($1);/*pointed=$1;*/ $$=$2;}
		  ;
direct_declarator:
				   IDENTIFIER { $$=currentSymbolTable->lookup(yytext); }
				   |'(' declarator ')' {}
				   |direct_declarator '[' type_qualifier_list_opt assignment_expression_opt ']'
           {
              int result;
              switch($4.sym->type.typ){
                case T_CHAR:
                  result=(int)$4.sym->initial.charval;
                  break;
                case T_DOUBLE:
                  result=(int)$4.sym->initial.doubleval;
                  break;
                default:
                  result=$4.sym->initial.intval;
                  break;
              }
              $$=currentSymbolTable->lookup($1->name);
              $1->makeArray(result,currentSymbolTable);
            }
				   |direct_declarator '[' STATIC type_qualifier_list_opt assignment_expression ']'
				   |direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
				   |direct_declarator '[' type_qualifier_list_opt '*' ']'
				   |direct_declarator '(' funcdecstart parameter_type_list funcdecend ')' {
               if($1->type.typ==T_FUNCTION){lastSymbolTable=$1->nested_table;} 
                else {$1->makeFunction($3); }lastFunction=$1->name;}
				   |direct_declarator '(' identifier_list ')' {  }
				   |direct_declarator '('funcdecstart funcdecend  ')' { if($1->type.typ==T_FUNCTION){lastSymbolTable=$1->nested_table;} 
                else {$1->makeFunction($3); }lastFunction=$1->name; }
				   ;

funcdecstart:
             {$$=currentSymbolTable=new Symboltable();};
funcdecend: 
          {lastSymbolTable=currentSymbolTable; currentSymbolTable = &globalSymbolTable;};
pointer_opt: {$$=0;  }
		   |pointer {  $$=$1;  }
		   ;

pointer:
	   '*' type_qualifier_list_opt  {$$=1; }
	   |'*' type_qualifier_list_opt pointer {$$=$3+1;} 
	   ;
type_qualifier_list_opt:
					   |type_qualifier_list
					   ;


type_qualifier_list:
				   type_qualifier
				   |type_qualifier_list type_qualifier
				   ;
parameter_type_list:
				   parameter_list
				   |parameter_list ',' ELLIPSIS
				   ;
parameter_list:
        parameter_declaration { currentSymbolTable->params=1; }
        |parameter_list ',' parameter_declaration { currentSymbolTable->params++; }
			  ;
parameter_declaration:
					 declaration_specifiers declarator
					 |declaration_specifiers
					 ;
/*
identifier_list_opt:
				   |identifier_list
				   ;
*/


identifier_list:
			   IDENTIFIER
			   |identifier_list ',' IDENTIFIER
			   ;
type_name:
		 specifier_qualifier_list
		 ;
initializer:
		   assignment_expression {$$=$1;}
		   |'{' initializer_list '}' {}
		   |'{' initializer_list ',' '}' {}
		   ;
initializer_list:
				designation_opt initializer
				|initializer_list ',' designation_opt initializer
				;
designation_opt:
		   |designation
		   ;
designation:
		   designator_list '='
		   ;

designator_list:
			   designator 
			   |designator_list designator
			   ;
designator:
		  '[' constant_expression ']'
		  |'.' IDENTIFIER
		  ;



statement:
		 labeled_statement
		 |compound_statement
		 |expression_statement
		 |selection_statement
		 |iteration_statement
		 |jump_statement
		 ;
labeled_statement:
				 IDENTIFIER ':' statement {}
				 |CASE constant_expression ':' statement {}
				 |DEFAULT ':' statement {}
				 ;
compound_statement:
				  '{' block_item_list_opt '}' {$$=$2;}
				  ;
block_item_list_opt:{$$=new vector<int>();}
				   |block_item_list
				   ;


block_item_list:
			   block_item {$$=$1;}
			   |block_item_list M block_item { backpatch($1,$2); $$ = $3;}
			   ;

block_item:
		  declaration {$$=new vector<int>();}
		  |statement {$$=$1;}
		  ;
expression_statement:
					expression_opt ';' { $$=new vector<int>(); }
					;
selection_statement:
				   IF '(' booexpression ')' M statement   N  %prec THEN { backpatch($3.truelist,$5); $$=merge($3.falselist,$6); quads.size--;}
				   |IF '(' booexpression ')'  M statement N ELSE M statement
           {
              backpatch($3.truelist, $5);
              backpatch($3.falselist, $9);
              vector<int>* foo = merge($6,$7);
              $$=merge(foo,$10);
  
            }
				   |SWITCH '(' expression ')' statement {}
				   ;
iteration_statement:
				   WHILE M '(' booexpression ')' M statement {


backpatch($7,$2); backpatch($4.truelist,$6); $$=$4.falselist; quads.emit(Q_GOTO,$2); }
				   |DO M statement M WHILE '(' booexpression ')' ';' {backpatch($7.truelist,$2); backpatch($3,$4); $$=$7.falselist;}
				   |FOR '(' expression_opt ';'  M booexpression_opt ';' M expression_opt N ')' M statement 
   { backpatch($6.truelist,$12); backpatch($10,$5); backpatch($13,$8); quads.emit(Q_GOTO, $8); $$=$6.falselist;  }
				   |FOR '(' declaration expression_opt ';' expression_opt ')' statement  {}
				   ;
jump_statement:
			  GOTO IDENTIFIER ';' {}
			  |CONTINUE ';' {}
			  |BREAK ';' {}
        |RETURN expression_opt ';' {$$ = new vector<int>(); char c[1];c[0]='\0'; if($2.sym!=NULL){ quads.emit(Q_RET, $2.sym->name); }else{ quads.emit(Q_RET, c); }}
			  ;



translation_unit:
				external_declaration
				|translation_unit external_declaration
				;
external_declaration:
					function_definition
					|declaration
					;
function_definition:
           declaration_specifiers declarator  declaration_list_opt temp1 compound_statement M temp2 {backpatch($5,$6);}
				   ;

temp1:{currentSymbolTable=lastSymbolTable; quads.emit(Q_FUNCSTART,lastFunction);};

temp2:{currentSymbolTable=&globalSymbolTable;  quads.emit(Q_FUNCEND,lastFunction);};

declaration_list_opt:
					|declaration_list
					;


declaration_list:
				declaration
				|declaration_list declaration
				;
M: {$$=quads.size;};
N: { $$=makelist(quads.size); quads.emit(Q_GOTO,-1);};
		  

%%
void yyerror(char *s) {
	printf("%s\n", s);
}
