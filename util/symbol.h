/*
 * symbol.h
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#define DUPLICATE_SYMBOL (-1)

typedef struct symbol {
	int addr; /*assigned address of the symbol*/
	char *name; /*symbol name*/
	struct symbol *next;
} symbol_node;

typedef struct sym_list {
	symbol_node *head;
	symbol_node *tail;
	int count;
} symbol_list;

symbol_list * new_symbol_list();
int symbol_exists_in (symbol_list * list, char * symbol); /*VV check if 'symbol' exists in 'list' and returns the address of it, or 0*/
int add_symbol (symbol_list * list, char * symbol, int address); /*VV add a new symbol with a name and an address*/
int delete_symbol_by_addr (symbol_list * list, int address); /* delete a symbol by the address*/
int delete_symbol_by_name (symbol_list * list, char * name); /* same as above, but by the name */
int flush_symbols (symbol_list * list); /*VV flushes a symbol list*/


#endif /* SYMBOL_H_ */
