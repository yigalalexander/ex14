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

#endif /* CONVERT_H_ */
