/*
 * parse.c
 *
 *  Created on: Mar 24, 2013
 *      Author: yigalalexander
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lang.h"
#include "opcode.h"
#include "convert.h"
#include "symbol.h"

void first_pass(FILE input, opcode_list * target, symbol_list * entries,
		symbol_list * externs) {
	/* init IC & DC */
	/* get line*/
	/* Is it: A symbol, Psuedo intruction,*/
}

int read_line(FILE *INPUT_PROGRAM, char *line) {
	char c = '\0';
	int index = 0;
	while ((c = getc(INPUT_PROGRAM)) != EOF && c != '\n') {
		line[index++] = c;
	}
	line[index] = '\0';
	line_pos += 1;
	if (c == EOF) /*anymore lines to read?*/
		return 1;
	else
		return 0;
}

void write_object_file(char *fname, opcode_list * oplist) {
	opcode_node *temp;
	char *target_file;

	temp = oplist->head;

	target_file = (char *) malloc((strlen(fname) + 5) * sizeof(char));
	if (target_file == NULL ) {
		printf(ERR_MEMORY_LOCATION_FAILURE, line_pos);
		exit(1);
	}
	strcpy(target_file, fname);
	strcat(target_file, ".ob");
	ASSEMBLY_TABLE = fopen(target_file, "w");
	/*First print code operations and the instructions*/
	while (temp) {
		if (temp->location == TBL_CODE) {
			fputs("\t", ASSEMBLY_TABLE);
			fputs(temp->doeznAddr, ASSEMBLY_TABLE);
			fputs("\t", ASSEMBLY_TABLE);
			fputs(temp->doeznMachineCode, ASSEMBLY_TABLE);
			fprintf(ASSEMBLY_TABLE, "\t%c\n", temp->mark);
		}
		temp = temp->next;
	}
	temp = headAssemblyTable;
	while (temp) {
		if (temp->location == TBL_DATA) {
			fputs("\t", ASSEMBLY_TABLE);
			fputs(temp->doeznAddr, ASSEMBLY_TABLE);
			fputs("\t", ASSEMBLY_TABLE);
			fputs(temp->doeznMachineCode, ASSEMBLY_TABLE);
			fprintf(ASSEMBLY_TABLE, "\t%c\n", temp->mark);
		}
		temp = temp->next;
	}
	fclose(ASSEMBLY_TABLE);
}

void write_target_file(char* fname, symbol_list * list) {
	int flag = 0;
	symbol_node * curr;
	char *target_file;
	FILE * TARGET;

	curr = list->head;

	target_file = (char *) malloc((strlen(fname) + 5) * sizeof(char));
	if (target_file == NULL ) {
		printf(ERR_MEMORY_LOCATION_FAILURE, line_pos);
		exit(1);
	}
	strcpy(target_file, fname);
	strcat(target_file, ".ext");

	while (curr) {
		if (!flag) {
			 TARGET = fopen(target_file, "w");
			flag = 1;
		}
		if (curr->location == EXTERN) {
			fputs("\t", TARGET);
			fputs(curr->name, TARGET);
			fputs("\t", TARGET);
			fputs(int2other(curr->dec_value), TARGET);
			fprintf(TARGET, "\n");
		}
		curr = curr->next;
	}
	fclose(TARGET);
}

void write_entry_file(symbol_list * entry_list, char *fname) {
	int flag = 0;
	symbol_node * curr;
	char *target_file;
	FILE * TARGET;
	curr = entry_list->head;

	target_file = (char *) malloc((strlen(fname) + 5) * sizeof(char));
	if (target_file == NULL ) {
		printf(ERR_MEMORY_LOCATION_FAILURE, line_pos);
		exit(1);
	}
	strcpy(target_file, fname);
	strcat(target_file, ".ent");

	while (curr) {
		if (curr->location == ENTRY) {
			if (!flag) {
				TARGET = fopen(target_file, "w");
				flag = 1;
			}
			fputs("\t", TARGET);
			fputs(curr->name, TARGET);
			fputs("\t", TARGET);
			fputs(int2other(curr->dec_value), TARGET);
			fprintf(TARGET, "\n");
		}
		curr = curr->next;
	}
	fclose(TARGET);
}

void second_pass(FILE input, opcode_list * target, symbol_list * entries, symbol_list * externs)
{}
