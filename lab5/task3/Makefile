run: main
	./main
math1.o: math1.c
	gcc -Wall -std=c99 -c -fpic -fPIC -shared math1.c -o math1.o
math2.o: math2.c
	gcc -Wall -std=c99 -c -fpic -fPIC -shared math2.c -o math2.o
math3.o: math3.c
	gcc -Wall -std=c99 -c -fpic -fPIC -shared math3.c -o math3.o
libmath.so: math1.o math2.o math3.o
	gcc -Wall -std=c99 -shared -o libmath.so math1.o math2.o math3.o -lm
main: main.c libmath.so
	gcc -Wall -std=c99 -c main.c
	gcc -Wall -std=c99 -o main main.o -ldl
clear:
	rm main main.o math1.o math2.o math3.o libmath.so
