
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode.h"
#include "symbol.h"
#include "convert.h"
#include "lang.h"
#include "parse.h"
#include "asoutput.h"


int main(int argc, const char * argv[])
{
	int i;
	char * fname=NULL;
	FILE * INPUT_PROGRAM;
	parsing_globals session;


	if (argc<2)		/*Worng number of parameters - file not received */
	{
		printf(ERR_FILE_MISSING_OPERAND);
		exit(1);
	}
	fname=(char *)malloc((strlen(argv[1])+3)*sizeof(char));
	if(fname==NULL)
	{
		printf(ERR_MEMORY_ALLOCATION_FAILURE,0);
		exit(1);
	}


	for (i=1; i<argc; ++i ) /* iterate over all input files */
	{

		init_globals(session);
		session.symbol_table=new_symbol_list(); /*new table for symbols*/
		session.code_table=new_opcode_list(100); /*new table for code*/
		strcpy(fname,argv[i]);
		strcat(fname,".as");

		INPUT_PROGRAM=fopen(fname,"r");
		if(INPUT_PROGRAM==NULL)
		{
			printf(ERR_FAILED_TO_OPEN_FILE);
			exit(1);
		}

		first_pass(INPUT_PROGRAM,session); /*read the file into the table, read symbols*/

		if(!session.errors_found) /*output only if no errors were found*/
		{
			update_tbl_addr(session);

			second_pass(session);
			if(!(session.errors_found))
			{
				write_object_file((argv[i]),session.code_table);
				write_extern_file(argv[i],session.symbol_table);
				write_entry_file(argv[i],session.symbol_table);
			}
		}
		/*clear the tables used for the parsing and free the memory, for another round*/
		flush_symbols(session.symbol_table);
		flush_opcode_list(session.code_table);
	}
	exit(1);

}



