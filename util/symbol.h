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

int symbol_exists_in (symbol_node * list, char * symbol); /*check if 'symbol' exists in 'list' and returns the address of it, or 0*/
int get_symbol_addr (symbol_node * list, char * symbol); /* might actually the same as above, may remove one of them */
int add_symbol (symbol_node * list, char * symbol, int address); /*add a new symbol with a name and an address*/
int delete_symbol_by_addr (symbol_node * list, int address); /* delete a symbol by the address*/
int delete_symbol_by_name (symbol_node * list, char * name); /* same as above, but by the name */
int flush_symbols (symbol_node * list); /*flushes a symbol list*/

#endif /* SYMBOL_H_ */
