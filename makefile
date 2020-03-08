all: gccc.o

run: gccc.o
	./obj/gccc.o in.txt

gccc.o: obj/lexParser.o obj/syntaxParser.o
	gcc  -Wall -Wextra -g src/main.c obj/lexParser.o obj/syntaxParser.o -o obj/gccc.o

obj/lexParser.o: src/lexParser.c
	gcc  -Wall -Wextra -g -c src/lexParser.c -o obj/lexParser.o

obj/syntaxParser.o: src/syntaxParser.c
	gcc  -Wall -Wextra -g -c src/syntaxParser.c -o obj/syntaxParser.o

clean:
	rm obj/*.o