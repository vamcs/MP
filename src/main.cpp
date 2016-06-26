#include <stdio.h>
#include <assert.h>
#include "../include/Digraph.h"
#include "../include/manager.h"
#include "../include/GUI.h"

int main(){
	/*Inicialização do digrafo.*/
	Digraph G = DIGRAPHinit();
	
	/*Leitura do arquivo de entrada.*/
	DIGRAPHread(G);

	GUI(G);
	
	/*Salva o arquivo de saída no mesmo formato do arquivo de entrada.*/
	printf("Salvando o grafo em output.txt...\n");
	DIGRAPHsave(G);

	/*Desaloca memória para o digrafo.*/
	printf("Programa finalizado.\n");
	DIGRAPHdestroy(G);
}