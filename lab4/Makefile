run: main main1
	./main test.txt R
	./main1 test.txt R
	cat test.new1
	cat test.new2
masstr.o: masstr.c
	gcc -Wall -std=c99 -c masstr.c
main: main.c masstr.o
	gcc -Wall -std=c99 -c main.c
	gcc -Wall -std=c99 masstr.o main.o -o main
main1: main1.c
	gcc -Wall -std=c99 -c main1.c
	gcc -Wall -std=c99  main1.o -o main1
clear:
	rm main main.o main1 main1.o masstr.o test.new1 test.new2
