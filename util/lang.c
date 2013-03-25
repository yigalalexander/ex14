/*
 * lang.c
 *
 *  Created on: Mar 3, 2013
 *      Author: yigalalexander
 */


#define HELLO 0

#define OPER1_LEN 5
#define OPER2_LEN 9
#define OPER3_LEN 2

char* OperationType1[OPER1_LEN]={"MOV", "CMP", "ADD", "SUB", "LEA"};
char* OperationType2[OPER2_LEN]={"INC", "DEC", "JMP", "BNE", "RED", "PRN", "JSR", "CLR","NOT"};
char* OperationType3[OPER3_LEN]={"RTS", "HLT"};

int nothing()
{
	return (0);
}
