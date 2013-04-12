/*
 * data_st.h
 *
 *  Created on: Apr 12, 2013
 *      Author: yigalalexander
 */

#ifndef DATA_ST_H_
#define DATA_ST_H_


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

typedef struct opn opcode_node;

struct opn {
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
};

typedef struct opl {
	int count;
	int next_addr;
	opcode_node * head;
	opcode_node * tail;
} opcode_list;
#define DUPLICATE_SYMBOL (-1)

typedef enum
{
	DATA,CODE
} symbol_type;

typedef enum
{
	ENTRY, EXTERN, LOCAL
} symbol_location;

typedef struct symbol {
	int addr; /*assigned address of the symbol*/
	char *name; /*symbol name*/
	int dec_value;
	char * base4_value;
	symbol_location location; /* where does it belong */
	symbol_type type;
	symbol_node * next;
} symbol_node;

typedef struct sym_list {
	symbol_node * head;
	symbol_node * tail;
	int count;
} symbol_list;

void add_opcode(opcode_list *list, opcode_node * new); /* add a new opcode_node to the list */
opcode_node * get_opcode_by_addr(opcode_list * list, int addr); /*receives a list, and an address and returns a point to the relevant opcode entry*/
int flush_opcode_list(opcode_list * list); /*flushes a list from the memory*/
opcode_node * new_opcode_node();
opcode_list * new_opcode_list(int start_addr); /*Create new opcode list item and return a pointer to it*/

symbol_list * new_symbol_list();
symbol_node * symbol_exists_in (symbol_list * list, char * symbol); /*VV check if 'symbol' exists in 'list' and returns the address of it, or 0*/
int add_symbol (symbol_list * list, char * symbol, int address,symbol_type sym_type,symbol_location sym_loc); /*VV add a new symbol with a name and an address*/
int flush_symbols (symbol_list * list); /*VV flushes a symbol list*/
#endif /* DATA_ST_H_ */
