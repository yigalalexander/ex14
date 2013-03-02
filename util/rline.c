#include<stdlib.h>
#include <stdio.h>

int main (void)
{
	char filename[] = "file.txt";
	FILE *file = fopen ( filename, "r" );
	if (file != NULL)
	{
		char line [80];
		int line_index = 0;

		while(fgets(line,sizeof line,file)!= NULL) /* read a line from a file */
		{
			fprintf(stdout,"%d",line_index);
			fprintf(stdout,"%s",line); /*print the file contents on stdout.*/
			line_index = line_index+1;
		}
		fclose(file);
	}
	else
	{
		perror(filename); /*print the error message on stderr.*/
	}
	return 0;
}
