#include <stdio.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

int main(){

	bool (*nameCheck)(Digraph, char*, int);
	nameCheck = NAMEcheck;

	bool (*inputCheck)(int);
	inputCheck = INPUTcheck;

	Digraph G = DIGRAPHinit("./supporting_files/input.txt", inputCheck, nameCheck);

	DIGRAPHshow(G);

	// printf("Inserindo uma tarefa nova:\n");
	// DIGRAPHinsertV(G, inputCheck, nameCheck);

	// DIGRAPHshow(G);

	// int id;
	// printf("Qual tarefa deve ser removida? Entre com a ID: ");
	// scanf("%d", &id);
	// DIGRAPHremoveV(G, id);

	// DIGRAPHshow(G);

	// modify(G);

	// DIGRAPHshow(G);

	execution(G, -1);
	
	printf("Salvando o grafo em output.txt...\n");
	DIGRAPHsave(G);

	printf("Programa finalizado.\n");
	DIGRAPHdestroy(G);
}