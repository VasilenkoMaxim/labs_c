run: main
	./main
math1.o: math1.c
	gcc -Wall -std=c99 -c math1.c
math2.o: math2.c
	gcc -Wall -std=c99 -c math2.c
math3.o: math3.c
	gcc -Wall -std=c99 -c math3.c
libmath.a: math1.o math2.o math3.o
	ar rc libmath.a math1.o math2.o math3.o
	ranlib libmath.a
main: main.c libmath.a
	gcc -Wall -std=c99 -c main.c
	gcc -Wall -std=c99 -o main main.o -L/home/maxim/labs_c/lab5/task1/ -lmath -lm 
clear:
	rm main main.o math1.o math2.o math3.o libmath.a
