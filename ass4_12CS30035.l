%{
#include "ass4_12CS30035_translator.h"
#include "y.tab.h"
%}

NONDIGIT [_a-zA-Z]
DIGIT [0-9]
DIGITS {DIGIT}+
INT_CONST ({NONZERO_DIGIT}({DIGIT}*))|0
NONZERO_DIGIT [1-9]
IDENTIFIER {NONDIGIT}(({DIGIT}|{NONDIGIT})*)

SIGN (\+|-)
EPART  (E|e){SIGN}?{DIGITS}
FRACT_CONST ({DIGITS}?\.{DIGITS})|({DIGITS}\.)
DOUBLE_CONST  (({DIGITS}{EPART})|({FRACT_CONST}{EPART}?))

CHAR_CONST '{CCHAR}'
ESC_SEQ \\['"?\\abfnrtv]
CCHAR [^'\\\n]|{ESC_SEQ}


STR_LITERAL \"{SCHARSEQ}?\"
SCHAR [^"\\\n]|{ESC_SEQ}
SCHARSEQ	{SCHAR}+

MULT_COMM \/\*([^*]|\*+[^/*])*\*+\/		
SING_COM	"//".*\n					 
ENUM_CONST 8982398


%%
"auto"			{ return(AUTO); }
"break"			{ return(BREAK); }
"case"			{ return(CASE); }
"char"			{ return(CHAR); }
"const"			{ return(CONST); }
"continue"		{ return(CONTINUE); }
"default"		{ return(DEFAULT); }
"do"			{ return(DO); }
"double"		{ return(DOUBLE); }
"else"			{ return(ELSE); }
"enum"			{ return(ENUM); }
"extern"		{ return(EXTERN); }
"for"			{ return(FOR); }
"goto"			{ return(GOTO); }
"inline"		{ return(INLINE); }
"if"			{ return(IF); }
"int"			{ return(INT); }
"long"			{ return(LONG); }
"register"		{ return(REGISTER); }
"restrict"		{ return(RESTRICT); }
"return"		{ return(RETURN); }
"short"			{ return(SHORT); }
"signed"		{ return(SIGNED); }
"sizeof"		{ return(SIZEOF); }
"static"		{ return(STATIC); }
"struct"		{ return(STRUCT); }
"switch"		{ return(SWITCH); }
"typedef"		{ return(TYPEDEF); }
"union"			{ return(UNION); }
"unsigned"		{ return(UNSIGNED); }
"void"			{ return(VOID); }
"volatile"		{ return(VOLATILE); }
"while"			{ return(WHILE); }
"_Bool"			{ return(BOOL); }
"_Complex"		{ return(COMPLEX); }
"_Imaginary"	{ return(IMAGINARY); }
{IDENTIFIER}    {  strcpy(yylval.charArray,yytext);/*yylval.symRow=currentSymbolTable->lookup(yytext);*/  return(IDENTIFIER); }
{ENUM_CONST}    { return(ENUMARATION_CONSTANT); }
{INT_CONST}     { yylval.intValue = atoi( yytext ); return(INT_CONST); }
{DOUBLE_CONST}   { yylval.doubleValue = atof( yytext ); return(DOUBLE_CONST); }
{CHAR_CONST}   { yylval.charValue =  yytext[1]; return(CHAR_CONST); }
{STR_LITERAL}   { yylval.stringValue =  yytext; return(STR_LITERAL); }

			
"["  { return('['); }
"]"  { return(']'); }
"("  { return('('); }
")"  { return(')'); }
"{"  { return('{'); }
"}"  { return('}'); }
"."  { return('.'); }
"->"  { return(ARR); }
"++"  { return(INC); }
"--"  { return(DEC); }
"&"  { return('&'); }
"*"  { return('*'); }
"+"  { return('+'); }
"-"  { return('-'); }
"~"  { return('~'); }
"!"  { return('!'); }
"/"  { return('/'); }
"%"  { return('%'); }
">>"  { return(RSH); }
"<<"  { return(LSH); }
"<"  { return('<'); }
">"  { return('>'); }
">="  { return(RANGB_EQ); }
"<="  { return(LANGB_EQ); }
"=="  { return(DOUBLE_EQ); }
"!="  { return(EXCL_EQ); }
"^"  { return('^'); }
"|"  { return('|'); }
"||"  { return(OR); }
"&&"  { return(AND); }
"?"  { return('?'); }
":"  { return(':'); }
";"  { return(';'); }
"..."  { return(ELLIPSIS); }
"="  { return('='); }
"*="  { return(MULT_ASSIGN); }
"/="  { return(DIV_ASSIGN); }
"%="  { return(MOD_ASSIGN); }
"+="  { return(ADD_ASSIGN); }
"-="  { return(SUB_ASSIGN); }
"<<="  { return(LSH_ASSIGN); }
">>="  { return(RSH_ASSIGN); }
"&="  { return(BIAND_ASSIGN); }
"^="  { return(BIXOR_ASSIGN); }
"|="  { return(BIOR_ASSIGN); }
","  { return(','); }
"#"  { return('#'); }
{MULT_COMM} {  }
{SING_COM} {  }

%%


