#include <stdio.h>
#include <stdlib.h>
#define MaxStr 100
#define MaxCharStr 1024

void countnumb(char **str[], int *amount, int N);
int mysortbubble(int *amount, char **str[], int N); //A-ключ сортировки, B-упорядоченный массив от 0 до N-1


int main( int argc, char **argv[]){
	char **str[MaxStr];
	//Ввод строк asd
	printf("%s\n", "Вводите строки, ввод прекратится когда будет введена пустая строка");
	int N = 0;
	str[N] = (char *) calloc(MaxCharStr, sizeof(char));
	gets(str[N]);
	while (strlen(str[N])>0){
		N++;
		str[N] = (char *) calloc(MaxCharStr, sizeof(char));
		gets(str[N]);
	}
	free(str[N]);
	int *amount = (int *) calloc(N, sizeof(int));
	
	//Считаем количество цифр в каждой строке
	countnumb( str, amount, N);
	//Выводим строки сортировки, а через пробел количество цифр в них
	printf("%s\n", "До сортировки:");
	for (int i = 0; i < N; ++i)
	{
		printf("%s %d\n", str[i], amount[i]);
	}

	
	printf("%s = %d\n", "Количество перестановок алгоритма пузырик", mysortbubble( amount, str, N));
	//Выводим после сортировки, а через пробел количество цифр в них
	printf("%s\n", "После сортировки:");
	for (int i = 0; i < N; ++i)
	{
		printf("%s %d\n", str[i], amount[i]);
	}
	
	for (int i = 0; i < N; ++i)
	{
		free(str[i]);	
	}
	free(amount);
	return 0;
}

void countnumb(char **str[], int *amount, int N){   //Вопрос: почему, когда вместо int *amount было int *amount[] то получалась всякая фигня
	int j;
	char *p;
	for (int i = 0; i < N; ++i){
		p = str[i];
		j=0;
		amount[i]=0;
		while ( p[j] != '\0'){
			if ( p[j] >= '0' && p[j] <= '9')
				amount[i]++;
			j++;
		}
	}
}

int mysortbubble(int *amount, char **str[], int N){
	int tmp;
	char *p;
	int count = 0;
	int f;
	for (int i = 0; i < N-1; ++i)
	{
		f=0;
		for (int j = 0; j < N-1-i; ++j)
		{
			if ( amount[j]>amount[j+1] ){
				tmp=amount[j];
				amount[j]=amount[j+1];
				amount[j+1]=tmp;
				p=str[j];
				str[j]=str[j+1];
				str[j+1]=p;
				count++;
				f=1;
			}
		}
		if ( !f ) break;
	}
	return count;
}