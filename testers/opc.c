#include <stdio.h>
#include <stdlib.h>
#include "opcode.h"


int main()
{
	opcode_list * list1;

	list1=new_opcode_list(100);
	if (flush_list(list1))
		printf("\nFlush was OK\n");
	else
		printf("\nFlush was NOT OK\n");
	return (0);
}
