/*
 * csv_gen.c : Create a sample csv file.
 */ 


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
	int a , b , c;
	char str[20];
	memset(str , '\0' , sizeof(str));
	
	FILE *fs;
	fs = fopen("test.csv" , "r+");
	fscanf(fs , "%d , %d , %d , %s" , &a , &b , &c , str);
	printf("a = %d , b = %d , c = %d , str = %s\n" , a , b , c , str);
	fclose(fs);
	return 0;
}
