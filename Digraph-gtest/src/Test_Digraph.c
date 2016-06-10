#include <stdio.h>
#include <stdlib.h>
#include "Digraph.h"

int main(){
	Vertex v, w;
	
	Digraph G = DIGRAPHinit("Digraph.txt");
	
	DIGRAPHshow(G);
	
	printf("\nDigrafo inicializado.\n\n");
	
	printf("Primeiro teste: busca por vertice inexistente.\n");
	
	v = VERTEXreturn(G, "H");
	w = VERTEXreturn(G, "I");
	
	printf("Vertice H, v: %d.\nVertice I, w: %d.\n", v, w);
	
	printf("\nSegundo teste: insercao de aresta.\n");
	printf("B-(1.0)->A. Ira falhar, ja que A eh origem.\n");
	
	v = VERTEXreturn(G, "B");
	w = VERTEXreturn(G, "A");
	Edge e = EDGE(v, w, 1.0);
	DIGRAPHinsertE(G, e);
	
	printf("Agora, fazer B-(2.0)->F ira funcionar.\n");
		
	w = VERTEXreturn(G, "F");
	e = EDGE(v, w, 2.0);
	DIGRAPHinsertE(G, e);
	DIGRAPHshow(G);
	
	printf("\nTerceiro teste: remocao de aresta.\n");
	printf("C->G nao existe, entao ira falhar.\n");
	
	v = VERTEXreturn(G, "C");
	w = VERTEXreturn(G, "G");
	e = EDGE(v, w, 10.0);
	DIGRAPHremoveE(G, e);
	
	printf("Agora, removendo A-(5.0)->G...\n");
	
	v = VERTEXreturn(G, "A");
	w = VERTEXreturn(G, "G");
	e = EDGE(v, w, 5.0);
	DIGRAPHremoveE(G, e);
	DIGRAPHshow(G);
	
	printf("\nQuarto teste: insercao de vertice.\n");
	printf("Tentar inserir um vertice falhara, como tentar inserir A.\n");
	
	DIGRAPHinsertV(G, "A");
	DIGRAPHshow(G);
	
	printf("\nMas H por exemplo funcionara.\n");
	
	DIGRAPHinsertV(G, "H");
	DIGRAPHshow(G);
	
	printf("\nQuinto teste: remocao de vertice.\n");
	printf("Remover um vertice inexistente falha, como tentar remover I.");
	
	
	DIGRAPHremoveV(G, "I");
	DIGRAPHshow(G);
	
	printf("Mas se removermos A...\n");
	DIGRAPHremoveV(G, "A");
	
	printf("E tambem C...\n");
	DIGRAPHremoveV(G, "C");
	
	printf("E por fim H que acabamos de inserir.\n");
	DIGRAPHremoveV(G, "H");
	
	DIGRAPHshow(G);
	
	printf("\nSexto teste: o nosso digrafo eh conexo?\n");
	printf("A funcao isConnected nos dira.\n");
	
	bool connected = isConnected(G);
	
	printf("Digrafo G conexo? R: %d\n", connected);
	
	printf("Mas se gerarmos um novo digrafo H contido em Digraph2.txt...\n");
	
	Digraph H = DIGRAPHinit("Digraph2.txt");
	
	DIGRAPHshow(H);
	
	printf("Podemos verificar se eh conexo ou nao.\n");
	printf("(Eh conexo! A partir de A chegamos em qualquer vertice.)\n");
	
	connected = isConnected(H);
	
	printf("Digrafo H conexo? R: %d\n", connected);
	
	printf("\nSetimo teste: existe caminho?\n");
	printf("Utilizando o algorito de Dijkstra, verificamos a existencia e o menor caminho entre dois vertices.\n");
	printf("De B ate G existe?\n");
	
	float path = FindPath(G, "B", "G");
	if(path == -1) printf("Nao existe o caminho B->G. Path = %.0f\n", path);
	else printf("Existe caminho! Distancia minima entre B->G: %.1f\n", path);
	
	printf("E entre B e E? Ha dois caminhos!\n");
	
	path = FindPath(G, "B", "E");
	if(path == -1) printf("Nao existe o caminho B->E. Path = %.0f\n", path);
	else printf("Existe caminho! Distancia minima entre B->E: %.1f\n", path);
	
	printf("\nImpressao dos resultados em output.txt...\n");
	DIGRAPHsave(G);
	
	printf("\nFim dos Testes. Digrafos destruidos.\n");
	
	DIGRAPHdestroy(G);
	DIGRAPHdestroy(H);
return 0;
}
