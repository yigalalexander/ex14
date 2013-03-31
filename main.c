
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opcode.h"
#include "symbol.h"
#include "convert.h"
#include "lang.h"
#include "parse.h"


int main(int argc, const char * argv[])
{
	int i;
	char * fname=NULL;
	FILE * INPUT_PROGRAM;

	if (argc<2)		/*Worng number of parameters - file not received */
	{
		printf(ERR_FILE_MISSING_OPERAND);
		exit(1);
	}
	fname=(char *)malloc((strlen(argv[1])+3)*sizeof(char));
	if(fname==NULL)
	{
		printf(ERR_MEMORY_LOCATION_FAILURE,line_pos);
		exit(1);
	}
	symbol_list * symbols_table;
	opcode_list * machine_code_table;
	for (i=1; i<argc; ++i ) /* iterate over all input files */
	{


		symbols_table=new_symbol_list(); /*new table for symbols*/
		machine_code_table=new_opcode_list(); /*new table for code*/
		strcpy(fname,argv[i]);
		strcat(fname,".as");

		INPUT_PROGRAM=fopen(fname,"r");
		if(INPUT_PROGRAM==NULL)
		{
			printf(ERR_FAILED_TO_OPEN_FILE);
			exit(1);
		}

		first_pass(INPUT_PROGRAM,machine_code_table,symbols_table); /*read the file into the table, read symbols*/

		if(!errors_found) /*output only if no errors were found*/
		{
			update_tbl_addr(machine_code_table,symbols_table);

			second_pass(machine_code_table,symbols_table);
			if(!errors_found)
			{
				write_object_file(argv[i],machine_code_table);
				write_extern_file(argv[i],symbols_table);
				write_entry_file(argv[i],symbols_table);
			}
		}
		/*clear the tables used for the parsing and free the memory, for another round*/
		flush_symbols(symbols_table);
		flush_opcode_list(machine_code_table);
	}
	exit(1);

}

