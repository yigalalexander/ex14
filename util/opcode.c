/*
 * opcode.c
 *	Contains the opcode data structure
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#include <stdlib.h>
#include <stdio.h>
#include "lang.h"

#define IS_EMPTY(X) ((X->count==0)?1:0)

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
	int addr; /*opcode address*/
	opcode_bits bits; /*bits of information*/
	int flag; /*type of command*/
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
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
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
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
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



