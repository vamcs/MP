/*Cabeçalho de Definições do Grafo*/
#include <stdbool.h>

/*Declaração dos tipos utilizados*/
#define Vertex int

// #define bool int
// #define true 1
// #define false 0

typedef struct node * link;
typedef struct vertexArray * VertexArray;
typedef struct digraph * Digraph;
typedef struct edge Edge;

/*Estruturas do digrafo*/

struct node{				//Nós do tipo "node" representam os nós da lista encadeada.
	Vertex w;				//Posição do vértice adjacente no array de vértices, contido neste nó da lista.
	int id;					//PROJETO FINAL: ID_TAREFA
	//int weight;				//Peso da aresta.	/*TRABALHO 2*/
	link next; 				//próximo nó da lista de adjacência.
};

struct vertexArray{			//Vertex v é definido pela posição no vetor.
	int id;					//PROJETO FINAL: ID_TAREFA
	char name[100];			//Nome do vértice.	//PROJETO FINAL: NOME_TAREFA
	bool exec;				//PROJETO FINAL: TAREFA_EXECUTADA
	int duration;			//PROJETO FINAL: DURAÇÃO_TAREFA
	int min_start;			//PROJETO FINAL: INICIO_MIN_TAREFA
	int reqs;				//PROJETO FINAL: número de PRÉ_REQUISITOS_TAREFA
	int time;				//PROJETO FINAL: tempo de execução da tarefa
	//bool source;			//Indica se o vértice é origem ou não.	/*TRABALHO 2*/
	int* reqs_id;			//PROJETO FINAL; IDs dos pré-requisitos.
	link adj;				//Lista encadeada do vértice.	//PROJETO FINAL: lista contendo os vértices dos quais essa tarefa é dependente.
};

struct digraph{
	int V;					//Número de vértices.
	int E;					//Número de arestas (edges).
	VertexArray * array;	//Ponteiro para o vetor de listas de adjacência.
};

struct edge{				//Define uma aresta (edge) v -> w.
	Vertex v;
	Vertex w;
	int id;					//PROJETO FINAL: ID_TAREFA
	//int weight;				//Peso da aresta.
};

/*Inicialização do Digrafo.*/
Digraph DIGRAPHinit(char*);	

/*Inserção e remoção de arestas.*/
void DIGRAPHinsertE(Digraph, Edge);
void DIGRAPHremoveE(Digraph, Edge);

/*Inserção e remoção de vértices.*/
void DIGRAPHinsertV(Digraph);
void DIGRAPHremoveV(Digraph, int);

/*Verificação das adjacências de um nó.*/
bool DIGRAPHadj(Digraph, Vertex, Vertex);

/*Desalocação do grafo*/
void DIGRAPHdestroy(Digraph);

/*Impressão do grafo na tela e criação de um arquivo de saída*/
void DIGRAPHshow(Digraph);
void DIGRAPHsave(Digraph);

/*Funções e algoritmos de busca. SPT de Dijkstra é utilizada.*/
// float FindPath(Digraph, char*, char*);
// void SPT(Digraph, Vertex, Vertex*, float*);
// void initialize(Digraph, Vertex, Vertex*, float*, Vertex*);
// bool isConnected(Digraph);

/*Criação de um vértice da lista de adjacência.*/
link NEWnode(Vertex, int, link);

/*Criação de uma aresta*/
Edge EDGE(Vertex, Vertex, int);

/*Funções auxiliares de busca de índice do vértice e remoção de vírgulas da string.*/
int VERTEXreturn(Digraph, int);
void removeComma(char*);