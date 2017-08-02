run: main
	./main
main: main.c
	gcc -mpreferred-stack-boundary=4 -fno-stack-protector -z execstack -Wall -std=c99 -o main -g main.c
clear:
	rm main main.o
