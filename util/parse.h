/*
 * parse.h
 *
 *  Created on: Mar 24, 2013
 *      Author: yigalalexander
 */

#ifndef PARSE_H_
#define PARSE_H_

char * get_first_word (char *);

/*
 * Receives an input files to run the first parsing pass over the file
 * @input - input file, must be opened
 * @target - opcode table
 * @entries - entry table
 * @externs - extern table
 */
void first_pass(FILE input,opcode_list * target, symbol_list * entries, symbol_list * externs); /*add parameters here!*/
void second_pass(FILE input,opcode_list * target, symbol_list * entries, symbol_list * externs); /*add parameters here!*/
void write_opcode_table_to_file(opcode_list * table,FILE main,FILE entries,FILE externs);

#endif /* PARSE_H_ */
