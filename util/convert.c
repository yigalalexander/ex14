/*
 * convert.c
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */


#include "opcode.h"
#include "symbol.h"
#include <string.h>
#include <stdlib.h>
#include <sdtio.h>
#include <string.h>

void ConvertToUpper(char* str)
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


char* int2base4(int num)
{
	int rem, i=0,n=0;
	char *temp=NULL;
	while(num)
	{
		temp=(char *)realloc(temp,(i+1)*sizeof(char));
		if((rem=num%4)>9)
			temp[i++]='A'+rem-10;
		else
			temp[i++]=rem+'0';
		num=num/4;
	}

	for(n=0;n<i/2;n++)/*Reverse binary char*/
	{
		help=temp[n];
		temp[n]=temp[i-n-1];
		temp[i-n-1]=help;
	}
	if(base!=12)
	{
		temp=(char *)realloc(temp,((i+1)*sizeof(char)));
		temp[i]='\0';
	}
	return temp;
}

