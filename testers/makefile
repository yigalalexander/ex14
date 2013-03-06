opc: opc.o opcode.o
	gcc  opc.o opcode.o -o opc

opc.o: opc.c
	gcc "../util" -Wall -ansi -pedantic -g -c opc.c

opcode.o: ..\util\opcode.h ..\util\opcode.c
	gcc -B "../util" -Wall -ansi -pedantic -g -c ..\util\opcode.h ..\util\opcode.c