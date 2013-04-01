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

char * get_first_word (char *);
int line_pos;
int errors_found;
int IC,DC;
char * first_label;

/*
 * Receives an input files to run the first parsing pass over the file
 * @input - input file, must be opened
 * @target - opcode table
 * @entries - entry table
 * @externs - extern table
 */

void first_pass(FILE input,opcode_list * target, symbol_list * entries); /*add parameters here!*/
void second_pass(opcode_list * opcode_table, symbol_list * symbol_table); /*add parameters here!*/
void update_tbl_addr(opcode_list * opcode_table, symbol_list * symbol_table); /*update the table addresses after both passes */
char* is_valid_number(char *string);
addr_methods type_of_addressing(char *lbl,char **externalLbl, char **internalLbl1,char **internalLbl2);


#endif /* PARSE_H_ */
