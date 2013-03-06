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

