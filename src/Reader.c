#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Digraph.h"

void reader(FILE* fp){
	
	//Leitura do arquivo de instruções e geração do grafo
	fscanf(fp, );
	fclose(fp);
}

FILE* openFile(char* name, char* type){
	FILE* fp = fopen(name, type);
	if(fp == NULL){
		printf("Arquivo inexistente. Entre com outro nome:\n");
		scanf("%s", name);
	}
}