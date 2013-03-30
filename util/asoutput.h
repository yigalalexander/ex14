/*
 * asoutput.h
 *
 *  Created on: Mar 26, 2013
 *      Author: yigalalexander
 */

#ifndef ASOUTPUT_H_
#define ASOUTPUT_H_

void write_object_file(char *fname, opcode_list * oplist);
void write_target_file(char* fname, symbol_list * list);
void write_entry_file(symbol_list * entry_list, char *fname);


#endif /* ASOUTPUT_H_ */
