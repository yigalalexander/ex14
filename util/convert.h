/*
 * convert.h
 *  Contains various conversion functions
 *
 *  Created on: Mar 2, 2013
 *      Author: yigalalexander
 */

#ifndef CONVERT_H_
#define CONVERT_H_

char * int2base4 (int num); /*converts an int to 4 base number as a string */
int bits2int (opcode_bits bit); /*converts the bits field from an opcode to an int*/
opcode_bits int2bits (int num); /*converts an int to the opcode bits*/
void opcode2row (opcode_node opc,char *target); /*converts ^opc^ opcode to a formatted row string*/

#endif /* CONVERT_H_ */
