#include <stdio.h>
#include <stdlib.h>

#define MaxLenStr 30


//Возвращает количество считаных pid
int check_pass(){
	char buf[MaxLenStr];
	printf("%s\n", "Введите пароль");
	gets(buf);
	return strcmp(buf, "pass");
}

int main(int argc, char **argv){
	if (check_pass()!=0){
		printf("%s\n", "Доступ запрещен");	
		exit(0);
	}
	printf("%s\n", "Доступ получен");
	return 0;
}