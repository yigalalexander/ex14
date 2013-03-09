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
	temp->next=NULL;
	return (temp);
}

opcode_list *new_opcode_list(int start_addr)
{
	opcode_list * temp;
	temp=malloc(sizeof(opcode_list));
	temp->count=0;
	temp->next_addr=start_addr;
	temp->head=temp->tail=NULL;
	return (temp);
}


int add_opcode(opcode_list *list, opcode_bits data, int flag)
{
	opcode_node * temp;
	int address_ret; /*address that would be returned*/

	if (list!=NULL && (list->count<(INITIAL_ADDRESS+MAX_OPS))) /* check pointer is no NULL, AND we did not exceed max num of ops*/
	{
		temp=new_opcode_node(); /*make new node*/
		if (IS_EMPTY(list)) /*empty list*/
		{
			list->head=temp;
			list->tail=temp;
		}
		else if (list->head==list->tail) /*single node*/
		{
			list->tail->next=temp;
			list->tail=temp;
		}
		else /*more than one node, IS THIS REALLY DIFFERENT FROM THE ABOVE????*/
		{
			list->tail->next=temp;
			list->tail=temp;
		}
		temp->addr=list->next_addr; /*set address*/
		temp->bits=data;
		temp->flag=flag;
		address_ret=list->next_addr;
		list->next_addr++; /* increase next address place */
		return (address_ret);
	} else return (0); /*failed to add for some reason*/
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
		else return (NULL); /*otherwise it was not found*/
	}
	else return (NULL); /*failed retrieve*/
}

int flush_list(opcode_list *list)
{
	opcode_node *curr, *to_clear;
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



