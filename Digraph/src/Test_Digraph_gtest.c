#include <stdio.h>
#include <stdlib.h>
#include "Digraph.h"
#include "gtest/gtest.h"

int main(){
	Digraph G = DIGRAPHinit("Digraph.txt");
	DIGRAPHshow(G);
	
	EXPECT_EQ(VERTEXreturn(G, "H"), -1);
	
	EXPECT_EQ(VERTEXreturn(G, "B"), 1);
	
	v = VERTEXreturn(G, "B");
	w = VERTEXreturn(G, "F");
	e = EDGE(v, w, 2.0);
	DIGRAPHinsertE(G, e);
	EXPECT_EQ(G->array[w]->name, G->array[v]->adj->name);
	
	//Remover a aresta inserida. Espera-se o teste não passar.
	DIGRAPHremoveE(G, e);
	EXPECT_EQ(G->array[v]->name, G->array[v]->adj->name);
	
	DIGRAPHinsertV(G, "H");
	EXPECT_EQ(G->array[G->V-1]->name, "H");
	
	//Remover o vértice A (array[0]). Espera-se o teste não passar.
	DIGRAPHremoveV(G, "A");
	EXPECT_EQ(G->array[0]->name, "A");
	
	//Este digrafo não é conexo.
	EXPECT_EQ(isConnected(G), 0);
	
	//Novo digrafo conexo H
	Digraph H = DIGRAPHinit("Digraph2.txt");
	EXPECT_EQ(isConnected(H), 1);
	
	//Caminho inválido.
	v = VERTEXreturn(G, "B");
	w = VERTEXreturn(G, "F");
	EXPECT_EQ(FindPath(G, v, w), -1);
	
	//Caminho válido.
	w = VERTEXreturn(G, "E");
	EXPECT_EQ(FindPath(G, v, w), 2.0);
	
	printf("\n\n");
	DIGRAPHshow(G);
	printf("\n\n");
	DIGRAPHshow(H);
	
	DIGRAPHdestroy(H);
	DIGRAPHdestroy(G);
}