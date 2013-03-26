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



void second_pass(FILE input, opcode_list * target, symbol_list * entries, symbol_list * externs)
{}
