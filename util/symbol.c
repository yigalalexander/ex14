/*
 * symbol.c
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lang.h"

#define IS_EMPTY(X) ((X->count==0)?1:0)

typedef struct symbol {
	int addr; /*assigned address of the symbol*/
	char *name; /*symbol name*/
	struct symbol *next;
} symbol_node;

typedef struct sym_list {
	symbol_node *head;
	symbol_node *tail;
	int count;
} symbol_list;

symbol_list * new_symbol_list()
{
	symbol_list *temp;
	temp=malloc(sizeof(symbol_list));
	if(temp==NULL)/*Check if memory error*/
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
		exit(1);
	}
	temp->count=0;
	temp->head=NULL;
	temp->tail=NULL;
	return (temp);
}

int symbol_exists_in (symbol_list * list, char * symbol) /*check if 'symbol' exists in 'list' and returns the address of it, or 0*/
{
	symbol_node * curr;
	curr=list->head;
	while (curr!=NULL)
		if ((strcmp(symbol,curr->name))==0)
			return (curr->addr); /*if symbol maches return address*/
		else curr=curr->next;/* iterate through list*/
	return (0);
	/*made it to NULL pointer return 0*/
}

int add_symbol (symbol_list * list, char * symbol, int address) /*add a new symbol with a name and an address*/
{
	symbol_node *temp,*new;
	if (list!=NULL && (new=malloc(sizeof(symbol_node)))!=NULL) /*if allocation worked*/
	{
		new->addr=address;
		strcpy(new->name,symbol);
		new->next=NULL;

		if (list->head==NULL) /*if its an empty list*/
		{
			list->head=new;
			list->tail=new;
		} else /*if we have nodes in it*/
		{
			temp=list->head;
			while (temp->next!=NULL)
				temp=temp->next;/*find the last node*/
			temp->next=new; /*attach the new node to the last one*/
			list->tail=new;
		}
		list->count++;
	}
	else /*allocation failed*/
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
		exit(1);
	}
	return (0);
}

int delete_symbol_by_name (symbol_list * list, char * name) /* same as above, but by the name */
{
	symbol_node *curr,*prev; /*positions*/
	prev=curr=list->head;

	if (curr!=NULL) /*not an empty list*/
	{

		while (curr!=NULL) /*did not make it to the end of the list*/
		{
			if (strcmp(name,curr->name)==0) /*found the symbol*/
			{
				if (curr==list->head)/*if first, check by list*/
				{
					list->head=curr->next;
				} else /*if not first*/
				{
					prev->next=curr->next;
				}
				free(curr); /*release the node*/
				list->count--;
				return (1);
			}
			prev=curr;
			curr=curr->next;
		}
	}
	return (0); /*empty list, nothing to delete*/
}

int delete_symbol_by_addr (symbol_list * list, int address) /* same as above, but by the name */
{
	symbol_node *curr,*prev; /*positions*/
	prev=curr=list->head;

	if (curr!=NULL) /*not an empty list*/
	{

		while (curr!=NULL) /*did not make it to the end of the list*/
		{
			if (curr->addr==address) /*found the symbol*/
			{
				if (curr==list->head)/*if first, check by list*/
				{
					list->head=curr->next;
				} else /*if not first*/
				{
					prev->next=curr->next;
				}
				free(curr); /*release the node*/
				list->count--;
				return (1);
			}
			prev=curr;
			curr=curr->next;
		}
	}
	return (0); /*empty list, nothing to delete*/
}

int flush_symbols(symbol_list *list)
{
	symbol_node *curr, *to_clear;
	int fc; /*free count*/
	if (list!=NULL)
	{
		if (!IS_EMPTY(list)) /* not an empty list*/
		{
			curr=list->head;
			fc=0;
			while (curr!=NULL)/* interate through list*/
			{
				to_clear=curr;/*free node by node*/
				curr=curr->next;/*careful not to free a NULL, use also count variable*/
				free(to_clear);
				fc++;
			}
		}
		free(list);/*free list pointer*/
		return (1);
	} else return (0); /*flush failed, nothing to fail*/
}
