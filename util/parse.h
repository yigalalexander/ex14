/*
 * parse.h
 *
 *  Created on: Mar 24, 2013
 *      Author: Yigal Alexander & Alex Maximov
 *
 *  Code for reading an parsing of assembly code
 */

#ifndef PARSE_H_
#define PARSE_H_

typedef struct {
	opcode_list * code_table;
	symbol_list * symbol_table;
	int line_pos, errors_found,IC,DC;
	char * first_label;
} parsing_globals;

/*
 * Receives an input files to run the first parsing pass over the file
 * @input - input file, must be opened
 * @target - opcode table
 * @entries - entry table
 * @externs - extern table
 */

void first_pass(FILE input,parsing_globals data); /*add parameters here!*/
void second_pass(parsing_globals data); /*add parameters here!*/
void update_tbl_addr(parsing_globals data); /*update the table addresses after both passes */
char* is_valid_number(char *string);
addr_methods type_of_addressing(char *lbl,char **externalLbl, char **internalLbl1,char **internalLbl2);
void init_globals(parsing_globals this);


#endif /* PARSE_H_ */
