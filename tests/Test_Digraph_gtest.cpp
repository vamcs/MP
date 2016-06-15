#include <limits.h>
#include "../include/Digraph.h"
#include "gtest/gtest.h"

//DIGRAPHshow(G);

class MyAppTestSuite : public testing::Test {
	void SetUp(){}
	void TearDown(){}
};


TEST(DigraphTest, Initialization){
	Digraph G = DIGRAPHinit("input.txt");
	EXPECT_EQ(0, VERTEXreturn(G, 100));		//Testa se "A" é o vértice 0
	EXPECT_EQ(1, VERTEXreturn(G, 101));		//Testa se "B" é o vértice 1
	EXPECT_EQ(2, VERTEXreturn(G, 102));		//Testa se "B" é o vértice 2
	EXPECT_EQ(3, VERTEXreturn(G, 103));		//Testa se "B" é o vértice 3
	EXPECT_EQ(4, VERTEXreturn(G, 104));		//Testa se "B" é o vértice 4
	EXPECT_EQ(5, VERTEXreturn(G, 105));		//Testa se "B" é o vértice 5
	EXPECT_EQ(6, VERTEXreturn(G, 106));		//Testa se "B" é o vértice 6

	//EXPECT_TRUE(G->array[0]->source == true);
}

/*int main(){
	int v, w, aux;
	Edge e;
	
	Digraph G = DIGRAPHinit("Digraph.txt");
	DIGRAPHshow(G);
	
	EXPECT_EQ(VERTEXreturn(G, "H"), -1);
	
	EXPECT_EQ(VERTEXreturn(G, "B"), 1);
	
	v = VERTEXreturn(G, "B");
	w = VERTEXreturn(G, "F");
	e = EDGE(v, w, 2.0);
	DIGRAPHinsertE(G, e);
	aux = G->array[v]->adj->w;
	EXPECT_EQ(G->array[w]->name, G->array[aux]->name);
	
	//Remover a aresta inserida. Espera-se o teste não passar.
	DIGRAPHremoveE(G, e);
	EXPECT_EQ(G->array[v]->name, G->array[aux]->name);
	
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
	EXPECT_EQ(FindPath(G, "B", "F"), -1);
	
	//Caminho válido.
	EXPECT_EQ(FindPath(G, "B", "E"), 2.0);
	
	printf("\n\n");
	DIGRAPHshow(G);
	printf("\n\n");
	DIGRAPHshow(H);
	
	DIGRAPHdestroy(H);
	DIGRAPHdestroy(G);
}*/