echoserv: echoserv.o helper.o
	gcc -o echoserv echoserv.o helper.o -Wall -Werror -Wimplicit-function-declaration

echoserv.o: echoserv.c helper.h
	gcc -o echoserv.o echoserv.c -c -ansi -pedantic -Wall -Werror -Wimplicit-function-declaration

helper.o: helper.c helper.h
	gcc -o helper.o helper.c -c -ansi -pedantic -Wall -Werror -Wimplicit-function-declaration

clean:
	rm *.o echoserv









