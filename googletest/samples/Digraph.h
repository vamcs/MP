/*Cabeçalho de Definições do Grafo*/

/*Declaração dos tipos utilizados*/
#define Vertex int
#include <stdbool.h>
//#define bool int
//#define true 1
//#define false 0

typedef struct node * lista;
typedef struct vertexArray * VertexArray;
typedef struct digraph * Digraph;
typedef struct edge Edge;

/*Estruturas do digrafo*/

struct node{				//Nós do tipo "node" representam os nós da lista encadeada.
	Vertex w;				//Posição do vértice adjacente no array de vértices, contido neste nó da lista.
	float weight;			//Peso da aresta.
	lista next; 				//próximo nó da lista de adjacência.
};

struct vertexArray{			//Vertex v é definido pela posição no vetor.
	char name[100];			//Nome do vértice.
	bool source;			//Indica se o vértice é origem ou não.
	lista adj;				//Lista encadeada do vértice.
};

struct digraph{
	int V;					//Número de vértices.
	int E;					//Número de arestas (edges).
	VertexArray * array;	//Ponteiro para o vetor de listas de adjacência.
};

struct edge{				//Define uma aresta (edge) v -> w.
	Vertex v;
	Vertex w;
	float weight;			//Peso da aresta.
};

/*Inicialização do Digrafo.*/
Digraph DIGRAPHinit(char*);	

/*Inserção e remoção de arestas.*/
void DIGRAPHinsertE(Digraph, Edge);
void DIGRAPHremoveE(Digraph, Edge);

/*Inserção e remoção de vértices.*/
void DIGRAPHinsertV(Digraph, char*);
void DIGRAPHremoveV(Digraph, char*);

/*Verificação das adjacências de um nó.*/
bool DIGRAPHadj(Digraph, Vertex, Vertex);

/*Desalocação do grafo*/
void DIGRAPHdestroy(Digraph);

/*Impressão do grafo na tela e criação de um arquivo de saída*/
void DIGRAPHshow(Digraph);
void DIGRAPHsave(Digraph);

/*Funções e algoritmos de busca. SPT de Dijkstra é utilizada.*/
float FindPath(Digraph, char*, char*);
void SPT(Digraph, Vertex, Vertex*, float*);
void initialize(Digraph, Vertex, Vertex*, float*, Vertex*);
bool isConnected(Digraph);

/*Criação de um vértice da lista de adjacência.*/
lista NEWnode(Vertex, float, lista);

/*Criação de uma aresta*/
Edge EDGE(int, int, float);	

/*Funções auxiliares de busca de índice do vértice e remoção de vírgulas da string.*/
int VERTEXreturn(Digraph, char*);
void removeComma(char*);