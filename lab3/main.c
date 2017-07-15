#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h>
#define MaxVoucher 100
#define MaxCharStr 1024

struct voucher{
	char 	name_route[30]; 	//название маршрута, не забывать менять длину массива в readVoucher
	double 	distance; 			//протяженность маршрута в км
	int 	halt_count; 		//количество остановок
	double 	route_cost; 		//цена путевки в рублях
};
typedef struct voucher voucher;


void countnumb(char **str[], int *amount, int N);
int mysortbubble(int *amount, char **str[], int N);

//MaxLenBuff - максимальный размер строки которую прочитает функция, но не факт что запишет
//CheckINF - индикатор включения диалога, если 0 то не будут показываться диалоги
char readVoucher( voucher *vch, int MaxLenBuff, char CheckINF);
void writeVoucher( voucher *vch);
int cmpVoucherNameRoute(const void *a, const void *b);
int cmpVoucherDistance(const void *a, const void *b);
int cmpVoucherHaltCount(const void *a, const void *b);
int cmpVoucherRouteCost(const void *a, const void *b);


char readVoucher( voucher *vch, int MaxLenBuff, char CheckINF){  //функция для ввода структуры voucher c stdin                 
	char buffer[MaxLenBuff];
	char buffer0[MaxLenBuff];
	buffer[0]='\0';
	buffer0[0]='\0'; //подумать как избавиться от второго буфера
	
	if ( CheckINF ){
		printf( "%s\n", "Введите название маршрута");
	}
		fgets( buffer, MaxLenBuff, stdin);
			sscanf( buffer, "%[^\n]", buffer0);
				snprintf( vch->name_route, 30, buffer0);
	if ( strlen(vch->name_route ) == 0 ){
		return 1;	//не было введено название маршрута
	}
	
	if ( CheckINF ){
		printf( "%s\n", "Введите протяженность маршрута в км");
	}
		fgets( buffer, MaxLenBuff, stdin);
			if ( sscanf( buffer, "%lf", &vch->distance) == -1 ){
				return 2;	//не была введена протяженность маршрута
			}
	
	if ( CheckINF ){
		printf( "%s\n", "Введите количество остановок");
	}
		fgets( buffer, MaxLenBuff, stdin);
			if ( sscanf( buffer, "%d", &vch->halt_count) == -1 ){
				return 3;	//не было введено количество остановок
			}

	if ( CheckINF ){
		printf( "%s\n", "Введите цену путевки в рублях");
	}
		fgets( buffer, MaxLenBuff, stdin);
			if ( sscanf( buffer, "%lf", &vch->route_cost) == -1 ){
				return 4;	//не была введена цена путевки
			}
	return 0;
}
void writeVoucher( voucher *vch){  //функция для вывода информации структуры voucher в stdout                 
	printf("%30s%20.2lf%20d%20.2lf\n", vch->name_route, vch->distance, vch->halt_count, vch->route_cost);
	//printf( "%s %s\n", "Название маршрута:", vch->name_route);	
	//printf("%s = %.2lf км\n", "Протяженность маршрута", vch->distance);
	//printf("%s = %d\n", "Количество остановок", vch->halt_count);
	//printf("%s = %.2lf руб.\n", "Цена путевки", vch->route_cost);
}
int cmpVoucherNameRoute(const void *a, const void *b){
	voucher *A, *B;
	A = *((voucher **)a);
	B = *((voucher **)b);
	return strcmp( A->name_route, B->name_route);
}
int cmpVoucherDistance(const void *a, const void *b){
	voucher *A, *B;
	A = *((voucher **)a);
	B = *((voucher **)b);
	double eps = 1.0e-15;
	if ( A->distance - B->distance < -eps ){
		return -1;
	}
	else if ( A->distance - B->distance > eps ){
		return 1;
	}
	else{
		return 0;
	}
}
int cmpVoucherHaltCount(const void *a, const void *b){
	voucher *A, *B;
	A = *((voucher **)a);
	B = *((voucher **)b);
	return A->halt_count - B->halt_count;
}
int cmpVoucherRouteCost(const void *a, const void *b){
	voucher *A, *B;
	A = *((voucher **)a);
	B = *((voucher **)b);
	double eps = 1.0e-15;
	if ( A->route_cost - B->route_cost < -eps ){
		return 1;
		printf("%lf\n", A->route_cost);
	}
	else if ( A->route_cost - B->route_cost > eps ){
		return -1;
		printf("%lf\n", A->route_cost);
	}
	else{
		printf("%lf\n", A->route_cost);
		return 0;
	}
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
		if ( !f ){
			break;
		} 
	}
	return count;
}

int main( int argc, char **argv[]){
	mtrace();
	printf("%s\n", "Ввод путевок, следуйте указаниям, введите путую строку, чтобы прекратить ввод");
	int N = -1; //количество введеных структур
	voucher **vch[MaxVoucher];
	do{
		N++;
		printf("%s %d\n", "Ввод путевки номер", N+1);
		vch[N] = (voucher *)malloc(sizeof(voucher));
		if ( !vch[N] )
			exit(1);
	}while ( !readVoucher( vch[N], MaxCharStr, 0) );
	free(vch[N]);

	printf("\n%s\n", "До сортировки");
	printf("%-47s%-35s%-34s%-35s\n", "Название маршрута", "Протяженность, км", "Кол-во остановок", "Цена, руб.");
	for (int i = 0; i < N; ++i)
	{
		writeVoucher(vch[i]);	
	}

	qsort( vch, N, sizeof(voucher *), cmpVoucherRouteCost);
	printf("\n%s\n", "После сортировки по цене по убыванию");
	printf("%-47s%-35s%-34s%-35s\n", "Название маршрута", "Протяженность, км", "Кол-во остановок", "Цена, руб.");
	for (int i = 0; i < N; ++i)
	{
		writeVoucher(vch[i]);	
	}

	qsort( vch, N, sizeof(voucher *), cmpVoucherNameRoute);
	printf("\n%s\n", "После сортировки по названию маршрута");
	printf("%-47s%-35s%-34s%-35s\n", "Название маршрута", "Протяженность, км", "Кол-во остановок", "Цена, руб.");
	for (int i = 0; i < N; ++i)
	{
		writeVoucher(vch[i]);	
	}

	qsort( vch, N, sizeof(voucher *), cmpVoucherDistance);
	printf("\n%s\n", "После сортировки по протяженности по возрастанию");
	printf("%-47s%-35s%-34s%-35s\n", "Название маршрута", "Протяженность, км", "Кол-во остановок", "Цена, руб.");
	for (int i = 0; i < N; ++i)
	{
		writeVoucher(vch[i]);	
	}

	qsort( vch, N, sizeof(voucher *), cmpVoucherHaltCount);
	printf("%s\n", "После сортировки по количеству остановок по возрастанию");
	printf("%-47s%-35s%-34s%-35s\n", "Название маршрута", "Протяженность, км", "Кол-во остановок", "Цена, руб.");
	for (int i = 0; i < N; ++i)
	{
		writeVoucher(vch[i]);	
	}

	for (int i = 0; i < N; ++i)
	{
		free(vch[i]);	
	}

	return 0;
}