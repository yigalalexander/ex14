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



void first_pass(FILE input, parsing_globals data)
{
  /* get line*/
	/* Is it: A symbol, Pseudo instruction,*/

	int i,temp,len;
	char *label=NULL;
	char line[80];/*line of text that will be read*/
	int isEOF=0;
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
			data.errors_found+=1;
			printf(ERR_INVALID_SYMBOL,addressing_validate_match(data)); /*    <<<<<<<<<<<<<<------------ needs attention */
		}
		else
		{
			while(isspace(temp=line[i++])); /*Skip white-spaces*/
			i--;
			if(line[i]=='.')/*Check if contains instruction*/
				instruction_parse(data.code_table,data.symbol_table,label,line+i);
			else
				general_operation_parse(data.code_table,data.symbol_table,label,line+i);

		}
		data.line_pos++;
	}while(!isEOF);
}



int read_line(FILE *INPUT_PROGRAM, char *line)
{
	char c = '\0';
	int index = 0;
	while ((c = getc(INPUT_PROGRAM)) != EOF && c != '\n') {
		line[index++] = c;
	}
	line[index] = '\0';
	if (c == EOF) /*anymore lines to read?*/
		return (1);
	else
		return (0);
}

void second_pass(parsing_globals data)
{

	opcode_node * dest_op_node;
	opcode_node * temp=data.code_table->head;
	symbol_node * temp_sym=data.symbol_table->head;

	while(temp)
	{
		/*temp->=int2other(temp->addr,4); ----- OLD line */
		if(temp->base4code !=NULL && !(strcmp(temp->base4code,"?")))/*Check if need to update the address*/
		{
			if((temp_sym=symbol_exists_in(data.symbol_table,temp->base2code)))
			{
				if(temp_sym->location==EXTERN)/*Enter '0' to Binary machine code*/
				{
					temp->base2code= int2other(0,2,0);
					temp->mark='e';
					if(temp_sym->dec_value==0)
						temp_sym->dec_value=temp->addr;
					temp_sym->base4_value = int2other(temp_sym->dec_value,4);
				}
				else
				{
					/*Check if need the the distance label or the address*/
					if(temp->arguments!=NULL && !(strcmp(temp->arguments,"*")))/*Indicates that it is "distance label"*/
					{
						while(dest_op_node && dest_op_node->addr!=temp_sym->dec_value)
							dest_op_node=dest_op_node->next;
						temp->base2code= dest_op_node->base2code;
					}
					else
					{
						temp->base2code = int2other(temp_sym->dec_value,2);
					}

				}
			}
			else
			{
				data.errors_found+=1;
				printf(ERR_SYMBOL_NOT_EXIST_NAME ,data.line_pos);
			}
		}
		temp->base4code=int2other(bin2int(temp->base2code),4);
		temp=temp->next;
	}
}


char* is_valid_number(parsing_globals data,char *string)
{
	int i=0,j=0;
	int temp,sum=0;
	char *fourNum;
	if(string[i]=='-' || string[i]=='+' )
	{
		i++;
	}
	for(;string[i]!='\0';i++)
	{
		if(string[i]<'0' || string[i]>'9')
		{
			printf(ERR_INVALID_DIRECT_ADDR,data.line_pos);
			data.errors_found +=1;
			return NULL;
		}
	}
	/*Convert to negative number 16 byets*/
	if(string[0]=='-')
	{
		temp=strlen(string);
		for(i=1;i<temp;i++)
			sum=sum*10+(string[i]-'0');
		string=int2other(sum,2,0);
		temp=strlen(string);
		for(i=0;i<temp;i++)
			string[i]=((string[i]-'0')^1)+'0';
		temp=bin2int(string);
		temp+=1;
		string=int2other(temp,2,0);
		temp=strlen(string);
		fourNum= (char *)malloc(17*sizeof(char));
		for(i=0;i<16;i++)
		{
			if(i<(16-temp))
			{
				fourNum[i]='1';
			}
			else
				fourNum[i]=string[j++];
		}
		fourNum[16]='\0';
		return fourNum;
	}
	if(string[0]=='+')
		return string+1;
	return string;
}

addr_methods type_of_addressing(parsing_globals data,char *lbl,char **externalLbl, char **internalLbl1,char **internalLbl2)
{
	int i=0,j=0;
	char temp[31];

	if(strlen(lbl) == 2 ) /* is it a register?*/
		if(lbl[0] == 'r'  && lbl[1]>='0' && lbl[1]<='7')/*check if fregistar*/
		{
			return DIRECT_REG_ADDR;
		}

	/*end of check if register*/


	/*Check if direct addresing	*/
	if(lbl[0]=='#')
	{
		if((*externalLbl=(isValidNumber(lbl+1)))==NULL)
		{
			return NA;
		}
		else
			return IMMIDATE_ADDR;
	}

	*externalLbl=NULL;

	for(i=0;lbl[i]!='\0' && lbl[i]!='[';i++) /*is it 1,2 or 3? */
		temp[i]=lbl[i];
	temp[i]='\0';
	j=IsValidLabel(temp,externalLbl);
	if(externalLbl==NULL)
	{
		printf(ERR_INVALID_LABEL,data.line_pos);
		data.errors_found +=1;
		return NA;
	}
	if(lbl[j]=='\0')/*Found label*/
	{
		return DIRECT_ADDR;
	}

	if(lbl[j++]=='[')/*Check if 2/3*/
	{
		for(i=0;lbl[j]!='\0' && lbl[j]!=']';i++)
			temp[i]=lbl[j++];
		temp[i]='\0';
		if(lbl[j]=='\0')/*Error no close barket*/
		{
			printf(ERR_INVALID_LABEL,data.line_pos);
			data.errors_found++;
			return NA;
		}
		if(temp[0]=='*')/*if Offset method*/
		{
			if(IsValidLabel(temp+1,internalLbl1)==-1)
			{
				printf(ERR_INVALID_LABEL,data.line_pos);
				data.errors_found++;
				return NA;
			}
		}
		else
		{
			if(IsValidLabel(temp,internalLbl1)==-1)
			{
				printf(ERR_INVALID_LABEL,data.line_pos);
				data.errors_found++;
				return NA;
			}
		}
		if(lbl[j]==']') /*Checking if other ] exists*/
		{
			j++;
			if(temp[0]=='*' )/*Check if method #2*/
			{
				if(lbl[j]=='\0')
				{
					return INDEX_ADDR;
				}
				else
				{
					printf(ERR_INVALID_LABEL,data.line_pos);
					data.errors_found =+1;
					return NA;
				}
			}
			if(lbl[j++]=='[')/*Check if 3 METHOUD*/
			{
				for(i=0;lbl[j]!='\0' && lbl[j]!=']';i++)
					temp[i]=lbl[j++];
				temp[i]='\0';
				if(lbl[j]=='\0')/*unclosed brackets*/
				{
					printf(ERR_INVALID_LABEL,data.line_pos);
					data.errors_found++;
					return NA;
				}
				/*=================
						  Check if register
						  =================*/

				if(strlen(temp) == 2 )
				{
					if(temp[0] != 'r'  || temp[1]<'0' || temp[1]>'7')/*check if fregistar*/
					{
						printf(ERR_INVALID_LABEL,data.line_pos);
						data.errors_found++;
						return NA;
					}
					else/*all is good*/
					{
						return INDEX_REG_ADD;
					}
				}
			}
		}
		/*error third operand is not a register*/
		printf(ERR_INVALID_OPERAND_NO_REG,data.line_pos);
		data.errors_found =+1;
		return NA;
	}
	printf(ERR_INVALID_OPERAND_NO_REG,data.line_pos);
	data.errors_found =1;
	return NA;
}

int addressing_validate_match(parsing_globals data,int cmdIndex,addr_methods typeAddr,int numOper)
{
	if(numOper==1)
	{
		if(methods[cmdIndex].AllowdMethodsSrc[typeAddr]==0)
		{
			data.errors_found+=1;
			printf(ERR_INVALID_ADDRESSING_METHOD, data.line_pos);
			return 0;
		}
	}
	else
	{
		if(methods[cmdIndex].AllowdMethodsDest[typeAddr]==0)
		{
			data.errors_found+=1;
			printf(ERR_INVALID_ADDRESSING_METHOD, data.line_pos);
			return 0;
		}
	}
	return 1;
}

void validate_addr_add_table(parsing_globals data,int index, char* lblSource, char * lblDest,int operationType)
{
	int i=0,len,j=0;
	addr_methods  typeAddr1, typeAddr2;
	char *exteranlLbl=NULL,*internalLbl1=NULL,*internalLbl2=NULL,*exteranlLbl2=NULL,*internalLbl21=NULL,*internalLbl22=NULL,*machineCode;
	opcode_node *newitem=NULL;
	char *temp;

	if(operationType == 1)/*if its operation type 1(contains label source) -> Analyse label source*/
	{
		if((typeAddr1=type_of_addressing(data,lblSource,&exteranlLbl,&internalLbl1,&internalLbl2))==NA)
		{
			return;
		}
		else
		{
			if(addressing_validate_match(data,index,typeAddr1,1)==0)/*Error- Non matching allowed addressing method*/
			{
				return;
			}
		}
	}

	if((typeAddr2=type_of_addressing(data,lblDest,&exteranlLbl2,&internalLbl21,&internalLbl22))==NA)/*Analyze label destination*/
	{
		return;
	}
	else
	{
		if(operationType==1)
		{
			if(addressing_validate_match(data,index,typeAddr2,2)==0)/*Error- Non matching allowed addressing method*/
				return;
		}
		else
		{
			if(addressing_validate_match(data,index+OPER1_LENGTH,typeAddr2,2)==0)/*Error- Non matching allowed addressing method*/
				return;
		}
	}

	machineCode=(char *)malloc(17*sizeof(char));
	if(machineCode==NULL)
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
		data.errors_found+=1;
		exit(1);
	}
	for(i=0;i<16;i++)
		machineCode[i]='0';
	machineCode[16]='\0';
	/*0-2 Dest reg*/
	if(typeAddr2==DIRECT_REG_ADDR)/*Put in value only if register*/
	{
		temp=int2other(lblDest[1]-'0',2,0);
		len=strlen(temp);
		for(i=len-1;i>=0;i--)
			machineCode[i]=temp[j++];
	}

	/*3-5 Method of Dest addressing	*/
	j=0;
	temp=int2other(typeAddr2,2,0);
	len=strlen(temp);
	for(i=2+len;i>=3;i--)
		machineCode[i]=temp[j++];
	/*The following is relevant only for operation type 1*/
	if(operationType==1)
	{
		/*6-8 Src reg*/
		if(typeAddr1==DIRECT_REG_ADDR)
		{
			j=0;
			temp=int2other(lblSource[1]-'0',2,0);
			len=strlen(temp);
			for(i=5+len;i>=6;i--)
				machineCode[i]=temp[j++];
		}
		/*9-11 Method of Source addressing*/
		j=0;
		temp=int2other(typeAddr1,2,0);
		len=strlen(temp);
		for(i=8+len;i>=9;i--)
			machineCode[i]=temp[j++];
	}

	/*12-15 Operand Code*/
	j=0;
	if(operationType==1)
		temp=int2other(index,2,0);
	else if(operationType==2)
		temp=int2other(index+OPER1_LENGTH,2,0);
	len=strlen(temp);
	for(i=11+len;i>=12;i--)
		machineCode[i]=temp[j++];


	newitem=(opcode_node  *)malloc(sizeof(opcode_node));
	if(newitem==NULL)/*Check if memory error*/
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
		exit(1);
	}

	newitem->addr = data.IC++;
	if(operationType ==1)
		temp=(char *)malloc((strlen(methods[index].commandName)+1)*sizeof(char));
	else
		temp=(char *)malloc((strlen(methods[index+OPER1_LENGTH].commandName)+1)*sizeof(char));/*for Operation type 2 set the command acoording to index+ operation1 length*/
	if(!temp) /*for malloc allocation error exit program*/
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
		exit(1);
	}
	if(operationType==1)
	{
		strcpy(temp,methods[index].commandName);
	}
	else
		strcpy(temp,methods[(index+OPER1_LENGTH)].commandName);
	newitem->command=temp;
	/*end of Handle the command*/
	newitem->base2code	=machineCode;/*add the bin Machine Code created above to the node*/
	newitem->base4code = int2other(bin2int(machineCode),4);

	if(operationType==1)/*Creating the arguments symbol in the main node */
	{
		temp=(char *)malloc((strlen(lblSource)+strlen(lblDest)+2)*sizeof(char));
		strcpy(temp,lblSource);
		temp[strlen(lblSource)]=',';
		strcpy(temp+strlen(lblSource)+1,lblDest);
		newitem->arguments=temp;
	}
	else
		newitem->arguments=lblDest;
	newitem->label=data.first_label;
	newitem->mark='a';
	newitem->location=CODE;
	newitem->next = NULL;
	add_opcode(data.code_table,newitem);

	if(data.first_label)
		add_symbol(data.symbol_table,data.first_label,data.IC-1,CODE,LOCAL);

	/*Handle additional addressing methods for both arguments*/
	if(operationType==1)
		if(typeAddr1!=DIRECT_REG_ADDR)
		{
			handle_rest_of_labels(data,typeAddr1,exteranlLbl,internalLbl1);
		}
	if(typeAddr2!=IMMIDATE_ADDR && typeAddr2!=DIRECT_REG_ADDR)
		handle_rest_of_labels(data,typeAddr2,exteranlLbl2,internalLbl21);
}

void handle_rest_of_labels(parsing_globals data,addr_methods type,char * exteranlLbl,char *internalLbl)
{
	int i=1,j=0;/*Indicates how many node we have to add to the Main Assembly table*/
	opcode_node * newitem;

	if(internalLbl!=NULL)
		i++;
	for(;j<i;j++)
	{
		newitem=(opcode_node *)malloc(sizeof(opcode_node));
		if(newitem==NULL)/*Check if memory error*/
		{
			printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
			exit(1);
		}
		newitem->label=NULL;
		newitem->addr=data.IC++;
		newitem->command=NULL;
		newitem->mark='r';
		newitem->arguments= NULL;
		newitem->base2code=exteranlLbl;
		if(j==1)
		{
			if(type==INDEX_ADDR)/*If its a direct index method(#2)*/
			{
				newitem->mark='a';
				newitem->arguments= (char *)malloc(2*sizeof(char));
				strcpy(newitem->arguments,"*");/*This is for iteration 2 - indicates that it is a label that is used as data*/
			}
		}
		if( type!=IMMIDATE_ADDR)
		{
			if(j==0 )
			{
				newitem->base2code = (char *)malloc(strlen(exteranlLbl) * sizeof(char));
				newitem->base2code =exteranlLbl;
			}
			else
			{
				newitem->base2code = (char *)malloc(strlen(internalLbl) * sizeof(char));
				newitem->base2code =internalLbl;
			}
		}
		if(type!=IMMIDATE_ADDR)/*Handle first methoud(immidate-0)seperatly, meaning no need to enter memeory address after */
		{
			newitem->base4code= (char *)malloc(2*sizeof(char));
			strcpy(newitem->base4code,"?");
		}
		else
		{
			newitem->mark='a';
			newitem->base4code=NULL;
		}
		newitem->location=CODE;
		newitem->next=NULL;
		add_opcode(data.code_table,newitem);
	}
}

void update_tbl_addr(parsing_globals data) /*update_tbl_addr*/
{
	symbol_node * temp=data.symbol_table->head;
	opcode_node * tempAssm=data.code_table->head;
	while(temp)
	{
		if(temp->location!=EXTERN)
		{
			if(temp->type== DATA)
			{
				temp->dec_value +=data.IC;
				temp->base4_value=int2other(temp->dec_value ,4);
			}
		}
		if(temp->location==EXTERN)
			temp->base4_value=int2other(temp->dec_value ,4);
		temp=temp->next;
	}
	while(tempAssm)
	{
		if(tempAssm->location == DATA)
		{
			tempAssm->addr  +=data.IC;
		}
		tempAssm=tempAssm->next; /*advance forward */
	}

}

int determine_operation_type(char* str,int *index)
{
	int i;
	for(i=0;i<OPER1_LENGTH;i++)
		if(!(strcmp(OperationType1[i],str)))
		{
			*index=i;
			return 1;
		}
	for(i=0;i<OPER2_LENGTH;i++)
		if(!(strcmp(OperationType2[i],str)))
		{
			*index=i;
			return 2;
		}
	for(i=0;i<OPER3_LENGTH;i++)
		if(!(strcmp(OperationType3[i],str)))
		{
			*index=i;
			return 3;
		}

	return 0;
}

void general_operation_parse(parsing_globals data,char* label, char* Command)
{

	int typeOfCommand,index;
	int i=0;
	char *cmdTemp=NULL;

	/*Check type of command*/

	while(Command[i] != ' ' && Command[i] != '\n' && Command[i]!='\t'  && Command[i]!='\0') /*Read till hit a space or a tab or EOL*/
	{
		cmdTemp=(char *)realloc(cmdTemp,(i+1)*sizeof(char));
		cmdTemp[i]=Command[i];
		i++;
	}
	cmdTemp=(char *)realloc(cmdTemp,(i+1)*sizeof(char));
	cmdTemp[i]='\0';
	ConvertToUpper(cmdTemp);

	data.first_label=label;
	if(!(typeOfCommand=determine_operation_type(cmdTemp,&index)))
	{
		printf(ERR_INVALID_COMMAND_NAME,data.line_pos);
		data.errors_found+=1;
		return;
	}
	else if (typeOfCommand==1)
		parse_operation_type1(data,Command +i , index);
	else if(typeOfCommand==2)
		parse_operation_type2(data,Command +i, index);
	else
		parse_operation_type3(data,Command +i , index);
}

void parse_operation_type1(parsing_globals data,char* command, int index)
{
	int temp,i=0;
	char *firstOper=NULL, *secondOper=NULL;

	/*Handling first operand*/
	while(isspace(temp=*command++)); /*Skip white-spaces*/
	command--;
	if(*command=='\n') /*Check if no operands*/
	{
		printf(ERR_MISSING_OPERAND,data.line_pos);
		data.errors_found+=1;
		return;
	}
	while(*command != ' ' && *command != '\n' && *command!='\t' && *command!=',' ) /*Read till hit a space or EOL*/
	{
		firstOper=(char *)realloc(firstOper, (i+1)*sizeof(char));
		firstOper[i++]=*command++;
	}
	firstOper=(char *)realloc(firstOper, (i+1)*sizeof(char));
	firstOper[i]='\0';
	if(*command=='\n')/*Return if reached EOL(Only one operand)*/
	{
		printf(ERR_MISSING_OPERAND, data.line_pos);
		data.errors_found+=1;
		return;
	}

	/*Looking for a ','*/
	while(isspace(temp=*command++)); /*Skip white-spaces*/
	command--;
	if(*command!=',')
	{
		if(*command=='\n')
			printf(ERR_MISSING_OPERAND, data.line_pos);
		else
			printf(ERR_COMMAND_SYNTAX_ERROR, data.line_pos);
		data.errors_found+=1;
		return;
	}
	command++; /*Skip the ','*/
	while(isspace(temp=*command++)); /*Skip white-spaces*/
	command--;
	if(*command=='\n') /*Check if no operands*/
	{
		printf(ERR_MISSING_OPERAND,data.line_pos);
		data.errors_found+=1;
		return;
	}

	/*Handling second operand*/
	i=0;
	while(*command !=' ' && *command!='\t' && *command !='\n' && *command!='\0' ) /*Read till hit a space or tab EOL*/
	{
		secondOper=(char *)realloc(secondOper, (i+1)*sizeof(char));
		if(secondOper==NULL)
		{
			printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
			exit(1);
		}
		secondOper[i++]=*command++;
	}
	secondOper=(char *)realloc(secondOper, (i+1)*sizeof(char));
	if(secondOper==NULL)
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
	}
	secondOper[i]='\0';
	while(isspace(temp=*command++)); /*Skip white-spaces*/
	command--;
	if(*command!='\n'&& *command!='\0')/*Return if didn't reach EOL , meaning extra operands*/
	{
		printf(ERR_EXTRA_OPERAND, data.line_pos);
		data.errors_found+=1;
		return;
	}
	validate_addr_add_table(data.code_table,data.symbol_table,index,firstOper,secondOper,1);
}



void parse_operation_type2(parsing_globals data,char* command, int index)
{
	int temp,i=0;
	char operand[31];
	while(isspace(temp=*command++));
	command--;
	if(*command=='\n')/*error- no operand*/
	{
		data.errors_found +=  1;
		printf(ERR_MISSING_OPERAND,data.line_pos);
		return;
	}
	while(i<30 && command[i]!=' ' && command[i]!='\n' && command[i]!='\0' && command[i]!='\t')
	{
		operand[i]=command[i];
		i++;
	}
	operand[i]='\0';
	if(i==30)/*error - label size invalid*/
	{
		data.errors_found +=  1;
		printf(ERR_MISSING_OPERAND,data.line_pos);
		return;
	}
	if(command[i]!='\0' && command[i]!='\n')
	{
		while(isspace(temp=command[i++])); /*Skip white-spaces*/
		i--;
		if(command[i-1]!='\n' && command[i-1]!='\0' && command[i-1]!='\t')/*error extra opernds*/
		{
			data.errors_found +=  1;
			printf(ERR_INVALID_LABEL,data.line_pos);
			return;
		}
	}
	else
		validate_addr_add_table(data.code_table,data.symbol_table,index,NULL,operand,2);
}

void parse_operation_type3(parsing_globals data,char* command, int index)
{
	opcode_node *  *newitem=NULL;
	int i=0,temp,j=0;
	char *numCmd=NULL;

	while(isspace(temp=*command++)); /*Skip white-spaces - Read till hit EOL*/
	command--;
	if(*command!='\n' && *command!='\0')/*Error- operation type 3 is supposed to not have any operands*/
	{
		data.errors_found+=1;
		printf(ERR_EXTRA_OPERAND,data.line_pos);
		return;
	}

	/*local handling*/
	newitem=(opcode_node *   *)malloc(sizeof(opcode_node * ));
	if(newitem==NULL)/*Check if memory error*/
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
		exit(1);
	}
	newitem->label=data.first_label;
	newitem->addr=data.IC++;
	newitem->command=(char *)malloc((strlen(methods[index+OPER2_LENGTH+OPER1_LENGTH ].commandName)+1)*sizeof(char));
	strcpy(newitem->command,methods[index+OPER2_LENGTH+OPER1_LENGTH].commandName);
	newitem->arguments=NULL;
	newitem->base2code = (char *)malloc(17 * sizeof(char));
	numCmd= int2other(index+OPER2_LENGTH+OPER1_LENGTH,4);
	temp=strlen(numCmd);
	for(i=0;i<4-temp;i++)
		newitem->base2code [i]='0';
	for(;i<4;i++)
		newitem->base2code [i]=numCmd[j++];
	for(i=4;i<16;i++)
		newitem->base2code[i] ='0';
	newitem->base2code[i]='\0';
	newitem->base4code=int2other(bin2int(newitem->base2code),4);
	newitem->mark='a';
	newitem->location=CODE;
	newitem->next=NULL;
	add_opcode(data.code_table,newitem);

	if(data.first_label)
		add_symbol(data.symbol_table,data.first_label,data.IC-1,CODE,LOCAL);
}


void instruction_parse(parsing_globals data,char* label, char* command)
{
	char *instruction;
	int instructionIndex=1;
	command++;
	if(*command=='\n') /*Check if no operands*/
	{
		printf(ERR_INVALID_INSTRUCTION,data.line_pos);
		data.errors_found+=1;
		return;
	}
	while(*command!= ' ' && *command != '\n' && *command!= '\t') /*Read till hit a space or EOL to get the instruction*/
	{
		instruction=(char *)realloc(instruction,(instructionIndex+1)*sizeof(char));
		instruction[instructionIndex-1]=*command++;
		instructionIndex++;
	}
	instruction[instructionIndex-1]='\0';
	if(!strcmp(instruction,DATA_INSTRUCTION))
	{
		parse_data_inst(data,command,instruction,label);
	}
	else
	{
		if(!strcmp(instruction,STRING_INSTRUCTION))
		{
			parse_string_inst(data,command,instruction,label);
		}
		else
			if(!strcmp(instruction,ENTRY_INSTRUCTION))
			{
				parse_enrty_inst(data,command,instruction);
			}
			else
				if(!strcmp(data,instruction,EXTERN_INSTRUCTION))
				{
					parse_extern_inst(data,command,instruction);
				}
				else
					printf(ERR_INVALID_INSTRUCTION,data.line_pos); /*instruction is not defined in the language*/
	}
}

void add_data_to_ocode_table(parsing_globals data,char *label,char *instruction,char *currentArg,char *binaryOp)
{
	opcode_node * newitem=NULL;

	newitem=(opcode_node *   *)malloc(sizeof(opcode_node * ));
	if(newitem==NULL)/*Check if memory error*/
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,data.line_pos);
		exit(1);
	}
	(newitem)->label=label;
	(newitem)->addr=data.DC++;
	(newitem)->command=instruction;
	(newitem)->arguments=currentArg;
	(newitem)->base2code= binaryOp;
	(newitem)->base4code= NULL;
	(newitem)->mark= ' ';
	(newitem)->next=NULL;
	(newitem)->location =DATA;
	add_opcode(data.code_table,newitem);
}


void parse_data_inst(parsing_globals data,char *command,char * instruction,char *label)
{
	int argLen=1,isMinus=0,temp=0;
	char *arg=NULL, *binaryArg=NULL;
	while(*command!='\0')
	{
		while(isspace(temp=*command++)); /*Skip white-spaces*/
		command--;
		if(*command=='\0')
		{
			printf(ERR_MISSING_DATA_MEMBERS,data.line_pos);
			data.errors_found++;
			return;
		}
		/*if label exist - insert it to the symbol table*/
		if(label!=NULL)
		{
			add_symbol(data.symbol_table,label,data.DC,DATA,LOCAL);
		}
		if(*command=='+' || *command=='-')
		{
			if( *command=='-')
				isMinus=1;
			command++;
		}
		/*validating each argument and copy it to send it to the data table*/
		while(*command>='0' && *command<='9')
		{
			arg=(char *)realloc(arg,(argLen+1)*sizeof(char));
			arg[argLen-1]=*command++;
			argLen++;
		}
		arg[argLen]='\0';
		if(argLen==1) /*if no number was found*/
		{
			printf(ERR_MISSING_DATA_MEMBERS, data.line_pos);
			data.errors_found++;
			return;
		}
		/*create int presentation of the current argument*/
		temp=atoi(arg);
		if(isMinus)
		{
			temp=~temp;
			temp+=1;
		}
		binaryArg=ConvertDecToOther(temp,2,0);
		InsertDataToAssemblyTable(label,instruction,arg,binaryArg);

		/*check if the numbers are seperated by ','*/
		while(isspace(temp=*command++)); /*Skip white-spaces*/
		command--;
		if(*command==',')
			command++;
		else
			if(*command!='\0')
			{
				printf(ERR_INVALID_INSTRUCTION_SYNTAX,data.line_pos);
				data.errors_found++;
				return;
			}
	}
}


void parse_string_inst(parsing_globals data,char *command,char * instruction,char *label)
{
	int temp=0;
	char arg[2]={'\0'},*binaryArg=NULL;
	while(isspace(*command++)); /*Skip white-spaces*/
	command--;
	if(*command=='\0')
	{
		printf(ERR_MISSING_DATA_MEMBERS,data.line_pos);
		data.errors_found++;
		return;
	}
	/*if label exist - insert it to the symbol table*/
	if(label!=NULL)
	{
		add_symbol(data.symbol_table,label,data.DC,DATA,LOCAL);
	}
	/*checking the validation of the string argument and send it to the data table*/
	if(*command!='"')
	{
		printf(ERR_INVALID_DATA_MEMBER,data.line_pos);
		data.errors_found++;
		return;
	}
	else
		command++;
	while(*command!='"'&& *command!='\0' && *command!=' ' && *command!=EOF)
	{
		arg[0]=*command++;
		arg[1]='\0';
		temp=(int)arg[0];
		binaryArg=ConvertDecToOther(temp,2,0);
		InsertDataToAssemblyTable(label,instruction,arg,binaryArg);
	}
	/*after sending all the characters insert a null parameter*/
	if((*command++)=='"')
	{
		InsertDataToAssemblyTable(label,instruction,NULL,0);
	}
	else
	{
		printf(ERR_INVALID_DATA_MEMBER,data.line_pos);
		return;
	}
	while(isspace(temp=*command++)); /*Skip white-spaces*/
	command--;
	/*only one string should apearin each instruction*/
	if(*command!='\0')
	{
		printf(ERR_INVALID_INSTRUCTION_SYNTAX,data.line_pos);
		data.errors_found++;
		return;
	}
}


void parse_extern_inst(parsing_globals data,char *command,char * instruction)
{
	char *externLbl=NULL;

	if(IsValidLabel(command,&externLbl)!=-1)
	{
		add_symbol(data.symbol_table,externLbl,0,DATA,EXTERN);
		data.DC++;
	}
	else
	{
		printf(ERR_INVALID_LABEL,data.line_pos);
		data.errors_found+=1;
		return;
	}

}

void parse_enrty_inst(parsing_globals data,char *command,char * instruction)
{
	char *entryLbl=NULL;
	if(IsValidLabel(command,&entryLbl)!=-1)
	{
		add_symbol(data.symbol_table,entryLbl,0,CODE,ENTRY);
	}
	else
	{
		printf(ERR_INVALID_LABEL,data.line_pos);
		data.errors_found+=1;
		return;
	}
}

int IsValidLabel(parsing_globals data,char *command, char **label)
{
	int temp, labelLen=1,index=0;
	while(isspace(temp=command[index++])); /*Skip white-spaces*/
	index--;
	if(command[index]=='\n')
	{
		printf(ERR_MISSING_DATA_MEMBERS,data.line_pos);
		data.errors_found++;
		*label=NULL;
	}
	else
	{
		while(((command[index]>='a') && (command[index]<='z')) || ((command[index]>='A') && (command[index]<='Z')) || ((command[index]>='0') && (command[index]<='9')))
		{
			*label=(char*)realloc(*label,(labelLen+1) * sizeof(char));
			(*label)[labelLen-1]=command[index++];
			labelLen++;
		}
		if(command[index]!='\0') /*the current word is not a label*/
		{
			*label=NULL;
			data.errors_found++;
			printf(ERR_INVALID_LABEL, data.line_pos);
		}
		else /*found a lable*/
		{
			if(labelLen> MAX_LABEL_LENGTH)
			{
				printf(ERR_INVALID_LABEL,data.line_pos);
				data.errors_found++;
				return -1;
			}
			else
			{
				*label=(char*)realloc(*label,(labelLen+1) * sizeof(char));
				(*label)[labelLen-1]='\0';
			}
		}
	}
	return index;
}

void  init_globals(parsing_globals this)
{
	this.code_table=NULL;
	this.symbol_table=NULL;
	this.errors_found=0;
	this.IC=100;
	this.DC=0;
	this.line_pos=0;
}
