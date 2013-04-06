/*
 * symbol.h
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

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
	struct symbol *next;
} symbol_node;

typedef struct sym_list {
	symbol_node *head;
	symbol_node *tail;
	int count;
} symbol_list;



symbol_list * new_symbol_list();
symbol_node * symbol_exists_in (symbol_list * list, char * symbol); /*VV check if 'symbol' exists in 'list' and returns the address of it, or 0*/
int add_symbol (symbol_list * list, char * symbol, int address,symbol_type sym_type,symbol_location sym_loc); /*VV add a new symbol with a name and an address*/
int flush_symbols (symbol_list * list); /*VV flushes a symbol list*/


#endif /* SYMBOL_H_ */
