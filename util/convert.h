/*
 * convert.h
 *  Contains various conversion functions
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#ifndef CONVERT_H_
#define CONVERT_H_

char * int2other(int num,int base); /*converts an int to other base number as a string */
int bits2int (opcode_bits bit); /*converts the bits field from an opcode to an int*/
int bin2int(char *binArr); /*converts a binary string to integer*/
opcode_bits int2bits (int num); /*converts an int to the opcode bits*/
void string2upper(char* str); /*Convert a string to all upper characters*/

#endif /* CONVERT_H_ */
