/*
 * rline.c
 *
 *  Created on: Mar 2, 2013
 *      Author: alex
 */

#include<stdlib.h>
#include <stdio.h>

int main (void)
{
	char filename[] = "file.txt";
	FILE *file = fopen ( filename, "r" );
	if (file != NULL)
	{
		char line [80];

		while(fgets(line,sizeof line,file)!= NULL) /* read a line from a file */
		{
			fprintf(stdout,"%s",line); /*print the file contents on stdout.*/
		}
		fclose(file);
	}
	else
	{
		perror(filename); /*print the error message on stderr.*/
	}
	return 0;
}



