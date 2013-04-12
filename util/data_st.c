/*
 * data_st.c
 *
 *  Created on: Apr 12, 2013
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
	int dec_value;
	char * base4_value;
	symbol_location location; /* where does it belong */
	symbol_type type;
	symbol_node *next;
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
	if(temp==NULL)/*Check if memory error encountered*/
	{
		printf(ERR_MEMORY_ALLOCATION_FAILURE);
		exit(1);
	}
	temp->count=0;
	temp->head=NULL;
	temp->tail=NULL;
	return (temp);
}

int symbol_exists_in (symbol_list * list, char * symbol) /*checks if 'symbol' exists in 'list' and returns the address of it, or 0*/
{
	symbol_node * curr;
	curr=list->head;
	while (curr!=NULL)
		if ((strcmp(symbol,curr->name))==0)
			return (curr->addr); /*if symbol matches the return address*/
		else curr=curr->next;/*iterate through the list*/
	return (0);
	/*end of list reached - return 0*/
}

int add_symbol (symbol_list * list, char * symbol, int address,symbol_type sym_type,symbol_location sym_loc) /*add a new symbol with a name and an address*/
{
	symbol_node *temp;
	symbol_node *new;
	if (list!=NULL && (new=malloc(sizeof(symbol_node)))!=NULL) /*if allocation worked*/
	{
		new->addr=address;
		strcpy(new->name,symbol);
		new->next=NULL;
		new->location=sym_loc;
		new->type=sym_type;

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
		printf(ERR_MEMORY_ALLOCATION_GEN);
		exit(1);
	}
	return (0);
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
			while (curr!=NULL)/* Iterate through list*/
			{
				to_clear=curr;/*free node by node*/
				curr=curr->next;/*careful not to free a NULL, use also count variable*/
				free(to_clear);
				fc++;
			}
		}
		free(list);/*free list pointer*/
		return (1);
	} else return (0); /*flush failed, nothing to flush*/
}


typedef struct {
	unsigned int comb:2;
	unsigned int dst_reg:3;
	unsigned int dst_add:2;
	unsigned int src_reg:3;
	unsigned int src_add:2;
	unsigned int command:4;
	unsigned int type:1;
	unsigned int reserve:3;
} opcode_bits;

typedef struct opn {
	int addr; /*Decimal opcode address*/
		char * command; /*command that was received from the file*/
		char *label; /* label that was received from the files*/
		char * arguments; /*arguments*/
		opcode_bits bits; /*bits of information*/
		char * base2code; /* code in binary */
		char * base4code; /* code in base 4 */
		symbol_location location; /*location of this record*/
		char mark; /*type of command*/
		struct opn *next;
} opcode_node;

typedef struct opl {
	int count;
	int next_addr;
	opcode_node * head;
	opcode_node * tail;
} opcode_list;

opcode_node *new_opcode_node()
{
	opcode_node * temp;
	temp=malloc(sizeof(opcode_node));
	if(temp==NULL)/*Checks if memory allocation error is encountered*/
	{
		printf(ERR_MEMORY_ALLOCATION_GEN);
		exit(1);
	}
	temp->next=NULL;
	return (temp);
}

opcode_list *new_opcode_list(int start_addr)
{
	opcode_list * temp;
	temp=malloc(sizeof(opcode_list));
	if(temp==NULL)/*Checks if memory allocation error is encountered*/
	{
		printf(ERR_MEMORY_ALLOCATION_GEN);
		exit(1);
	}
	temp->count=0;
	temp->next_addr=start_addr;
	temp->head=temp->tail=NULL;
	return (temp);
}


void add_opcode(opcode_list *list, opcode_node * new_item)
{
	opcode_node *temp;

	if(!(list->head)) /*if list is empty*/
		list->head = new_item;
	else /* if not empty, find the last node and add a new one to the list */
	{
		temp=(list->head);
		while(temp->next)
		{
			temp=temp->next;
		}
		temp->next=new_item;
	}
}

opcode_node *get_opcode_by_addr(opcode_list *list, int addr)
{
	opcode_node * curr;
	if (list!=NULL && (addr>=INITIAL_ADDRESS && addr<list->next_addr) && !IS_EMPTY(list)) 	/*not NULL list, addr within permitted range*/
	{
		curr=list->head;
		while (addr!=curr->addr && curr!=NULL)
			curr=curr->next; /*advance to where the relevant opcode is*/
		if (curr!=NULL) /*if it not the end of the list*/
			return (curr);
		else return (NULL); /*otherwise opcode was not found*/
	}
	else return (NULL); /*opcode retrieve failed*/
}

int flush_opcode_list(opcode_list *list)
{
	opcode_node *curr, *to_clear;
	int fc; /*free counter*/
	if (list!=NULL)
	{
		if (!IS_EMPTY(list))
		{
			curr=list->head;
			fc=0;
			while (curr!=NULL)/*iterate through list*/
			{
				to_clear=curr;/*flushes node by node*/
				curr=curr->next;
				free(to_clear);/*making sure NULL is not flushed*/
				fc++;
			}
		}
		free(list);/*flush list pointer*/
		return (1);
	} else return (0); /*flush failed or nothing to flush*/
}


typedef struct symbol {
	int addr; /*assigned address of the symbol*/
	char *name; /*symbol name*/
	int dec_value;
	char * base4_value;
	symbol_location location; /* where does it belong */
	symbol_type type;
	symbol_node *next;
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
	if(temp==NULL)/*Check if memory error encountered*/
	{
		printf(ERR_MEMORY_ALLOCATION_FAILURE);
		exit(1);
	}
	temp->count=0;
	temp->head=NULL;
	temp->tail=NULL;
	return (temp);
}

int symbol_exists_in (symbol_list * list, char * symbol) /*checks if 'symbol' exists in 'list' and returns the address of it, or 0*/
{
	symbol_node * curr;
	curr=list->head;
	while (curr!=NULL)
		if ((strcmp(symbol,curr->name))==0)
			return (curr->addr); /*if symbol matches the return address*/
		else curr=curr->next;/*iterate through the list*/
	return (0);
	/*end of list reached - return 0*/
}

int add_symbol (symbol_list * list, char * symbol, int address,symbol_type sym_type,symbol_location sym_loc) /*add a new symbol with a name and an address*/
{
	symbol_node *temp;
	symbol_node *new;
	if (list!=NULL && (new=malloc(sizeof(symbol_node)))!=NULL) /*if allocation worked*/
	{
		new->addr=address;
		strcpy(new->name,symbol);
		new->next=NULL;
		new->location=sym_loc;
		new->type=sym_type;

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
		printf(ERR_MEMORY_ALLOCATION_GEN);
		exit(1);
	}
	return (0);
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
			while (curr!=NULL)/* Iterate through list*/
			{
				to_clear=curr;/*free node by node*/
				curr=curr->next;/*careful not to free a NULL, use also count variable*/
				free(to_clear);
				fc++;
			}
		}
		free(list);/*free list pointer*/
		return (1);
	} else return (0); /*flush failed, nothing to flush*/
}

