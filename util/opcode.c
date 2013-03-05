/*
 * opcode.c
 *	Contains the opcode data structure
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#include <stdlib.h>
#include <stdio.h>
#include "lang.h"

int add_opcode(opcode_list * list, opcode_bits data, int flag)
{
	opcode_node * temp;
	int address_ret; /*address that would be returned*/

	if (list!=NULL && (list->count<(INITIAL_ADDRESS+MAX_OPS))) /* check pointer is no NULL, AND we did not exceed max num of ops*/
	{
		temp=new_opcode_node(); /*make new node*/
		if (IS_EMPTY(list))
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

opcode_node * get_opcode_by_addr(opcode_list * list, int addr)
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

int flush_list(opcode_list * list)
{
	opcode_node * curr, next;
	int fc; /*free count*/
	if (list!=NULL)
	{
	
		if (!IS_EMPTY(list)) /* not an empty list*/
		{
			curr=list->head;
			next=curr->next;
			while (next

		} 
	free(list);
	} else return (0); /*flush failed, nothing to fail*/
/* interate through list*/
	/*free node by node*/
	/*careful not to free a NULL, use also count variable*/
	/*free list pointer*/
}

opcode_node * new_opcode_node()
{
	opcode_node * temp;
	temp=(opcode_node*)malloc(opcode_node);
	temp->next=NULL;
	return (temp);
}

opcode_list * new_opcode_list(int start_addr)
{
	opcode_list * temp;
	temp=(opcode_list*)malloc(opcode_list);
	temp->count=0;
	temp->next_addr=start_addr;
	temp->head=temp->tail=NULL;
	return (temp);
}


