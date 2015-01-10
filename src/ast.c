/*
 * TODO Functions of Abstract Syntax Tree
 * Author: Yu Zhang (yuzhang@ustc.edu.cn)
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"

char *opname[]={
#undef opxx
#define opxx(a, b) b,
#include "op.h"
	"Undefined Op"
};

ASTNode
newNumber(Table table,int value)
{
	ASTNode new;
	NEW0(new);
	new->kind = KValue;
	new->val = value;
	return new;
}

ASTNode
newName(Table ptab, char *name)
{
	ASTNode new;
	NEW0(new);
	new->kind = KName;
	new->sym = getSym(ptab, name);
	return new;
}
ASTNode newcall(Table table,char *name)
{
        ASTNode new;
        NEW0(new);
        new->kind =Kcall;
        new->sym =getSym(table,name);
        return new;
}
ASTNode 
newConstdecl()
{
  ASTNode new;
  NEW0(new);
  new->kind=KConstDecl;
  Constdecl new_decl;
  NEW0(new_decl);
  new_decl->constdef=newList();
  new->constdecl=new_decl;
  return new;
  
}
void destroyconstdecl(Constdecl *pnode)
{
  if(*pnode ==NULL) return;
  Constdecl node =*pnode;
  destroyList(&node->constdef,destroyAST);
  free(node);
  *pnode =NULL;
}
ASTNode 
newVardecl()
{
   ASTNode new;
   NEW0(new);
   new->kind=KVarDecl;
   Vardecl new_var;
   NEW0(new_var);
   new_var->vardef=newList();
   new->vardecl=new_var;
   return new;
}   
void destroyvardecl(Vardecl *pnode)
{
  if(*pnode ==NULL) return;
  Vardecl node =*pnode;
  destroyList(&node->vardef,destroyAST);
  free(node);
  *pnode =NULL;
}

ASTNode 
newProcDecl()
{
    ASTNode new;
    NEW0(new);
    new->kind=KProcDecl;
    Procdecl new_fun;
    NEW0(new_fun);
    new_fun->procdef=newList();
    new->procdecl=new_fun;
    return new;
}
void destroyprocdecl(Procdecl *pnode)
{
  if(*pnode ==NULL) return;
  Procdecl node =*pnode;
  destroyList(&node->procdef,destroyAST);
  free(node);
  *pnode =NULL;
}
ASTNode 
newConstdef(Table table,const char *name,int value)
{
    ASTNode new;
    NEW0(new);
    new->kind=KConstDef;
    Constdef newconstdef;
    NEW0(newconstdef);
    newconstdef->name=name;
    newconstdef->value=value;
    new->constdef=newconstdef;
    return new;
}


ASTNode 
newVardef(Table table,int type,const char* name ,int value)
{
    ASTNode new;
    NEW0(new);
    new->kind=KVardef;
    Vardef new_def;
    NEW0(new_def);
    new_def->type=type;
    new_def->name=name;
    new_def->value=value;
    new->vardef=new_def;
    return new;
}
    
ASTNode 
newprocdef(const char *name,ASTNode block)
{
    ASTNode new;
    NEW0(new);
    new->kind=KProcDef;
    Procdef new_def;
    NEW0(new_def);
    new_def->function_name=name;
    new_def->block=block;
    new->procdef =new_def;
    return new;
}
void
destroyprocdef(Procdef *pnode)
{
   if(*pnode ==NULL) return;
   Procdef node=*pnode;
   destroyAST(&node->block);
   free(node);
   *pnode=NULL;
}
ASTNode 
newstmts()
{
   ASTNode new;
   NEW0(new);
   new->kind=KStmts;
   Stmts new_s;
   NEW0(new_s);
   new_s->stmts=newList();
   new->stmts=new_s;
   return new;
}
void
destroystmts(Stmts *pnode)
{
   if(*pnode ==NULL) return;
   Stmts node=*pnode;
   destroyList(&node->stmts,destroyAST);
   free(node);
   *pnode=NULL;
}

ASTNode 
newblock(ASTNode constdecl,ASTNode vardecl,ASTNode stmts)//list how to add
{
   ASTNode new;
   NEW0(new);
   new->kind=KBlock;
   Block new_block;
   NEW0(new_block);
   new_block->constdecl=constdecl;
   new_block->vardecl=vardecl;
   new_block->stmts=stmts;
   new->block=new_block;
   return new;
}
void destroyblock(Block *pnode)
{
   if(*pnode==NULL) return ;
   Block node =*pnode;
   destroyAST(&node->constdecl);
   destroyAST(&node->vardecl);
   destroyAST(&node->stmts);
   free(node);
   *pnode=NULL;
}

ASTNode newif(ASTNode cond ,ASTNode stmt)
{
    
   ASTNode new;
   NEW0(new);
   new->kind=Kif;
   Choose new_c;
   NEW0(new_c);
   new_c->cond=cond;
   new_c->stmt=stmt;
   new->choose=new_c;
   return new;
}

ASTNode newwhile(ASTNode cond ,ASTNode stmt)
{
  
   ASTNode new;
   NEW0(new);
   new->kind=Kwhile;
   Choose new_c;
   NEW0(new_c);
   new_c->cond=cond;
   new_c->stmt=stmt;
   new->choose=new_c;
   return new;
}
void destroyChoose(Choose *pnode)
{
    if(*pnode==NULL)return ;
    Choose node=*pnode;
    destroyAST(&node->cond);
    destroyAST(&node->stmt);
    free(node);
    *pnode=NULL;
}

ASTNode newNull()
{
   ASTNode new;
   NEW0(new);
   new->kind = Knull;
   return new; 
}
/*ASTNode newstmt(int type,ASTNode add)//add assign this type
{
     ASTNode new;
     NEW0(new);
     new->kind=KStmt;
     Stmt new_st;
     switch(type)
     {
        case 0:
                new_st->kind=assign;
 		new_st->exp=add;
                break;
      	case 1:
		new_st->kind=function;
		new_st->fun_name=add; 
		break;
	case 2:
		new_st->kind=if_choose;
		new_st->Choose=add;
		break;
	case 3:
 		new_st->kind=while_choose;
		new_st->Choose=add;
		break;
	case 4:
		new_st->kind=successer;	
		new_st->successor=add;//add stmts
		break;
     }
     new->stmt=new_st;
		
}*/

ASTNode
newPrefixExp(int op, ASTNode exp)
{
	ASTNode new;
	NEW0(new);
	new->kind = KPrefixExp;
	Exp newexp;
	NEW0(newexp);
	newexp->op = op;
	newexp->kids[0] = exp;
	new->exp = newexp;
	return new;
}

ASTNode
newParentExp(ASTNode exp)
{
	ASTNode new;
	NEW0(new);
	new->kind = KParenExp;
	Exp newexp;
	NEW0(newexp);
	newexp->op = -1;
	newexp->kids[0] = exp;
	new->exp = newexp;
	return new;
}

ASTNode
newInfixExp(int op, ASTNode left, ASTNode right)
{
	ASTNode new;
	NEW0(new);
	new->kind = KInfixExp;
	Exp newexp;
	NEW0(newexp);
	newexp->op = op;
	newexp->kids[0] = left;
	newexp->kids[1] = right;
	new->exp = newexp;
	return new;
}

ASTNode
newAssignment(int op, ASTNode left, ASTNode right)
{
	ASTNode new;
	NEW0(new);
	new->kind = KAssignExp;
	Exp newexp;
	NEW0(newexp);
	newexp->op = op;
	newexp->kids[0] = left;
	newexp->kids[1] = right;
	new->exp = newexp;
	return new;
}
void
destroyExp(Exp *pnode)
{
	if (*pnode == NULL) return;
	Exp node = *pnode;
	destroyAST(&node->kids[0]);
	destroyAST(&node->kids[1]);
	free(node);
	*pnode = NULL;
}

ASTNode newprogram(ASTNode constdecl,ASTNode vardecl,ASTNode procdecl ,ASTNode block)
{
     ASTNode new;
     NEW0(new);
     new->kind=KProgram;
     Program new_pro;
     NEW0(new_pro);
     new_pro->constdecl=constdecl;
     new_pro->vardecl=vardecl;
     new_pro->procdecl=procdecl;
     new_pro->mainfunction=block;
     new->program=new_pro;
    // printf("the ast.c newprogram is ok:\n");
     return new;
    
}
void destroyprogram(Program *pnode)
{
   if(*pnode==NULL)return;
   Program node=*pnode;
   destroyAST(&node->constdecl);
   destroyAST(&node->vardecl);
   destroyAST(&node->procdecl);
   destroyAST(&node->mainfunction);
   free(node);
   *pnode=NULL;
}

ASTTree
newAST()
{
	ASTTree new;
	NEW0(new);
	return new;
}

/*void
destroyAST(ASTNode *pnode)
{
	if (*pnode == NULL) return;
	ASTNode node = *pnode;
	int kind = node->kind;
	
	switch (kind) {
	case KValue:
	case KName:
		break;
	case KPrefixExp:
	case KParenExp:
	case KInfixExp:
	case KAssignExp:
		destroyExp(&node->exp);
		break;
	case KExpStmt:
		destroyExpStmt(&node->estmt);
		break;
	case KBlock:
		destroyBlock(&node->block);
		break;
	default:
		printf("Unhandled ASTNode kind!\n");
	}
	free(node);
	*pnode = NULL;
}
*/
void destroyAST(ASTNode *pnode)
{
    if(*pnode==NULL) return;
    ASTNode node=*pnode;
    int kind=node->kind;
   
    switch(kind)
      {
         case KValue :
         case KName:
         case KPrefixExp:
         case KInfixExp:
         case KAssignExp:
         case KParenExp:
                break;
         case KBlock:  
                destroyblock(&node->block);
                 break;
         case KProgram:
                destroyprogram(&node->program);
                break;
         case KConstDecl:
                destroyconstdecl(&node->constdecl);
                break;
         case KVarDecl:
                destroyvardecl(&node->vardecl);
                break;
        case  KProcDecl:
                destroyprocdecl(&node->procdecl);
                break;
         case KProcDef:
                destroyprocdef(&node->procdef);
                break;
         case KStmts:
                destroystmts(&node->stmts);
                break;
          case Kif:
         case  Kwhile:
                destroyChoose(&node->choose);
                break;
         case  Kcall:
         case KConstDef:
         case KVardef:
         case Knull :
                break;
                      
      }
      free(node);
      *pnode=NULL;
   
}
Loc
setLoc(ASTNode node, Loc loc)//fline, int fcol, int lline, int lcol)
{
	if (node->loc == NULL )
		NEW0(node->loc);
	node->loc->first_line = loc->first_line;
	node->loc->first_column = loc->first_column;
	node->loc->last_line = loc->last_line;
	node->loc->last_column = loc->last_column;
	return node->loc;
}


void
dumpAST(ASTNode node)
{
        //printf("dump AST\n");
	if(!node) return;
	int kind = node->kind;
	
	switch (kind) {
	case KValue:
		printf("%d", node->val);
		break;
	case KName:
		printf("%s", node->sym->name);
		break;
	case KPrefixExp:
		printf("%s", opname[node->exp->op]);
		dumpAST(node->exp->kids[0]);
		break;
	case KParenExp:
		printf("(");
		dumpAST(node->exp->kids[0]);
		printf(")");
		break;
	case KInfixExp:
		dumpAST(node->exp->kids[0]);
		printf("%s", opname[node->exp->op]);
		dumpAST(node->exp->kids[1]);
		break;
	case KAssignExp:
		dumpAST(node->exp->kids[0]);
		printf("%s", opname[node->exp->op]);
		dumpAST(node->exp->kids[1]);
   		printf(";");
		break;
	case KBlock:
               {
                
                dumpAST(node->block->constdecl);
                dumpAST(node->block->vardecl);
                dumpAST(node->block->stmts);
	        break;
                }
        case KProgram:
                dumpAST(node->program->constdecl);
                printf("\n");
                dumpAST(node->program->vardecl);
                printf("\n");
                dumpAST(node->program->procdecl);
                printf("void main() \n{\n");
                dumpAST(node->program->mainfunction);
                printf("}");
                break;
        case  KConstDecl:
               {
                  
                  List constdef=node->constdecl->constdef;
                  ListItr itr=getGListItr(constdef,0);
                  if(hasNext(itr))
                 {
                  printf("const int ");
                  dumpAST((ASTNode)nextItem(itr));
                 
                  while(hasNext(itr))
                   {
                       printf(",");
                       dumpAST((ASTNode)nextItem(itr));
                      // printf("\n");
                   }
                 printf(";");}
                 break;
                 
               }
        case  KVarDecl:
              {
                 
 		 List vardef=node->vardecl->vardef;
                  ListItr itr=getGListItr(vardef,0);
                  if(hasNext(itr))
                    {
                       printf("int  ");
                       dumpAST((ASTNode)nextItem(itr));
                    
                 // dumpAST((ASTNode)nextItem(itr));
                  while(hasNext(itr))
                   {
                       printf(",");
                       dumpAST((ASTNode)nextItem(itr));
                       //printf("\n");
                   } 
                 printf(";");}
                 break;
               

              }
        case KProcDecl:
             {
                 //printf("KProcDecl");
       		 List procdef=node->procdecl->procdef;         
                
                Lnode itr = procdef->first;
                 if(itr){
			dumpAST((ASTNode)itr->item);
			itr=itr->next;
			while(itr){
				printf("\n");
				dumpAST((ASTNode)itr->item);
				itr = itr->next;
			}
			printf("\n");
		}
		break;
             } 
   	case   KConstDef:
                  printf("%s ",node->constdef->name);   
                  printf("=");
                  printf("%d",node->constdef->value);
                  break;
        case  KVardef:
               {
                 printf("%s",node->vardef->name);
                 if(node->vardef->name)
                   {
                      printf("=");
                      printf("%d",node->vardef->value);
                   }
                 break;
                }
 	case  KProcDef:
              { 
               printf("void ");
               printf("%s()\n{\n",node->procdef ->function_name);
               dumpAST(node->procdef->block);
               printf("}\n");
               break;
              }
        case KStmts:
              {
                 List stmts=node->stmts->stmts;
                  ListItr itr=getGListItr(stmts,0);
                  while(hasNext(itr))
                   {
                       dumpAST((ASTNode)nextItem(itr));
                       printf("\n");
                   }
                 break;
              }
        case Kif:
             printf("if(");
             dumpAST(node->choose->cond);
             printf(")\n");
             printf("{\n");
             dumpAST(node->choose->stmt);
             printf("}\n");
             break;
        case Kwhile:
             printf("while(");
             dumpAST(node->choose->cond);
             printf(")\n");
             printf("{\n");
             dumpAST(node->choose->stmt);
             printf("}\n");
             break;
        case Kcall:
              printf("%s",node->sym->name);
              printf("();\n");
              break;
        case Knull:
              printf(";");
              break;                    
	default:
		printf("Unhandled ASTNode kind!\n");
	}
}
void Changetoasm(Table table,ASTNode node)
{
    FILE *fp;
    int group_num=0;
    if((fp=fopen("c0.s","w"))==NULL)
    {
      printf("File can not open!\n");
      return ;
    }
    
    Create_asm(table,node, fp,&group_num);

    fclose(fp);
}
void Create_asm(Table table,ASTNode node,FILE *fp,int *group_num)
{
      //fprintf(fp,"\n\n  in the changetoasm \n\n");
       char *temp;
       Symbol s_t;
       int ad;
       if(!node) return;
	int kind = node->kind;
         switch(kind)
 {
     case KProgram:
        {
            s_t=getSym(table,"main");
            fprintf(fp,"%s\n",".section .data");
            fprintf(fp,"out_start:\n");
            fprintf(fp,".ascii \"c0.o code start\"\n");
            fprintf(fp,"out_start_end:\n");
            fprintf(fp,".equ len,out_start_end-out_start\n");
            fprintf(fp,"out_end:\n");
            fprintf(fp,".ascii \"c0.o code end\"\n");
            fprintf(fp,"out_end_end:\n");
            fprintf(fp,".equ len2,out_end_end-out_end\n");
            Create_asm(table,node->program->vardecl,fp,group_num);
            fprintf(fp,"%s\n",".section .text");
            fprintf(fp,"%s\n",".globl _start");
            fprintf(fp,"%s\n","_start:");
            fprintf(fp,"call main\n");
	    fprintf(fp,"mov $0,%%ebx\n");
	    fprintf(fp,"mov $1,%%eax\n");
	    fprintf(fp, "int $0x80\n");
            Create_asm(table,node->program->procdecl,fp,group_num);
            fprintf(fp,".type main,@function\n");
            fprintf(fp,"%s\n","main:");
            fprintf(fp,"pushl %%ebp\n");
            fprintf(fp,"movl %%esp,%%ebp\n");
            fprintf(fp,"subl $%d,%%esp\n",s_t->width*4);
            fprintf(fp,"mov $4,%%eax\n mov $1,%%ebx\n mov $out_start,%%ecx\n mov $len,%%edx\n int $0x80\n xor %%ebx,%%ebx\n xor %%eax,%%eax \n xor %%ecx,%%ecx\n xor %%edx,%%edx\n");
            Create_asm(table,node->program->mainfunction,fp,group_num);
            fprintf(fp,"mov $4,%%eax\n mov $1,%%ebx\n mov $out_end,%%ecx\n mov $len2,%%edx\n int $0x80\n");
            fprintf(fp,"movl %%ebp,%%esp\n");
	    fprintf(fp,"popl %%ebp\n");
	    fprintf(fp,"ret\n");
            break;

        }
    case KVarDecl:
       {
          List vardef=node->vardecl->vardef;
          ListItr itr=getGListItr(vardef,0);
          if(hasNext(itr))
          {
              Create_asm(table,(ASTNode)nextItem(itr),fp,group_num);

          }
          while(hasNext(itr))
          {
              Create_asm(table,(ASTNode)nextItem(itr),fp,group_num);

          }
          break;
       }
       case KVardef:
       {
           temp=(char *)node->vardef->name;
            s_t=getSym(table,temp);
            if(s_t->kind==kvar)
            {
                if(s_t->level==0)
                {
                    fprintf(fp,"%s :\n",s_t->name);
                    if(s_t->isInitial==TRUE)
                    {
                        fprintf(fp,".int %d\n",s_t->val);
                    }
                    else fprintf(fp,".int %d\n",0);
                }
                else
                {
                    ad=(s_t->offset)*4;
                    fprintf(fp,"movl $%d,%d(%%ebp)\n",s_t->val,ad);
                }
            }
            else printf("error ,the variable is not a variant variable\n");
            break;
       }
       case KProcDecl:
         {
             List procdef =node->procdecl->procdef;
             Lnode itr=procdef->first;
             while(itr)
             {
                 Create_asm(table,(ASTNode)itr->item,fp,group_num);
                 itr=itr->next;
             }
             break;
         }
         case KProcDef:
         {
             temp=(char *)node->procdef->function_name;
             s_t=getSym(table,temp);
             fprintf(fp,".type %s,@function\n",s_t->name);
             fprintf(fp,"%s :\n",s_t->name);
             fprintf(fp,"%s\n","pushl %ebp");
             fprintf(fp,"%s\n","movl %esp,%ebp");
             //printf("\n\n %d\n\n",s_t->width);
             fprintf(fp,"subl $%d,%%esp\n",s_t->width*4);
             Create_asm(table,node->procdef->block,fp,group_num);
            // fprintf(fp,"addl $%d,%%esp\n",s_t->width*4);
            fprintf(fp,"movl %%ebp,%%esp\n");
	    fprintf(fp,"popl %%ebp\n");
             fprintf(fp,"ret\n");
             break;
        }
        case KBlock:
        {
            Create_asm(table,node->block->vardecl,fp,group_num);
            Create_asm(table,node->block->stmts,fp,group_num);
            break;
        }
        case KStmts:
        {
             List stmts=node->stmts->stmts;
             ListItr itr=getGListItr(stmts,0);
             while(hasNext(itr))
             {
                 Create_asm(table,(ASTNode)nextItem(itr),fp,(group_num));

             }
             break;
        }
        case KAssignExp:
        {
              Create_asm(table,node->exp->kids[1],fp,group_num);
              s_t=node->exp->kids[0]->sym;
              if(s_t->level==0)
              {
                  fprintf(fp,"movl %%eax,%s\n",s_t->name);
              }
              else  fprintf(fp,"movl %%eax,%d(%%ebp)\n",(s_t->offset*4));
              break;
        }
        case Kcall:
        {
              fprintf(fp,"call %s\n",node->sym->name);
              break;
        }
        case Kif:
        {
            int k=*group_num;
            Create_asm(table,node->choose->cond,fp,group_num);
            fprintf(fp,"movl %%eax,%%edx\n");
            fprintf(fp,"jz L%d\n",k);
            *group_num=*group_num+1;
            Create_asm(table,node->choose->stmt,fp,group_num);
            fprintf(fp,"L%d :\n",k);
            break;
        }
        case Kwhile:
        {
            int k=*group_num;
            fprintf(fp,"w_s%d :\n",k);
            Create_asm(table,node->choose->cond,fp,group_num);
            fprintf(fp,"test %%eax,%%eax\n");
            fprintf(fp,"jz wh%d\n",k);
	    *group_num=*group_num+1;
            Create_asm(table,node->choose->stmt,fp,group_num);
            fprintf(fp,"jmp w_s%d\n",k);
            fprintf(fp,"wh%d :\n",k);
            break;
        }
        case KInfixExp:
        {
           int k=*group_num;
           *group_num=*group_num+1;
           Create_asm(table,node->exp->kids[0],fp,group_num);
           fprintf(fp,"pushl %%eax\n");
           Create_asm(table,node->exp->kids[1],fp,group_num);
           int op=node->exp->op;//just have a try
           switch(op)
           {
               case 0:
                  {
                      fprintf(fp,"movl %%eax,%%ebx\n");
                      fprintf(fp,"popl %%eax\n");
                      fprintf(fp,"addl %%ebx,%%eax\n");
                      break;
                  }
               case 1:
               {
                     fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"subl %%ebx,%%eax\n");
                     break;
               }
               case 2:
               {
                    fprintf(fp,"movl %%eax,%%ebx\n");
                    fprintf(fp,"popl %%eax\n");
                    fprintf(fp,"mul %%ebx\n");
                    break;
                }
                case 3:
                 {
                     fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"divl  %%ebx\n");
                     break;
                 }
                 case 4: break;
                 case 5:
                 {
                     fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"cmp %%ebx,%%eax\n");
                     fprintf(fp,"jnz k%d\n",k);
                     fprintf(fp,"movl $1,%%eax\n");
                     fprintf(fp,"jmp k%d_end\n",k);
                     fprintf(fp,"k%d:\n",k);
                     fprintf(fp,"xor %%eax,%%eax\n");
                     fprintf(fp,"k%d_end :\n",k);
                     break;
                 }
                 case 6:
                 {
                     fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"cmp %%ebx,%%eax\n");
                     fprintf(fp,"jle k2%d\n",k);
                     fprintf(fp,"movl $1,%%eax\n");
                     fprintf(fp,"jmp k2%d_end\n",k);
                     fprintf(fp,"k2%d:\n",k);
                     fprintf(fp,"xor %%eax,%%eax\n");
                     fprintf(fp,"k2%d_end :\n",k);
                     break;
                 }
                 case 7:
                 {
                     fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"cmp %%ebx,%%eax\n");
                     fprintf(fp,"jl k3%d\n",k);
                     fprintf(fp,"movl $1,%%eax\n");
                     fprintf(fp,"jmp k3%d_end\n",k);
                     fprintf(fp,"k3%d:\n",k);
                     fprintf(fp,"xor %%eax,%%eax\n");
                     fprintf(fp,"k3%d_end :\n",k);
                     break;
                 }
                 case 8:
                 {
                     fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"cmp %%ebx,%%eax\n");
                     fprintf(fp,"jge k4%d\n",k);
                     fprintf(fp,"movl $1,%%eax\n");
                     fprintf(fp,"jmp k4%d_end\n",k);
                     fprintf(fp,"k4%d:\n",k);
                     fprintf(fp,"xor %%eax,%%eax\n");
                     fprintf(fp,"k4%d_end :\n",k);
                     break;
                 }
                 case 9:
              {
                    fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"cmp %%ebx,%%eax\n");
                     fprintf(fp,"jg k5%d\n",k);
                     fprintf(fp,"movl $1,%%eax\n");
                     fprintf(fp,"jmp k5%d_end\n",k);
                     fprintf(fp,"k5%d:\n",k);
                     fprintf(fp,"xor %%eax,%%eax\n");
                     fprintf(fp,"k5%d_end :\n",k);
                     break;
              }
           case 10:
              {
                   fprintf(fp,"movl %%eax,%%ebx\n");
                     fprintf(fp,"popl %%eax\n");
                     fprintf(fp,"cmp %%ebx,%%eax\n");
                     fprintf(fp,"je k6%d\n",k);
                     fprintf(fp,"movl $1,%%eax\n");
                     fprintf(fp,"jmp k6%d_end\n",k);
                     fprintf(fp,"k6%d:\n",k);
                     fprintf(fp,"xor %%eax,%%eax\n");
                     fprintf(fp,"k6%d_end :\n",k);
                     break;
              }
            default :break;

           }
           break;
        }
        case KParenExp:
        {
             Create_asm(table,node->exp->kids[0],fp,group_num);
             break;
        }
        case KPrefixExp:
        {
             Create_asm(table,node->exp->kids[0],fp,group_num);
             int op=node->exp->op;
             if(op==1)
             {
                 fprintf(fp,"movl %%eax,%%ebx\n");
                 fprintf(fp,"movl $0,%%eax\n");
                 fprintf(fp,"subl %%ebx,%%eax\n");
                 break;
             }
             break;

        }
        case KName:
        {
            s_t=node->sym;/***********intialed*******/
           // fprintf(fp,"\n\n %s \n\n",s_t->name);
            if(s_t->level==0)
            {
                if(s_t->kind==kconst)
                    fprintf(fp,"movl $%d,%%eax\n",s_t->val);
                else if(s_t->kind==kvar)
                     fprintf(fp,"movl %s,%%eax\n",s_t->name);

             }
            else 
             {
                  if(s_t->kind==kconst)
                      fprintf(fp,"movl $%d,%%eax\n",s_t->val);
                  else if(s_t->kind==kvar)
                       fprintf(fp,"movl %d(%%ebp),%%eax\n",s_t->offset*4);
              }
            break;
        }
        case KValue:
        {
            fprintf(fp,"movl $%d,%%eax\n",node->val);
            break;
        }
        case KConstDecl:
        case KConstDef:
        case Knull:
        default : break;

 }


}
