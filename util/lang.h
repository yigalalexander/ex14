/*
 * lang.h
 *
 *  Created on: Mar 3, 2013
 *  Author: Yigal Alexander & Alex Maximov
 *  Describes the language we will be parsing
 */

#ifndef LANG_H_
#define LANG_H_

#define INITIAL_ADDRESS 100
#define MAX_OPS 2000

#define OPER1_LENGTH 5
#define OPER2_LENGTH 9
#define OPER3_LENGTH 2
#define MAX_LABEL_LENGTH 30
#define MAX_LINE_LENGTH 80

#define DATA_INSTRUCTION "data"
#define STRING_INSTRUCTION "string"
#define ENTRY_INSTRUCTION "entry"
#define EXTERN_INSTRUCTION "extern"

typedef enum{IMMIDATE_ADDR,
	DIRECT_ADDR,
	INDEX_ADDR,
	INDEX_REG_ADD,
	DIRECT_REG_ADDR,
	NA
} addr_methods; /*addressing methods*/

typedef struct{
	char commandName[4];
	int AllowdMethodsSrc[5];
	int AllowdMethodsDest[5];
} command_methods;

static command_methods methods[]= /*legal source and destination methods*/
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
		{"STOP",{0,0,0,0,0},{0,0,0,0,0}}
};

static char* OperationType1[OPER1_LENGTH]={"MOV", "CMP", "ADD", "SUB", "LEA"};
static char* OperationType2[OPER2_LENGTH]={"INC", "DEC", "JMP", "BNE", "RED", "PRN", "JSR", "CLR","NOT"};
static char* OperationType3[OPER3_LENGTH]={"RTS", "STOP"};

#define ERR_MEMORY_ALLOCATION_FAILURE "Error: Cannot allocate memory - exiting program, occurred in line %d \n"
#define ERR_MEMORY_ALLOCATION_GEN "Error: Cannot allocate memory - exiting program"

/*File manipulation related errors*/
#define ERR_FILE_MISSING_OPERAND "Error: No parameters were expected - exiting program \n"
#define ERR_FAILED_TO_OPEN_FILE "Error: Failed to open file - exiting program \n"

/*Operation errors*/
#define ERR_MISSING_OPERAND "Error: Internal error in line %d\n"
#define ERR_COMMAND_SYNTAX_ERROR "Error: Internal error in line %d\n"
#define ERR_INVALID_COMMAND_NAME "Error: Invalid Command name %d\n"
#define ERR_EXTRA_OPERAND "Error: There are extra operands  in line %d\n"

/*Addressing errors*/
#define ERR_INVALID_DIRECT_ADDR "Error: Invalid Direct Addressing label in line %d\n"
#define ERR_INVALID_LABEL "Error:invalid label in line %d\n"
#define ERR_INVALID_OPERAND_NO_REG "Error: 3rd operand is not a register as expected in line %d\n"
#define ERR_INVALID_ADDRESSING_METHOD "Error in line  %d: Addressing method is not appropriate for the  command \n"

/*Instruction errors*/
#define ERR_INVALID_INSTRUCTION "Error: Missing an instruction after the '.' ,in line %d\n"
#define ERR_INVALID_DATA_MEMBER "Error: Data instruction contains an invalid number , in line %d"
#define ERR_MISSING_DATA_MEMBERS "Error: Missing parameters after the data instruction, in line %d"
#define ERR_INVALID_INSTRUCTION_SYNTAX "Error: A syntax error was found, in line %d"

/*Symbol errors*/
#define ERR_DUPLICATE_SYMBOL "Error: Symbol in line %d already exists\n "
#define ERR_SYMBOL_NOT_EXIST "Error: Symbol in line %d doesn't exists not in the code nor the data\n "
#define ERR_SYMBOL_NOT_EXIST_NAME "Error: Symbol not defined in line %d\n "
#define ERR_INVALID_SYMBOL "Error: Invalid symbol in line %d\n"

#define ERR_INVALID_FILE "Failed to open the requested file."
#define ERR_INVALID_OPERAND "Error: Invalid name operand %d\n"
#endif /* LANG_H_ */
