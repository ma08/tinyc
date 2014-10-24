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
}
%union
{
  struct constant_val conval;
  enum Tp typeValue;
  int intValue; 
  double doubleValue;
  char charValue;
  char *stringValue;
  struct symrow* symRow;
}

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token CHAR SHORT INT LONG SIGNED UNSIGNED DOUBLE CONST VOLATILE VOID
%token RESTRICT ENUM UNION BOOL IMAGINARY COMPLEX SIZEOF STRUCT
%token ENUMARATION_CONSTANT

%token <symRow> IDENTIFIER

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


%start translation_unit
/*%type <symRow> assignment_expression*/

%type <symRow> primary_expression postfix_expression unary_expression cast_expression
%type <symRow> multiplicative_expression additive_expression shift_expression relational_expression equality_expression
%type <symRow> AND_expression exclusive_OR_expression inclusive_OR_expression logical_AND_expression logical_OR_expression
%type <symRow> conditional_expression assignment_expression assignment_expression_opt initializer
%type <symRow> init_declarator_list init_declarator_list_opt expression

%type <symRow> declaration init_declarator
%type <symRow> declarator direct_declarator 

%%
identifier_opt:
			  |IDENTIFIER
			  ;
	
primary_expression:
				  IDENTIFIER {$$=$1;}
          |constant
         {
            $$=currentSymbolTable->gentemp(Type($1.type));

            /*printf("kkkkkkkkkkkkkaaaa%d",$1.type);*/
            switch($1.type){
              case T_INT:
                /*printf("kkkkkkkkkkkkk%d",$1.ival);*/
                $$->initial.intval=$1.ival;
                break;
              case T_DOUBLE:
                $$->initial.doubleval=$1.dval;
                break;
              case T_CHAR:
                $$->initial.charval=$1.cval;
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
				  primary_expression {$$=$1;}
				  |postfix_expression '[' expression ']'
				  |postfix_expression '(' argument_expression_list_opt ')'
				  |postfix_expression '.' IDENTIFIER
				  |postfix_expression ARR IDENTIFIER
				  |postfix_expression INC 
				  |postfix_expression DEC
				  |'(' type_name ')' '{' initializer_list '}' {}
				  |'(' type_name ')' '{' initializer_list ',' '}'{}
				  ;
argument_expression_list_opt:
						  |argument_expression_list
						  ;
argument_expression_list:
					  assignment_expression
					  |argument_expression_list ',' assignment_expression
					  ;
unary_expression:
				postfix_expression {$$=$1;}
				|INC unary_expression {}
				|DEC unary_expression {}
				|unary_operator cast_expression {}
				|SIZEOF unary_expression {}
				|SIZEOF '(' type_name ')' {}
				;
unary_operator:
			  '&'
			  |'*'
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
						|multiplicative_expression '/' cast_expression
						|multiplicative_expression '%' cast_expression
						;
additive_expression:
				   multiplicative_expression {$$=$1;}
				   |additive_expression '+' multiplicative_expression
				   |additive_expression '-' multiplicative_expression
				   ;
shift_expression:
				additive_expression {$$=$1;}
				|shift_expression LSH additive_expression
				|shift_expression RSH additive_expression
				;
relational_expression:
					 shift_expression {$$=$1;}
					 |relational_expression '<' shift_expression
					 |relational_expression '>' shift_expression
					 |relational_expression RANGB_EQ shift_expression
					 |relational_expression LANGB_EQ shift_expression
					 ;
equality_expression:
				   relational_expression {$$=$1;}
				   |equality_expression DOUBLE_EQ relational_expression
				   |relational_expression EXCL_EQ relational_expression
				   ;
AND_expression:
			  equality_expression {$$=$1;}
			  |AND_expression '&' equality_expression
			  ;
exclusive_OR_expression:
					   AND_expression {$$=$1;}
					   |exclusive_OR_expression '^' M AND_expression
					   ;
inclusive_OR_expression:
					   exclusive_OR_expression {$$=$1;}
					   |inclusive_OR_expression '|' exclusive_OR_expression
					   ;
logical_AND_expression:
					  inclusive_OR_expression {$$=$1;}
					  |logical_AND_expression AND M inclusive_OR_expression
					  ;
logical_OR_expression:
					 logical_AND_expression {$$=$1;}
					 |logical_OR_expression OR M logical_AND_expression
					 ;
conditional_expression:
					  logical_OR_expression {$$=$1;}
					  |logical_OR_expression N '?' M expression N ':' M conditional_expression
					  ;

assignment_expression_opt: {}
						 |assignment_expression {$$=$1;}
						 ;

assignment_expression:
					 conditional_expression {$$=$1;}
					 |unary_expression assignment_operator assignment_expression
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
expression_opt:
			  |expression
			  ;

expression:
		  assignment_expression
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
          $$->setInitial($3->type.typ,$3->initial);
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
              switch($4->type.typ){
                case T_CHAR:
                  result=(int)$4->initial.charval;
                  break;
                case T_DOUBLE:
                  result=(int)$4->initial.doubleval;
                  break;
                default:
                  result=$4->initial.intval;
                  break;
              }
              $$=currentSymbolTable->lookup($1->name);
              $1->makeArray(result);
            }
				   |direct_declarator '[' STATIC type_qualifier_list_opt assignment_expression ']'
				   |direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
				   |direct_declarator '[' type_qualifier_list_opt '*' ']'
				   |direct_declarator '(' parameter_type_list ')'
				   |direct_declarator '(' identifier_list_opt ')'
				   ;
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
			  parameter_declaration
			  |parameter_list ',' parameter_declaration
			  ;
parameter_declaration:
					 declaration_specifiers declarator
					 |declaration_specifiers
					 ;
identifier_list_opt:
				   |identifier_list
				   ;


identifier_list:
			   IDENTIFIER
			   |identifier_list ',' IDENTIFIER
			   ;
type_name:
		 specifier_qualifier_list
		 ;
initializer:
		   assignment_expression
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
				 IDENTIFIER ':' statement
				 |CASE constant_expression ':' statement
				 |DEFAULT ':' statement
				 ;
compound_statement:
				  '{' block_item_list_opt '}'
				  ;
block_item_list_opt:
				   |block_item_list
				   ;


block_item_list:
			   block_item 
			   |block_item_list block_item
			   ;

block_item:
		  declaration
		  |statement
		  ;
expression_statement:
					expression_opt ';'
					;
selection_statement:
				   IF '(' expression ')' M statement
				   |IF '(' expression ')'  M statement N ELSE M statement
				   |SWITCH '(' expression ')' statement
				   ;
iteration_statement:
				   WHILE M '(' expression ')' M statement
				   |DO M statement M WHILE '(' expression ')' ';'
				   |FOR '(' expression_opt ';'  M expression_opt ';' M expression_opt N ')' M statement 
				   |FOR '(' declaration expression_opt ';' expression_opt ')' statement 
				   ;
jump_statement:
			  GOTO IDENTIFIER ';'
			  |CONTINUE ';'
			  |BREAK ';'
			  |RETURN expression_opt ';'
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
				   declaration_specifiers declarator declaration_list_opt compound_statement
				   ;

declaration_list_opt:
					|declaration_list
					;


declaration_list:
				declaration
				|declaration_list declaration
				;
M:;
N:;
		  

%%
void yyerror(char *s) {
	printf("%s\n", s);
}
