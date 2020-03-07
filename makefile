all: gccc.o

gccc.o: obj/lexParser.o
	gcc  -Wall -Wextra -g src/main.c obj/lexParser.o -o obj/gccc.o

obj/lexParser.o: src/lexParser.c
	gcc  -Wall -Wextra -g -c src/lexParser.c -o obj/lexParser.o

clean:
	rm obj/*.o