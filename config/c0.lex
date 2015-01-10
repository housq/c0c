/*
* c0.lex
*/
%{
#include "common.h"
#include "c0.tab.h"


%}

%x COMMENT1 COMMENT2
digit   [0-9]
digitn  [1-9]
letter  [A-Za-z_]
hex_alpha [A-Fa-f0-9]
ident   {letter}({letter}|{digit})*
number  {digitn}{digit}*|0
hex_num  0[xX]{hex_alpha}+
octal_num 0[0-7]+
COMMENT_end   [^*/]
COMMENT		"/*"[/]*([*]*{COMMENT_end}+[/]*)*[*]*"*/"

%%
[\t ]*             {ECHO;/*return(EOL);*/}
[\n]              {ECHO;/*yycolumn = 1;*/}
{number}          { yylval.val= atol(yytext);
                     ECHO;return(NUMBER);}
{hex_num}         { sscanf(yytext,"%d",&yylval.val);
                    ECHO;return (NUMBER);}
{octal_num}       { sscanf(yytext,"%d", &yylval.val);
                    ECHO;return(NUMBER);}
"+"                {yylval.val=OP_PLUS;ECHO;return(PLUS);}
"-"                {yylval.val=OP_MINUS;ECHO;return(MINUS);}
"*"                {yylval.val=OP_MULT;ECHO;return(MULT);}
"/"                {yylval.val=OP_DIV;ECHO;return(DIV);}
"("                {ECHO;return(LP);}
")"                {ECHO;return(RP);}
"{"                {ECHO;return(LB);}
"}"                {ECHO;return(RB);}
"main"             {ECHO;return(MAIN);}
"void"             {ECHO;return(VOID);}
"int"              {ECHO;return(INT);}
"const"            {ECHO;return(CONST);}
"if"               {ECHO;return(IF);}
"while"            {ECHO;return(WHILE);}
{ident}            {yylval.name = malloc(yyleng+1);
                   strncpy(yylval.name, yytext, yyleng+1);
                    ECHO;return(IDENT);}
"="                {yylval.val=OP_ASSIGN;ECHO;return(ASSIGN);}
"=="               {yylval.val=OP_EQL;ECHO;return(EQL);}
">"                {yylval.val=OP_GRE;ECHO;return(GRE);}
">="               {yylval.val=OP_GREQ;ECHO;return(GREQ);}
"<"                {yylval.val=OP_LES;ECHO;return(LES);}
"<="               {yylval.val=OP_LEQ;ECHO;return(LEQ);}
"!="               {yylval.val=OP_NEQ;ECHO;return(NEQ);}
","                {ECHO;return(COMMA);}
";"                {ECHO;return(SEMI);}
{COMMENT}	   {ECHO;}
.                  { yyerror("Illegal character");
                   }
%%
