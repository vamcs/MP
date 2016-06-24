#include <stdio.h>
#include "../include/Digraph.h"
#include "../include/manager.h"
#include "../include/fileReader.h"
#include "../include/GUI.h"

int main(){

	bool (*nameCheck)(Digraph, char*, int);
	nameCheck = NAMEcheck;

	bool (*inputCheck)(int);
	inputCheck = INPUTcheck;
	FILE* fp = fopen("./supporting_files/input.txt","r");

	Digraph G = DIGRAPHinit();
	
	while (!feof(fp)) {
		char* vertexSTR = readFileLine(fp);
		VertexArray V = cnvInputStrToVertex(vertexSTR);
		printf("%d",DIGRAPHinsertV(G, V, inputCheck, nameCheck));
	
	}

	GUI(G);

	//DIGRAPHshow(G);

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

	fclose(fp);

	execution(G, -1);
	
	printf("Salvando o grafo em output.txt...\n");
	DIGRAPHsave(G);

	printf("Programa finalizado.\n");
	DIGRAPHdestroy(G);
}