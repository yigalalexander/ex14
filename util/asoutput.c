/*
 * asoutput.c
 *
 *  Created on: Mar 26, 2013
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

void write_object_file(char *fname, opcode_list * oplist) {
	opcode_node *temp;
	char *target_file;
	FILE * ASSEMBLY_TABLE;

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
		if (temp->location == CODE) {
			fputs("\t", ASSEMBLY_TABLE);
			fputs(int2other(temp->addr,4), ASSEMBLY_TABLE);
			fputs("\t", ASSEMBLY_TABLE);
			fputs( int2other( bits2int(temp->bits),4 ), ASSEMBLY_TABLE);
			fprintf(ASSEMBLY_TABLE, "\t%c\n", temp->mark);
		}
		temp = temp->next;
	}
	temp = oplist->head;
	while (temp) {
		if (temp->location == DATA) {
			fputs("\t", ASSEMBLY_TABLE);
			fputs(int2other(temp->addr,4), ASSEMBLY_TABLE);
			fputs("\t", ASSEMBLY_TABLE);
			fputs( int2other( bits2int(temp->bits),4 ), ASSEMBLY_TABLE);
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
