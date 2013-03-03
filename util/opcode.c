/*
 * opcode.c
 *	Contains the opcode data structure
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#include <stdlib.h>
#include <stdio.h>

int add_opcode(opcode_node * list, int address, opcode_bits data, int flag)
{
	/* check pointer is no NULL*/
		/*cases, if only
}
int get_opcode_by_addr(opcode_node * list, int addr);
int delete_opcode_by_addr(opcode_node * list, int addr);
int flush_list(opcode_node * list);
opcode_node * new_opcode_node();

int flush_list(opcode_node * list)
{
	if (list!=NULL)
	{
		/* for each node free it, use a temp pointer to the next*/
		opcode_node * temp;

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

