/*
  c0.y
*/
%{
 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <common.h>
void yyerror(char *);
int level=0;
int prev_offset=0;
int offset=0;
 Table symtab;
 ASTTree ast;
%}
%union {
   int val;
   char *name;
   ASTNode node;
}
%token MAIN CONST INT VOID IF WHILE 
%token IDENT
%token NUMBER
%token EQL GRE GREQ LES LEQ NEQ
%token PLUS MINUS MULT DIV 
%token LP RP
%token LB RB
%token ASSIGN
/*%token EOL*/
%token COMMA SEMI

%left MINUS PLUS
%left MUIT DIV
%right UMINUS
/*%right EXPON*/
%type <val>NUMBER
%type <name>IDENT
%type <val> PLUS MINUS MULT DIV ASSIGN
%type <val> EQL GRE GREQ LES LEQ NEQ
%type <node> program constdecl vardecl procdecl block
%type <node> constdef ex_constdef vardef ex_vardef procdef 
%type <node> stmts stmt cond exp  term factor

%start goal

%%
goal    :
         {
            printf("//开始分析：\n");
            symtab=newTable();
	     level=0;
             prev_offset=0;
             offset=0;
	    
         }
           program
          {
             ast->root=$2;
             
             
          }  
	| error
	 ;  
program : constdecl vardecl procdecl VOID MAIN LP RP LB
          {
              level++;
              prev_offset=offset;
          }
         block RB
        {  
           printf("//程序分析\n");
           $$=newprogram($1,$2,$3,$10);
           enterfunc(symtab,"main",0,(offset-prev_offset)*4);
           level--;
        }
        ;
constdecl: CONST INT constdef ex_constdef SEMI
         {
            addFirst($4->constdecl->constdef,$3);
             $$=$4;
         }
         |
          {$$=newConstdecl();
            setLoc($$, (Loc)&(@$));}
         ;
constdef : IDENT ASSIGN NUMBER
           {
              printf("	//常量分析\n");
              $$=newConstdef(symtab,$1,$3);
              setLoc($$, (Loc)&(@$));
              enterconst(symtab,$1,$3,level,0);
	      
           }
         ;
ex_constdef : ex_constdef COMMA constdef
              {
               addLast($1->constdecl->constdef,$3);
               $$=$1;
		setLoc($$, (Loc)&(@$));
              }
            | 
              { $$=newConstdecl();}
            ;

vardecl  : INT vardef ex_vardef SEMI
            {
              addFirst($3->vardecl->vardef,$2);
              $$=$3;
	      setLoc($$, (Loc)&(@$));
            }
         |
             {$$=newVardecl();}
         ;
vardef   : IDENT ASSIGN NUMBER
           {
             $$=newVardef(symtab,1,$1,$3);
	     setLoc($$, (Loc)&(@$));
             entervar(symtab,$1,$3,1,level,offset-prev_offset);
	     offset++;
           }
         | IDENT
            {
                 $$=newVardef(symtab,0,$1,0);
                 setLoc($$, (Loc)&(@$));
                entervar(symtab,$1,0,2,level,offset-prev_offset);
		offset++;
            }
         ;
ex_vardef : ex_vardef COMMA vardef 
             {
              addLast($1->vardecl->vardef,$3);
              $$=$1;
              setLoc($$, (Loc)&(@$));
              }

          |
              {$$=newVardecl();}
          ;

procdecl  : procdecl procdef
           { addLast($1->procdecl->procdef,$2);
              $$=$1;
              setLoc($$, (Loc)&(@$));
             //printf("%d",listsize($1->procdecl->procdef));
           }
          |
           { $$=newProcDecl();}
          ;
procdef   : VOID IDENT LP RP LB 
            {
                level++;
                prev_offset=offset;
		printf("\n\n //prev_offset %d\n\n",prev_offset);
		
             }
            block RB
            {
		
               $$= newprocdef($2,$7);
               setLoc($$, (Loc)&(@$));
               printf("\n\n //offset %d\n\n",offset);
               enterfunc(symtab,$2,offset,(offset-prev_offset));
               offset++;
	      level--;
               
             }
          ;
block     : constdecl vardecl stmts
           { $$=newblock($1,$2,$3);setLoc($$, (Loc)&(@$));}
          ;      
stmts     : stmts stmt
            {addLast($1->stmts->stmts,$2);
             $$=$1;
	     setLoc($$, (Loc)&(@$));
             }
          |
            { $$= newstmts();}
          ;
stmt      :IDENT ASSIGN exp SEMI 
            {
             ASTNode temp=newName(symtab,$1);
             $$=newAssignment($2,temp,$3);
	     setLoc($$, (Loc)&(@$));
            }
          |IDENT LP RP SEMI
            { $$=newcall(symtab,$1);setLoc($$, (Loc)&(@$));}
          |LB stmts RB
            {$$=$2; setLoc($$, (Loc)&(@$));}
          |IF LP cond RP stmt
      	     {$$=newif($3,$5);setLoc($$, (Loc)&(@$));}
          |WHILE LP cond RP stmt
             { $$=newwhile($3,$5);setLoc($$, (Loc)&(@$));}
          |SEMI
            {$$=newNull();setLoc($$, (Loc)&(@$));}
          ;

cond      : exp EQL exp
            {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
          | exp GRE exp
            {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
          | exp GREQ exp
             {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
          | exp LES exp
             {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
          | exp LEQ exp
            { $$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
          | exp NEQ exp
            {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
          ;
/*relop     :EQL
          |GRE
          |GREQ
          |LES
          |LEQ
          |NEQ
          ;*/
exp     : exp PLUS  term   
          {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
        | exp MINUS term 
           {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}   
        | term
          { $$=$1; setLoc($$,(Loc)&(@$));}
	| PLUS exp %prec UMINUS
          {$$=newPrefixExp($1,$2); setLoc($$,(Loc)&(@$));}
	| MINUS exp %prec UMINUS 
          { $$=newPrefixExp($1,$2); setLoc($$,(Loc)&(@$));}
        ;
term    : term MULT factor
          {$$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
        | term DIV  factor
          { $$=newInfixExp($2,$1,$3); setLoc($$,(Loc)&(@$));}
        | factor
          {$$=$1; setLoc($$,(Loc)&(@$));}
        ;


factor    : IDENT
            {$$=newName(symtab,$1); setLoc($$,(Loc)&(@$));}   
          | NUMBER
             {$$=newNumber(symtab,$1); setLoc($$,(Loc)&(@$));} 
          | LP exp RP  
            {$$=newParentExp($2); setLoc($$,(Loc)&(@$));}
          ;
%%

void yyerror(char *message)
{
  printf ("%s\n",message);
}
 
int main(int argc ,char *argv[])
{
   symtab=newTable();
   ast=newAST();
   printf("Parsing\n");
   fprintf(stdout, "%d\n", yyparse());
  // fprintf(stderr, "DUMp AST\n");
  // dumpAST(ast->root);
   Changetoasm(symtab,ast->root);
   destroyAST(&ast->root);
  // printf("\n\nFinished destorying AST\n\n");
   destroyTable(&symtab);
  // printf("\n\nFinished destorying table\n\n"); 
   return 0;
}

