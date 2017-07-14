run: main
	./main < test.txt
main: main.c
	gcc -Wall -std=c99 -c main.c
	gcc -Wall -std=c99 main.o -o main
clear:
	rm main main.o