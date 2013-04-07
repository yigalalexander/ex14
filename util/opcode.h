/*
 * opcode.h
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#ifndef OPCODE_H_
#define OPCODE_H_
#include "symbol.h"

typedef struct {
	unsigned int comb:2;
	unsigned int dst_reg:3;
	unsigned int dst_add:2;
	unsigned int src_reg:3;
	unsigned int src_add:2;
	unsigned int opcode:4;
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


void add_opcode(opcode_list *list, opcode_node * new); /* add a new opcode_node to the list */
opcode_node * get_opcode_by_addr(opcode_list * list, int addr); /*receives a list, and an address and returns a point to the relevant opcode entry*/
int flush_opcode_list(opcode_list * list); /*flushes a list from the memory*/
opcode_node * new_opcode_node();
opcode_list * new_opcode_list(int start_addr); /*Create new opcode list item and return a pointer to it*/


#endif /* OPCODE_H_ */
