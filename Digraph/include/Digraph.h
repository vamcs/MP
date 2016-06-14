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

/** \Brief Estrutura de dados que representa um nó da lista encadeada.
 */
struct node {				/*!< Nós do tipo "node" representam os nós da lista encadeada.*/
	Vertex w;				/*!< Posição do vértice adjacente no array de vértices, contido neste nó da lista.*/
	int id;					/*!< Indentificador único do nó.*/
	link next; 				/*!< próximo nó da lista de adjacência.*/
};

/** \Brief Estrutura de dados que representa a lista de adjacências do grafo.
 */
struct vertexArray {
	int id;					/*!< Identificador único do vértice.*/
	char name[100];			/*!< Nome da tarefa. */
	bool exec;				/*!< Flag para controle de execução da tarefa.*/
	int duration;			/*!< Duração da tarefa.*/
	int min_start;			/*!< Ciclo mínimo para início da tarefa.*/
	int reqs;				/*!< Quantidade de tarefas que são pré-requisitos para que esta seja executada.*/
	int time;				/*!< Tempo de execução da tarefa.*/
	int* reqs_id;			/*!< Lista de identificadores para as tarefas pré-requisito.*/
	link adj;				/*!< Lista de vértices os quais a tarefa corrent é dependente.*/
};

/** \Brief Estrutura de dados que representa a cabeça do grafo.
 */
struct digraph {
	int V;					/*!< Quantidade de vértices no grafo.*/
	int E;					/*!< Quantidade de arestas no grafo.*/
	VertexArray * array;	/*!< Ponteiro para o vetor de listas de adjacências.*/
};

/** \Brief Estrutura de dados que representa uma aresta do grafo.
 */
struct edge {				//Define uma aresta (edge) v -> w.
	Vertex v;               /*!< Vértice de origem.*/
	Vertex w;               /*!< Vértice de destino*/
	int id;					/*!< Identificador da aresta*/
};

/** \brief Inicialização do Digrafo
 *
 *  Detailed Constrói e retorna um digrafo com vértices 0 ... V-1 e todas arestas definidas no arquivo txt
 *	passado à função.
 *
 *  \param file Nome do arquivo de definições do grafo.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
Digraph DIGRAPHinit(char* file);

/*! \brief Insere uma aresta no grafo.
 *
 *  Detailed Insere um arco v-w contido na struct Edge 'e' no digrafo 'G'.
 *
 *  \param G Grafo onde será inserida a aresta 'e'
 *  \param e Aresta a ser inserida no grafo 'G'
 *  \return
 */
void DIGRAPHinsertE(Digraph G, Edge e);

/*! \brief Remove uma aresta no grafo.
 *
 *  Detailed Remove um arco v-w contido na struct Edge 'e' no digrafo 'G'.
 *
 *  \param G Grafo onde será removida a aresta 'e'
 *  \param e Aresta a ser removida no grafo 'G'
 *  \return
 */
void DIGRAPHremoveE(Digraph G, Edge e);

/*! \brief Insere um vértice no grafo.
 *
 *  \param G Grafo onde será inserido o vértice.
 *  \return
 */
void DIGRAPHinsertV(Digraph G);

/*! \brief Remove um vértice no grafo.
 *
 *  \param G Grafo onde será removido o vértice.
 *  \return
 */
void DIGRAPHremoveV(Digraph, int);

/*! \brief Esta função faz a verificação da existência de uma aresta checando a lista de adjacência de um
 *  vértice do digrafo G.
 *
 *  \param G Grafo onde será verificada a existência da aresta.
 *  \param v Vértice de origem.
 *  \param w Vértice de destino.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
bool DIGRAPHadj(Digraph G, Vertex v, Vertex w);

/*! \brief Desaloca um grafo.
 *
 *  Detailed Esta função desaloca o digrafo G entrado, assim como cada vértice contido em seu array de vértices,
 *  além de cada um dos nós das listas de adjacências de cada um destes vértices.
 *
 *  \param G Grafo onde será verificada a existência da aresta.
 *  \param v Vértice de origem.
 *  \param w Vértice de destino.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
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

/*! \brief Cria uma aresta para o grafo.
 *
 *  Detailed Esta fun ção simplesmenet inicializa uma estrutura do tipo Edge contendo os valores de aresta que serão pas-
 *	sados a outra função que as utilizará de acordo com o que for implementado.
 *
 *  \param v Vértice de origem.
 *  \param w Vértice de destino.
 *  \param id Identificador da Aresta.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
Edge EDGE(Vertex v, Vertex w, int id);

/*Funções auxiliares de busca de índice do vértice e remoção de vírgulas da string.*/
int VERTEXreturn(Digraph, int);
void removeComma(char*);