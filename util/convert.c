/*
 * convert.c
 *
 *  Created on: Mar 2, 2013
 *      Author: Yigal Alexander & Alex Maximov
 */


#include "opcode.h"
#include "symbol.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*characters are converted to upper case*/
void string2upper(char* str)

{
	int i=0;
	while(str[i]!='\0')
	{
		str[i]=str[i]-('a'-'A');
		i++;
	}
}

int bin2int(char *binArr)
{
	int i,numSquare;
	int sum=0,temp,len;
	len=strlen(binArr)-1;
	for(i=len;i>=0;i--)
	{
		temp=1;
		numSquare=len-i;
		while(numSquare)
		{
			temp*=2;
			numSquare--;
		}
		sum +=(binArr[i]-'0')*temp;
	}
	return sum;
}


char * int2other(int num,int base)
{
	int rem, i=0,n=0;
	char *temp=NULL;
	char temp_char;
	while(num)
	{
		temp=(char *)realloc(temp,(i+1)*sizeof(char)); /*increase size of string by one char*/
		if((rem=num%base)>9)
			temp[i++]='A'+rem-10;
		else
			temp[i++]=rem+'0';
		num=num/base;
	}

	for(n=0;n<i/2;n++)/*Reverse binary char*/
	{
		temp_char=temp[n];
		temp[n]=temp[i-n-1];
		temp[i-n-1]=temp_char;
	}
	temp=(char *)realloc(temp,((i+1)*sizeof(char))); /* Add a terminating char to the string */
	temp[i]='\0';
	return temp;
}

int bits2int (opcode_bits bit)
{
	int temp;
	int currbit=0; /*bits position for shifting*/

	temp=temp & (unsigned int)bit.comb; currbit+=2; /*2 bits starting at bit 0 */
	temp=temp & ((unsigned int)bit.dst_reg >> currbit); currbit+=3; /*3 bits*/
	temp=temp & ((unsigned int)bit.dst_add >> currbit); currbit+=2; /*2 bits*/
	temp=temp & ((unsigned int)bit.src_reg >> currbit); currbit+=3; /*3 bits*/
	temp=temp & ((unsigned int)bit.src_add >> currbit); currbit+=2; /*2 bits*/
	temp=temp & ((unsigned int)bit.opcode >> currbit); currbit+=4; /*4 bits*/
	temp=temp & ((unsigned int)bit.type >> currbit); currbit+=1; /*3 bits*/
	temp=temp & ((unsigned int)bit.reserve >> currbit);

	return(temp);
}


