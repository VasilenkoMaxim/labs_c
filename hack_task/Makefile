run: main
	./main
main: main.c
	gcc -Wall -std=c99 -o main -g main.c -mpreferred-stack-boundary=4 -fno-stack-protector -z execstack
clear:
	rm main main.o
