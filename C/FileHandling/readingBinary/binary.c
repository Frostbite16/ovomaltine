#include<stdio.h>
#include<stdlib.h>

void printFile(FILE** binfile){
	char currentSymbol;
	currentSymbol = fgetc(*binfile);
	while(currentSymbol!=EOF){
		printf("%c", currentSymbol);
		currentSymbol = fgetc(*binfile);
	}
}
