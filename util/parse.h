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

/* d
 * Receives an input files to run the first parsing pass over the file
 * @input - input file, must be opened
 * @target - opcode table
 * @entries - entry table
 * @externs - extern table
 */

void init_globals(parsing_globals this);
int read_line(FILE *INPUT_PROGRAM, char *line);

void first_pass(FILE * input,parsing_globals data); /*add parameters here!*/
void second_pass(parsing_globals data); /*add parameters here!*/
void update_tbl_addr(parsing_globals data); /*update the table addresses after both passes */

char* is_valid_number(parsing_globals data,char *string);
addr_methods type_of_addressing(char *lbl,char **externalLbl, char **internalLbl1,char **internalLbl2);

void general_operation_parse(parsing_globals data,char* label, char* Command);
int determine_operation_type(char* str,int *index);
void parse_operation_type1(parsing_globals data,char* command, int index);
void parse_operation_type2(parsing_globals data,char* command, int index);
void parse_operation_type3(parsing_globals data,char* command, int index);

void instruction_parse(parsing_globals data,char* label, char* command);
void parse_enrty_inst(parsing_globals data,char *command,char * instruction);
void parse_extern_inst(parsing_globals data,char *command,char * instruction);
void parse_string_inst(parsing_globals data,char *command,char * instruction,char *label);
void parse_data_inst(parsing_globals data,char *command,char * instruction,char *label);

void add_data_to_ocode_table(parsing_globals data,char *label,char *instruction,char *currentArg,char *binaryOp);
void update_tbl_addr(parsing_globals data); /*update_tbl_addr*/
void handle_rest_of_labels(parsing_globals data,addr_methods type,char * exteranlLbl,char *internalLbl);
void validate_addr_add_table(parsing_globals data,int index, char* lblSource, char * lblDest,int operationType);
int addressing_validate_match(parsing_globals data,int cmdIndex,addr_methods typeAddr,int numOper);

#endif /* PARSE_H_ */
