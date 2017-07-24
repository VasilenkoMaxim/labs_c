#include <stdio.h>
#include <stdlib.h>
#include "math1.h"
#include "math2.h"
#include "math3.h"
#include <dlfcn.h>

typedef int (*my_func1)( int*, int* ); // ЧТО ЭТО ЗНАЧИТ?!
typedef int (*my_func2)( int* ); // ЧТО ЭТО ЗНАЧИТ?! Описание прототипа?


int main(int argc, char const **argv[])
{
	void *dl_handle = dlopen( "./libmath.so", RTLD_LAZY);
	if (!dl_handle){
		printf( "ERROR: %s\n", dlerror());
		return 3;
	}
	int A=9;
	int B=4;
	
	my_func1 f_summAB = dlsym( dl_handle, "summAB");
	my_func1 f_subtAB = dlsym( dl_handle, "subtAB");
	my_func1 f_multAB = dlsym( dl_handle, "multAB");
	my_func1 f_divAB = dlsym( dl_handle, "divAB");
	my_func1 f_pawAB = dlsym( dl_handle, "pawAB");
	my_func2 f_sqrtA = dlsym( dl_handle, "sqrtA");
	char *error = dlerror();
	if ( error != NULL){
		printf("ERROR: %s\n", error);
		return 4;
	}
	printf("A = %d; B = %d;\n", A, B);
	printf("A + B = %d;\n", (*f_summAB)( &A, &B));
	printf("A - B = %d;\n", (*f_subtAB)( &A, &B));
	printf("A * B = %d;\n", (*f_multAB)( &A, &B));
	printf("A / B = %d;\n", (*f_divAB)( &A, &B));
	printf("A ^ B = %d;\n", (*f_pawAB)( &A, &B));
	printf("A ^ 1/2 = %d;\n", (*f_sqrtA)( &A));
	dlclose( dl_handle );
	return 0;
}