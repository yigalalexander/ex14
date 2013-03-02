/*
 * opcode.c
 *	Contains the opcode data structure
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

typedef struct {
	unsigned int comb:2;
	unsigned int dst_reg:3;
	unsigned int dst_add:2;
	unsigned int src_reg:3;
	unsigned int src_add:2;
	unsigned int command:4;
	unsigned int type:1;
	unsigned int reserve:3;
}opcode_bits;

typedef struct {
	int addr; /*opcode address*/
	opcode_bits bits; /*bits of information*/
	int flag; /*type of command*/
	void * next;
} opcode_node;

