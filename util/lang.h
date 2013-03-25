/*
 * lang.h
 *
 *  Created on: Mar 3, 2013
 *      Author: yigalalexander
 */

#ifndef LANG_H_
#define LANG_H_

#define INITIAL_ADDRESS 100
#define MAX_OPS 2000

typedef enum{IMMIDATE_ADDR,
	DIRECT_ADDR,
	INDEX_ADDR,
	INDEX_REG_ADD,
	DIRECT_REG_ADDR,
	NA
} typesOfAddrMethods;

typedef struct{
	char commandName[4];
	int AllowdMethodsSrc[5];
	int AllowdMethodsDest[5];
} CommandsMethods;

static CommandsMethods methods[]= /*allowed source and destination methods*/
{
		{"MOV",{1,1,1,1,1},{1,1,1,1,1}},
		{"CMP",{1,1,1,1,1},{1,1,1,1,1}},
		{"ADD",{1,1,1,1,1},{0,1,1,1,1}},
		{"SUB",{1,1,1,1,1},{0,1,1,1,1}},
		{"LEA",{0,1,1,1,0},{0,1,1,1,1}},
		{"INC",{0,0,0,0,0},{0,1,1,1,1}},
		{"DEC",{0,0,0,0,0},{0,1,1,1,1}},
		{"JMP",{0,0,0,0,0},{0,1,1,1,1}},
		{"BNE",{0,0,0,0,0},{0,1,1,1,1}},
		{"RED",{0,0,0,0,0},{0,1,1,1,1}},
		{"PRN",{0,0,0,0,0},{1,1,1,1,1}},
		{"JSR",{0,0,0,0,0},{0,1,0,0,0}},
		{"NOT",{0,0,0,0,0},{0,1,1,1,1}},
		{"CLR",{0,0,0,0,0},{0,1,1,1,0}},
		{"RTS",{0,0,0,0,0},{0,0,0,0,0}},
		{"HLT",{0,0,0,0,0},{0,0,0,0,0}}
};

#endif /* LANG_H_ */
