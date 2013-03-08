/*
 * symbol.c
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#include <stdlib.h>
#include <stdio.h>

typedef struct symbol {
	int addr; /*assigned address of the symbol*/
	char *name; /*symbol name*/
	struct symbol *next;
} symbol_node;

int symbol_exists_in (symbol_node * list, char * symbol) /*check if 'symbol' exists in 'list' and returns the address of it, or 0*/
{
	symbol_node * curr;
	curr=list;
	while (curr!=NULL)
		if ((strcmp(symbol,curr->name))==0)
			return (curr->addr); /*if symbol maches return address*/
		else curr=curr->next;/* iterate through list*/
	return (0);
	/*made it to NULL pointer return 0*/
}

int add_symbol (symbol_node * list, char * symbol, int address) /*add a new symbol with a name and an address*/
{
	symbol_node *temp;
	if (list==NULL) /*if its an empty list*/
	{
		list=malloc(sizeof(symbol_node));
		list->addr=address;
		strcpy(list->name,symbol);
		list->next=NULL;
	} else /*if we have nodes in it*/
	{
		temp=malloc(sizeof(symbol_node));
		temp->addr=address;
		strcpy(temp->name,symbol);
		temp->next=list;
		list=temp;
	}
}

int flush_symbols(symbol_node *list)
{
	if (list!=NULL)
	{
		/* for each node free it, use a temp pointer to the next*/
		symbol_node * temp;

		while (list->next!=NULL) /*while we have another node*/
		{
			temp=list; /*save the pointer for this node
			list=list->next; /*move one node forward */
			free(temp); /* free current node */
		}
		free(list); /* free the list */
		return (1); /* finished with success */
	} else return (0); /* failed */
}

