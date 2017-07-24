#include <stdio.h>
#include <stdlib.h>
#include "math1.h"
#include "math2.h"
#include "math3.h"

int main(int argc, char const **argv[])
{

	int A=9;
	int B=4;
	printf("A = %d; B = %d;\n", A, B);
	printf("A + B = %d;\n", summAB( &A, &B));
	printf("A - B = %d;\n", subtAB( &A, &B));
	printf("A * B = %d;\n", multAB( &A, &B));
	printf("A / B = %d;\n", divAB( &A, &B));
	printf("A ^ B = %d;\n", pawAB( &A, &B));
	printf("A ^ 1/2 = %d;\n", sqrtA( &A));
	
	return 0;
}