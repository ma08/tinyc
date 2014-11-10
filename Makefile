a.out: tcgoutput.o libass2_12CS30035.a
	g++ -m32 tcgoutput.o -L. -lass2_12CS30035 
libass2_12CS30035.a:	ass2_12CS30035.o
		ar -rcs libass2_12CS30035.a ass2_12CS30035.o
ass2_12CS30035.o:	ass2_12CS30035.c my1.h
	cc -Wall -m32 -c ass2_12CS30035.c
tcgoutput.o: tcgoutput.s
	g++ -m32 -c tcgoutput.s
quadoutput.out: outputfile
	 ./outputfile	< ass4_12CS30035_test.c
tcgoutput.s: outputfile
	./outputfile < ass4_12CS30035_test.c
outputfile: lex.yy.o y.tab.o ass4_12CS30035_target_translator.o ass4_12CS30035_translator.o
	g++  -o outputfile lex.yy.o y.tab.o ass4_12CS30035_target_translator.o ass4_12CS30035_translator.o -lfl
ass4_12CS30035_target_translator.o: ass4_12CS30035_target_translator.c
	g++ -c ass4_12CS30035_target_translator.c
ass4_12CS30035_translator.o: ass4_12CS30035_translator.cxx ass4_12CS30035_translator.h
	g++ -c ass4_12CS30035_translator.h
	g++ -c ass4_12CS30035_translator.cxx
lex.yy.o: y.tab.o lex.yy.c
	g++ -c lex.yy.c
y.tab.o: y.tab.c
	g++ -c y.tab.c
lex.yy.c: ass4_12CS30035.l ass4_12CS30035_translator.h
	flex ass4_12CS30035.l
y.tab.c: ass4_12CS30035.y
	yacc -dtv ass4_12CS30035.y
y.tab.h: ass4_12CS30035.y
	yacc -dtv ass4_12CS30035.y

clean:
	rm ass4_12CS30035_target_translator.o ass4_12CS30035_translator.o y.output outputfile ass4_12CS30035_translator.h.gch lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h tcgoutput.o tcgoutput.s quadoutput.out ass2_12CS30035.o a.out
