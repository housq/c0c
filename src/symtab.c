/*
 * Functions of Symbolic Table
 * Author: Yu Zhang (yuzhang@ustc.edu.cn)
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <common.h>

/**
 * Creates a symbolic table
 */
Table
newTable()
{
	Table new;
	NEW0(new);
	new->number=0;
	return new;
}
/********enter*******/


Symbol enterconst(Table table,const char* name ,int val,int level,int offset)
{
      Entry pent;
      unsigned hashkey =(unsigned long)name[0]&(HASHSIZE-1);
      for(pent=table->buckets[hashkey];pent!=NULL;pent=pent->next)
        if(strcmp(name,pent->sym.name)==0)
            return &pent->sym;//const 重命名以第一个为主
       NEW0(pent);
       pent->sym.name=(char *)name;
       pent->sym.kind=kconst;
       pent->sym.val=val;
       pent->sym.level=level;
       pent->sym.isInitial=TRUE;
       pent->sym.offset=offset;
       pent->next=table->buckets[hashkey];
       table->buckets[hashkey]=pent;
       return &pent->sym; 
}

Symbol entervar(Table table,const char *name,int val,int type ,int level,int offset)
{
     Entry pent;
    unsigned hashkey=(unsigned long)name[0]&(HASHSIZE-1);
     for(pent=table->buckets[hashkey];pent!=NULL;pent=pent->next)
        if((strcmp(name,pent->sym.name)==0)&&(level==pent->sym.level))
            return &pent->sym;

       NEW0(pent);
        pent->sym.kind=kvar;
 	pent->sym.name=(char *)name;
        pent->sym.level=level;
	pent->sym.offset=0-offset-1;
	if(type ==1)
 	{
           pent->sym.val=val;
	   pent->sym.isInitial=TRUE;
        }
	else if(type==2)
        {
           pent->sym.val=0;
	   pent->sym.isInitial=FALSE;
        }
       pent->next=table->buckets[hashkey];
       table->buckets[hashkey]=pent;
       table->number++;
        return &pent->sym; 
}

Symbol enterfunc(Table table,const char *name ,int offset,int width)
{
      Entry pent;
    unsigned hashkey=(unsigned long)name[0]&(HASHSIZE-1);
     for(pent=table->buckets[hashkey];pent!=NULL;pent=pent->next)
        if((strcmp(name,pent->sym.name)==0))
            return &pent->sym;

        NEW0(pent);
	pent->sym.kind=kproc;
	pent->sym.name=(char *)name;
	pent->sym.offset=offset;
	pent->sym.width=width;
        pent->next=table->buckets[hashkey];
        table->buckets[hashkey]=pent;
//	printf(" \n\nsym .width in the enterfunc %d\n\n",width);
        return &pent->sym;

}
static void
destroyBucket(Entry *list)
{
	Entry node = *list, temp;
	while ( node != NULL ) {
		temp = node->next;
		free(node);
		node = temp;
	}
	*list = NULL;
}

/**
 * Destroys the specified table
 */
void
destroyTable(Table *tab)
{
	int i=0;
	if (*tab == NULL)
		return;
	Entry *bucket = (*tab)->buckets, *bucket_end = (*tab)->buckets+256;
	while (bucket < bucket_end ) {
		destroyBucket(bucket);
		bucket++;
	}
	free(*tab);
	*tab = NULL;
}

// Look up the symbolic table to get the symbol with specified name
Symbol
lookup(Table ptab, const char *name)
{
	Entry pent;
	unsigned hashkey = (unsigned long)name[0] & (HASHSIZE-1);
	for (pent = ptab->buckets[hashkey]; pent!=NULL; pent = pent->next)
		if ( strcmp(name, pent->sym.name) == 0 ) 
			return &pent->sym;
	return NULL;
}

// Get value of the specified name from the symbolic table
float
getVal(Table ptab, const char *name)
{
	Entry pent;
	unsigned hashkey = (unsigned long)name[0] & (HASHSIZE-1);
	for (pent = ptab->buckets[hashkey]; pent!=NULL; pent = pent->next)
		if ( strcmp(name, pent->sym.name) == 0 ) 
			return pent->sym.val;
	NEW0(pent);
	pent->sym.name = (char *)name;
	pent->sym.val = 0;
	printf("Warn: %s(%d) was not initialized before, set it 0 as default\n",
		(char *)name, hashkey);
	pent->next = ptab->buckets[hashkey];
	ptab->buckets[hashkey] = pent;
	return 0;
}

Symbol
getSym(Table ptab, const char *name)
{
	Entry pent;
	unsigned hashkey = (unsigned long)name[0] & (HASHSIZE-1);
	for (pent = ptab->buckets[hashkey]; pent!=NULL; pent = pent->next)
		if ( strcmp(name, pent->sym.name) == 0 ) 
			return &pent->sym;
	NEW0(pent);
	pent->sym.name = (char *)name;
	pent->sym.val = 0;
	pent->sym.isInitial = FALSE;
	pent->next = ptab->buckets[hashkey];
	ptab->buckets[hashkey] = pent;
	return &pent->sym;
}

// Set value of the specified name into the symbolic table
Symbol
setVal(Table ptab, const char *name, float val)
{
	Entry pent;
	unsigned hashkey = (unsigned long)name[0] & (HASHSIZE-1);
	for (pent = ptab->buckets[hashkey]; pent!=NULL; pent = pent->next) {
		if ( strcmp(name, pent->sym.name) == 0 ) {
			pent->sym.val = val;
#if DEBUG
			printf("update %s(%d)  %f\n", 
				(char *)name, hashkey, val);
#endif
			return &pent->sym;
		}
	}
	NEW0(pent);
	pent->sym.name = (char *)name;
	pent->sym.val = val;
#if DEBUG
	printf("create %s(%d) %f\n", (char *)name, 
		hashkey, val);
#endif
	pent->next = ptab->buckets[hashkey];
	ptab->buckets[hashkey] = pent;
	return &pent->sym;
}
