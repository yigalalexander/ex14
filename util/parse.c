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
	/* get line*/
	/* Is it: A symbol, Pseudo instruction,*/

	int i,temp,len;
	char *label=NULL;
	char line[100];
	int isEOF=0;

	InitializeEnvironment();

	do
		{
			label=NULL;
			isEOF=read_line(input,line);
			/*is it an empty or a comment line?*/
			len=strlen(line);
			for(i=0;i<len && (temp=isspace(line[i])) ; i++);
			if(i==strlen(line)) /*Empty line*/
			{
				continue;
			}
			if(line[i]==';') /*Comment line*/
			{
				continue;
			}
			/*check if the first word is a symbol*/
			if((i+=IsSymbolExist(line+i,&label))==-1)
			{
				ErrorFlag+=1;
				printf(AS_INVALID_SYMBOL,currentLine);/*Return Symbol*/
			}
			else
			{
				while(isspace(temp=line[i++])); /*Skip white-spaces*/
				i--;
				if(line[i]=='.')/*Check if contains instruction*/
					InstructionParse(label,line+i);
				else
					OperationParse(label,line+i);

			}
		}while(!isEOF);
	}
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
		return (1);
	else
		return (0);
}

/* draft option - these variables should be either set up globally or in "main" to be persistent. function will be deleted later*/
void  InitializeEnvironment()
{
	symbol_list *new_list;
	opcode_list *new_op_list;
	int ErrorFlag = 0;
	int IC = 100;
	int DC = 0;
	/*int currentLine=0;*/
}


void second_pass(FILE input, opcode_list * target, symbol_list * entries, symbol_list * externs)
{}
