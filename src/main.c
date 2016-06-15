#include <stdio.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

int main(){
	Digraph G = DIGRAPHinit("instructions.txt");
	DIGRAPHshow(G);

	modify(G);

	//DIGRAPHremoveV(G, 100);
	DIGRAPHshow(G);

	//DIGRAPHsave(G);
	// DIGRAPHremoveV(G, 100);
	// DIGRAPHshow(G);

	// DIGRAPHinsertV(G);
	// DIGRAPHshow(G);

	// DIGRAPHinsertE(G, EDGE(0, 4, G->array[4]->id));
	// DIGRAPHshow(G);

	// DIGRAPHremoveE(G, EDGE(0, 4, G->array[4]->id));
	// DIGRAPHshow(G);

	DIGRAPHdestroy(G);
}