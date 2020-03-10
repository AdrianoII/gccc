all: gccc.o

run: gccc.o
	./obj/gccc.o in.txt

run-pretty: gccc.o
	./obj/gccc.o in.txt -st

gccc.o: obj/lexParser.o obj/syntaxParser.o obj/semanticParser.o obj/intermediateCodeGeneration.o
	gcc  -Wall -Wextra -g src/main.c obj/lexParser.o obj/syntaxParser.o obj/semanticParser.o obj/intermediateCodeGeneration.o -o obj/gccc.o

obj/lexParser.o: src/lexParser.c
	gcc  -Wall -Wextra -g -c src/lexParser.c -o obj/lexParser.o

obj/syntaxParser.o: src/syntaxParser.c
	gcc  -Wall -Wextra -g -c src/syntaxParser.c -o obj/syntaxParser.o

obj/semanticParser.o: src/semanticParser.c
	gcc  -Wall -Wextra -g -c src/semanticParser.c -o obj/semanticParser.o

obj/intermediateCodeGeneration.o: src/intermediateCodeGeneration.c
	gcc  -Wall -Wextra -g -c src/intermediateCodeGeneration.c -o obj/intermediateCodeGeneration.o

clean:
	rm obj/*.o