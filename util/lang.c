/*
 * lang.c
 *
 *  Created on: Mar 3, 2013
 *      Author: yigalalexander
 */


#define HELLO 0

#define OPER1_LENGTH 5
#define OPER2_LENGTH 9
#define OPER3_LENGTH 2

char* OperationType1[OPER1_LENGTH]={"MOV", "CMP", "ADD", "SUB", "LEA"};
char* OperationType2[OPER2_LENGTH]={"INC", "DEC", "JMP", "BNE", "RED", "PRN", "JSR", "CLR","NOT"};
char* OperationType3[OPER3_LENGTH]={"RTS", "STOP"};

int nothing()
{
	return (0);
}
