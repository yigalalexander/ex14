/*
 * parse.h
 *
 *  Created on: Mar 24, 2013
 *      Author: yigalalexander
 */

#ifndef PARSE_H_
#define PARSE_H_

typedef struct ad_meth {
	int op_num; /*number of operands*/
	type;
} addr_method;

char * get_first_word (char *);
addr_method get_addressing_method(char *);
void first_pass(); /*add parameters here!*/
void second_pass(); /*add parameters here!*/

#endif /* PARSE_H_ */
