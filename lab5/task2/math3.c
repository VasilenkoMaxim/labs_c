#include <math.h>
int pawAB(int *A, int *B){
	int result=*A;
	for (int i = 1; i < *B; ++i)
	{
		result*=*B;
	}
	return (int )(result);
}
int sqrtA(int *A){
	return (int )(sqrt( *A));
}