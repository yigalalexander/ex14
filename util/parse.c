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



void first_pass(FILE input, opcode_list * target, symbol_list * entries)
{
	/* get line*/
	/* Is it: A symbol, Pseudo instruction,*/

	int i,temp,len;
	char *label=NULL;
	char line[80];
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
			errors_found+=1;
			printf(ERR_INVALID_SYMBOL,addressing_validate_match);
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

void second_pass(opcode_list * opcode_table, symbol_list * symbol_table)
{

	opcode_node * temp=opcode_table->head;
	symbol_node *  sym=symbol_table->head;
	while(temp)
	{
		/*temp->=int2other(temp->addr,4); ----- OLD line */
		if(temp->base4code !=NULL && !(strcmp(temp->base4code,"?")))/*Check if need to update the address*/
		{
			if((sym=DoesNodeExist(temp->base2code)))
			{
				if(sym->location==EXTERN)/*Enter '0' to Binary machine code*/
				{
					temp->base2code= int2other(0,2,0);
					temp->mark='e';
					if(sym->dec_value==0)
						sym->dec_value=temp->decAddr;
					sym->base4_value = int2other(sym->dec_value,12,4);
				}
				else
				{
					/*Check if need the the distance label or the address*/
					if(temp->operands!=NULL && !(strcmp(temp->operands,"*")))/*Indicates that it is "distance label"*/
					{
						while(findDestNode && findDestNode->decAddr!=sym->dec_value)
							findDestNode=findDestNode->next;
						temp->base2code= findDestNode->base2code;
					}
					else
					{
						temp->base2code = int2other(sym->dec_value,2,0);
					}

				}
			}
			else
			{
				errors_found+=1;
				printf(ERR_SYMBOL_NOT_EXIST_NAME ,line_pos);
			}
		}
		temp->base4code=int2other(ConvertBinToDec(temp->base2code),12,8);
		temp=temp->next;
	}
}





char* is_valid_number(char *string)
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
			printf(ERR_INVALID_DIRECT_ADDR,line_pos);
			errors_found +=1;
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

addr_methods type_of_addressing(char *lbl,char **externalLbl, char **internalLbl1,char **internalLbl2)
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
		printf(ERR_INVALID_LABEL,line_pos);
		errors_found +=1;
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
			printf(ERR_INVALID_LABEL,line_pos);
			errors_found++;
			return NA;
		}
		if(temp[0]=='*')/*if Offset method*/
		{
			if(IsValidLabel(temp+1,internalLbl1)==-1)
			{
				printf(ERR_INVALID_LABEL,line_pos);
				errors_found++;
				return NA;
			}
		}
		else
		{
			if(IsValidLabel(temp,internalLbl1)==-1)
			{
				printf(ERR_INVALID_LABEL,line_pos);
				errors_found++;
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
					printf(ERR_INVALID_LABEL,line_pos);
					errors_found =+1;
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
					printf(ERR_INVALID_LABEL,line_pos);
					errors_found++;
					return NA;
				}
				/*=================
						  Check if register
						  =================*/

				if(strlen(temp) == 2 )
				{
					if(temp[0] != 'r'  || temp[1]<'0' || temp[1]>'7')/*check if fregistar*/
					{
						printf(ERR_INVALID_LABEL,line_pos);
						errors_found++;
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
		printf(ERR_INVALID_OPERAND_NO_REG,line_pos);
		errors_found =+1;
		return NA;
	}
	printf(ERR_INVALID_OPERAND_NO_REG,line_pos);
	errors_found =1;
	return NA;
}

int addressing_validate_match(int cmdIndex,addr_methods typeAddr,int numOper)
{
	if(numOper==1)
	{
		if(methods[cmdIndex].AllowdMethodsSrc[typeAddr]==0)
		{
			errors_found+=1;
			printf(ERR_INVALID_ADDRESSING_METHOD, line_pos);
			return 0;
		}
	}
	else
	{
		if(methods[cmdIndex].AllowdMethodsDest[typeAddr]==0)
		{
			errors_found+=1;
			printf(ERR_INVALID_ADDRESSING_METHOD, line_pos);
			return 0;
		}
	}
	return 1;
}

void validate_addr_add_table(int index, char* lblSource, char * lblDest,int operationType)
{
	int i=0,len,j=0;
	addr_methods  typeAddr1, typeAddr2;
	char *exteranlLbl=NULL,*internalLbl1=NULL,*internalLbl2=NULL,*exteranlLbl2=NULL,*internalLbl21=NULL,*internalLbl22=NULL,*machineCode;
	opcode_node *newitem=NULL;
	char *temp;

	if(operationType == 1)/*if its operation type 1(contains label source) -> Analyse label source*/
	{
		if((typeAddr1=type_of_addressing(lblSource,&exteranlLbl,&internalLbl1,&internalLbl2))==NA)
		{
			return;
		}
		else
		{
			if(addressing_validate_match(index,typeAddr1,1)==0)/*Error- Non mathcing allowed addressing method*/
			{
				return;
			}
		}
	}

	if((typeAddr2=type_of_addressing(lblDest,&exteranlLbl2,&internalLbl21,&internalLbl22))==NA)/*Analyse label destination*/
	{
		return;
	}
	else
	{
		if(operationType==1)
		{
			if(addressing_validate_match(index,typeAddr2,2)==0)/*Error- Non mathcing allowed addressing method*/
				return;
		}
		else
		{
			if(addressing_validate_match(index+OPER1_LENGTH,typeAddr2,2)==0)/*Error- Non mathcing allowed addressing method*/
				return;
		}
	}

	machineCode=(char *)malloc(17*sizeof(char));
	if(machineCode==NULL)
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
		errors_found+=1;
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
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
		exit(1);
	}

	newitem->addr = IC++;
	if(operationType ==1)
		temp=(char *)malloc((strlen(methods[index].commandName)+1)*sizeof(char));
	else
		temp=(char *)malloc((strlen(methods[index+OPER1_LENGTH].commandName)+1)*sizeof(char));/*for Operation type 2 set the command acoording to index+ operation1 length*/
	if(!temp) /*for malloc allocation error exit program*/
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
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
	newitem->label=first_label;
	newitem->mark='a';
	newitem->location=CODE;
	newitem->next = NULL;
	AddNodeToAssemblyTable(newitem);

	if(first_label)
		AddNodeToSymTable(first_label,IC-1,CODE,LOCAL);

	/*Handle additional addressing methods for both arguments*/
	if(operationType==1)
		if(typeAddr1!=DIRECT_REG_ADDR)
		{
			handle_rest_of_labels(typeAddr1,exteranlLbl,internalLbl1);
		}
	if(typeAddr2!=IMMIDATE_ADDR && typeAddr2!=DIRECT_REG_ADDR)
		handle_rest_of_labels(typeAddr2,exteranlLbl2,internalLbl21);
}

void handle_rest_of_labels(addr_methods type,char * exteranlLbl,char *internalLbl)
{
	int i=1,j=0;/*Indicates how many node we have to add to the Main Assembly table*/
	opcode_node *newitem;

	if(internalLbl!=NULL)
		i++;
	for(;j<i;j++)
	{
		newitem=(opcode_node *)malloc(sizeof(opcode_node));
		if(newitem==NULL)/*Check if memory error*/
		{
			printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
			exit(1);
		}
		newitem->label=NULL;
		newitem->addr=IC++;
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
		AddNodeToAssemblyTable(newitem);
	}
}

void update_tbl_addr(opcode_list * opcode_table, symbol_list * symbol_table) /*update_tbl_addr*/
{
	symbol_node * temp=symbol_table->head;
	opcode_node *tempAssm=opcode_table->head;
	while(temp)
	{
		if(temp->location!=EXTERN)
		{
			if(temp->type == DATA)
			{
				temp->dec_value +=IC;
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
			tempAssm->decAddr  +=IC;
			tempAssm->doeznAddr=int2other(tempAssm->decAddr ,12,8);
		}
		tempAssm=tempAssm->next;
	}

}
