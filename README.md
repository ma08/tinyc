#Compiler for tinyc - 32 bit ( A reduced adaption of the C language )
Done as a part of the course work for the Compilers Lab course in 5th sem.
Implemented as a series of 4 assignments. The assignment(specs) will be 
added soon. The revision history is available from some point around the 
4th assignment.

The compiler uses Flex for parsing and Bison for grammar and semantic actions.
Make sure you use the latest version of yacc(bison (GNU Bison) 3.0.2 ) to make
it work - which I had found the hard way.

Check out the Makefile to get a hang of the process.

The input file is to be named as ass4_12CS30035_test.c to be compiled

##Stage 1: Lexical Analysis (generating tokens)
Done using flex. The rules are defined in the .l file.

##Stage2: Parsing (grammar) using Bison
The grammar and the corresponding actions are defined in .y file
The symboltable and the quad classes are defined in the translator.h file

##Stage 3: 3 Address Code Generation. Stage 2 and 3 are simultaneous
The 3AC is generated for each operation. These are stored as quads in the 
quadarray. The quads are written to the `quadoutput.out` file

##Stage 4: Assembly Code Generation (using GAS - GNU Assembler ) - Intel stntax
Assembly code is generated for each quad. The generating function is in the 
target_translator.cpp file. The assembly is written to the tcgoutput.s file

##Stage 5: Compiling the Assembly code into executable file
The Assembly code is compiled by the gcc along with a library(for reading 
and writing) into the executable a.out file which is to be run to execute 
the program







A more descriptive README will be put up soon when I find the energy. 





