/*
 * opcode.h
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#ifndef OPCODE_H_
#define OPCODE_H_

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
	opcode_node * next;
} opcode_node;

typedef struct opl {
	int count;
	int next_addr;
	opcode_node * head;
	opcode_node * tail;
} opcode_list;

int add_opcode(opcode_list * list, opcode_bits data, int flag);
int get_opcode_by_addr(opcode_list * list, int addr);
int delete_opcode_by_addr(opcode_list * list, int addr);
int flush_list(opcode_list * list);
opcode_node * new_opcode_node();


#endif /* OPCODE_H_ */
