/* 
 * Common definitions and declarations for compilers 
 * Author: Yu Zhang (yuzhang@ustc.edu.cn)
 */
#ifndef _COMMON_H_
#define _COMMON_H_
#include "util.h"

// operator kinds
// You could add more kinds of error messages into op.h 
enum {
#define opxx(a, b) OP_##a,
#include "op.h"
	OPLAST
};



typedef struct symbol {
	char	*name;	// name of the symbol
	bool	isInitial;	// whether it is initialized	
        enum{
            kconst=0,
 	    kvar,
	    kproc,
	  }kind;
	int level;	
	int val;	
 	int offset;
	int width;
} *Symbol;

typedef struct entry {
	struct symbol sym;
	struct entry *next;
} *Entry;

typedef struct table {
	// a hashtable to store symbols
	struct entry *buckets[256];
        int number;
} *Table;
#define HASHSIZE 256

// Function declarations corresponding to symbolic table
Table 	newTable();
Symbol enterconst(Table table,const char *name,int val,int level, int offset);
Symbol entervar(Table table,const char* name,int val,int type,int level,int offset);
Symbol enterfunc(Table table,const char* name,int offset,int width);
Symbol 	lookup(Table ptab, const char *name);
Symbol 	getSym(Table ptab, const char *name);
float 	getVal(Table ptab, const char *name);
Symbol 	setVal(Table ptab, const char *name, float val);
void 	destroyTable();
// Error/warning message
// You could add more kinds of error messages into errcfg.h 
enum {
#define errxx(a, b) a,
#include "errcfg.h"
	LASTERR
};

// An error/warning message
typedef struct errmsg {
	bool	isWarn;
	int 	type;
	char 	*msg;
	int	line;
	int	column;
} *Errmsg;

// Error factory
typedef struct errfactory { 
	List	errors;
	List	warnings;
} *ErrFactory;

// Function declarations on error message management
Errmsg	newError	(ErrFactory errfactory, int type, int line, int col);
Errmsg	newWarning	(ErrFactory errfactory, int type, int line, int col);
void	dumpErrmsg	(Errmsg error);
ErrFactory newErrFactory();
void	dumpErrors	(ErrFactory errfactory);
void	dumpWarnings	(ErrFactory errfactory);
void	destroyErrFactory(ErrFactory *errfact);

// abstract syntax tree
// Structure for tracking locations, same as YYLTYPE in y.tab.h
typedef struct location {
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} *Loc;

typedef struct {
	int 	op;
	//int type;
	int 	val;
	struct astnode	*kids[2];// kids of the AST node
} *Exp;

typedef struct {
   struct astnode *cond;
   struct astnode *stmt;
}*Choose;


/*typedef struct {
        enum {
           assign=0,
           function,
	   if_choose,
	   while_choose,
           successer,
           }kind;
	union{
	struct astnode *exp;
        struct astnode *fun_name;//how to call function 
  	struct astnode *Choose;
        struct astnode *successor;
        };
} *Stmt;*/


typedef struct {
        struct astnode *constdecl;
 	struct astnode *vardecl;
	struct astnode  *stmts;
} *Block;

typedef struct {
        List stmts;
}*Stmts;

typedef struct  {
   const char  *name;
   int value;
}*Constdef;

typedef struct {
    List   constdef;
}*Constdecl;


typedef struct {
   int type;//1 for have number ,2 for not have;
   const char  *name;
   int   value;
}*Vardef;
typedef struct {
   List vardef;   
}*Vardecl;



typedef struct {
    const char *function_name;
    struct astnode *block;
}*Procdef;
typedef struct {
   List procdef;
}*Procdecl;


typedef struct {
   struct astnode *constdecl;
   struct astnode *vardecl;
   struct astnode *procdecl;
   struct astnode *mainfunction;  
}*Program;
typedef struct astnode{
	enum {
		KValue = 0x200,		// numerial value:
		KName,			// name, such as variable name
		KPrefixExp,		// prefix expression
		KInfixExp,		// infix expression
		KAssignExp,		// assignment expression
		KParenExp,		// parentheses expression
		KStmt,		// expression statement
		KBlock,			// block
                KProgram,
                KConstDecl,
		KVarDecl,
		KProcDecl,
		KConstDef,
		KVardef,
		KProcDef,
		KStmts,
		Kif,
                Kwhile,
                Kcall,
                Knull,
		
	} kind;	// kind of the AST node
	union {		// information of various kinds of AST node 
		int  val;		// KValue: numerial value
		Symbol sym;		// KName: symbols 
		Exp   exp;		// KPrefixExp,
					// KInfixExp,
					// KAssignExp,
					// KParenExp
		//Stmt   stmt;		// KExpStmt
		Block  block;		// KBlock
                Choose choose;
                Stmts stmts;
		Constdecl constdecl;
		Constdef constdef;
		Vardecl vardecl;
		Vardef  vardef;
		Procdecl procdecl;
		Procdef  procdef;
		Program  program;
               
	};
	Loc 	loc;			// locations
} *ASTNode;

typedef struct ASTtree {
	ASTNode root;
} *ASTTree;
void destroyAST(ASTNode *pnode);
// functions for creating various kinds of ASTnodes
ASTNode newNumber(Table table, int value);
ASTNode newName(Table ptab, char *name);
ASTNode newConstdecl();
void destroyconstdecl(Constdecl *pnode);
ASTNode newConstdef(Table table,const char* name,int value);
ASTNode newVardecl();
void  destroyvardecl(Vardecl *pnode);
ASTNode newVardef(Table table,int type,const char* name ,int value);
ASTNode newProcDecl();
void destroyprocdecl(Procdecl *pnode);
ASTNode newprocdef(const char *name,ASTNode block);
void destroyprocdef(Procdef *pnode);
ASTNode newblock(ASTNode constdecl,ASTNode vardecl,ASTNode stmts );
void destroyblock(Block *pnode);
ASTNode newstmts();
void destroystmts(Stmts *pnode);
//ASTNode newstmt(int type,ASTNode add);
ASTNode newcall(Table table,char *name);
ASTNode newif(ASTNode cond,ASTNode stmt);
ASTNode newwhile(ASTNode cond,ASTNode stmt);
void destroyChoose(Choose *pnode);
ASTNode newNull();
ASTNode newPrefixExp(int op, ASTNode exp);
ASTNode newParentExp(ASTNode exp);
ASTNode newInfixExp(int op, ASTNode left, ASTNode right);
ASTNode newAssignment(int op, ASTNode left, ASTNode right);
void destroyExp(Exp *pnode);
ASTNode newprogram(ASTNode constdecl,ASTNode vardecl,ASTNode procdecl ,ASTNode block);
void destroyprogram(Program *pnode);

ASTTree newAST();
//void destroyAST(ASTNode *pnode);
void 	dumpAST(ASTNode node);
Loc	setLoc(ASTNode node, Loc loc);
void Changetoasm(Table table,ASTNode node);
void Create_asm(Table table,ASTNode node,FILE *fp,int *group_num);
#endif // !def(_COMMON_H_)
