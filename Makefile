a.out: lex.yy.o y.tab.o ass4_12CS30035_main.o ass4_12CS30035_translator.o
	g++ lex.yy.o y.tab.o ass4_12CS30035_main.o ass4_12CS30035_translator.o -lfl
ass4_12CS30035_main.o: ass4_12CS30035_main.c
	g++ -c ass4_12CS30035_main.c
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
	yacc -dtv -Wno-yacc ass4_12CS30035.y
y.tab.h: ass4_12CS30035.y
	yacc -dtv -Wno-yacc ass4_12CS30035.y

clean:
	rm ass4_12CS30035_main.o ass4_12CS30035_translator.o y.output a.out ass4_12CS30035_translator.h.gch lex.yy.o y.tab.o lex.yy.c y.tab.c y.tab.h
