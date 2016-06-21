#include <stdio.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

int main(){

	Digraph G = DIGRAPHinit("./supporting_files/input.txt");

	DIGRAPHshow(G);

	// printf("Inserindo uma tarefa nova:\n");
	// DIGRAPHinsertV(G);

	// DIGRAPHshow(G);

	// int id;
	// printf("Qual tarefa deve ser removida? Entre com a ID: ");
	// scanf("%d", &id);
	// DIGRAPHremoveV(G, id);

	// DIGRAPHshow(G);

	// modify(G);

	execution(G, -1);

	// DIGRAPHshow(G);
	
	printf("Salvando o grafo em output.txt...\n");
	DIGRAPHsave(G);

	printf("Programa finalizado.\n");
	DIGRAPHdestroy(G);
}