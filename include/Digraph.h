#ifndef DIGRAPH_H
#define DIGRAPH_H
#endif

/*Cabeçalho de Definições do Grafo*/
#include <stdbool.h>

/*Declaração dos tipos utilizados*/
#define Vertex int

typedef struct node * LinkedList;
typedef struct vertexArray * VertexArray;
typedef struct digraph * Digraph;
typedef struct edge Edge;


enum DigraphErrors{
    DigraphInvalidStringError = -1,
    DigraphInvalidDependenceOnFileError = -2,
    DigraphEdgeAlreadyExistsError = -3,
    DigraphOriginEqualDestinyError = -4,
    DigraphInvalidEdgeIDError = -5,
    DigraphInvalidVertexIDError = -6,
    DigaphVertexAlreadyExistError = -7,
    DigraphInvalidVertexDurationError = -8,
    DigraphInvalidVertexMinStartError = -9,
    DigraphInvalidVertexReqsError = -10,
};


/*Estruturas do digrafo*/

/*! \Brief Estrutura de dados que representa um nó da LinkedList encadeada.
 */
struct node {				/*!< Nós do tipo "node" representam os nós da LinkedList encadeada.*/
	Vertex w;				/*!< Posição do vértice adjacente no array de vértices, contido neste nó da LinkedList.*/
	int id;					/*!< Indentificador único do nó.*/
	LinkedList next; 				/*!< próximo nó da LinkedList de adjacência.*/
};

/*! \Brief Estrutura de dados que representa a LinkedList de adjacências do grafo.
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
	LinkedList adj;				/*!< Lista de vértices os quais a tarefa corrent é dependente.*/
};

/*! \Brief Estrutura de dados que representa a cabeça do grafo.
 */
struct digraph {
	int V;					/*!< Quantidade de vértices no grafo.*/
	int E;					/*!< Quantidade de arestas no grafo.*/
	VertexArray * array;	/*!< Ponteiro para o vetor de LinkedLists de adjacências.*/
};

/*! \Brief Estrutura de dados que representa uma aresta do grafo.
 */
struct edge {				//Define uma aresta (edge) v -> w.
	Vertex v;               /*!< Vértice de origem.*/
	Vertex w;               /*!< Vértice de destino*/
	int id;					/*!< Identificador da aresta*/
};

/*! \Brief Inicialização do Digrafo
 *
 *  Detailed Constrói e retorna um digrafo com vértices 0 ... V-1 e todas arestas definidas no arquivo txt
 *	passado à função.
 *
 *  \param file Nome do arquivo de definições do grafo.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
Digraph DIGRAPHinit();

/*! \Brief Insere uma aresta no grafo.
 *
 *  Detailed Insere um arco v-w contido na struct Edge 'e' no digrafo 'G'.
 *
 *  \param G Grafo onde será inserida a aresta 'e'
 *  \param e Aresta a ser inserida no grafo 'G'
 *  \return
 */
void DIGRAPHinsertE(Digraph, Edge);

/*! \Brief Remove uma aresta no grafo.
 *
 *  Detailed Remove um arco v-w contido na struct Edge 'e' no digrafo 'G'.
 *
 *  \param G Grafo onde será removida a aresta 'e'
 *  \param e Aresta a ser removida no grafo 'G'
 *  \return
 */
void DIGRAPHremoveE(Digraph, Edge);

/*! \Brief Insere um vértice no grafo.
 *
 *  \param G Grafo onde será inserido o vértice.
 *  \return
 */
int DIGRAPHinsertV(Digraph, VertexArray w, bool (*inputCheck)(int), bool (*nameCheck)(Digraph, char*, int));

/*! \Brief Remove um vértice no grafo.
 *
 *  \param G Grafo onde será removido o vértice.
 *  \return
 */
int DIGRAPHremoveV(Digraph, int);

/*! \Brief Esta função faz a verificação da existência de uma aresta checando a LinkedList de adjacência de um
 *  vértice do digrafo G.
 *
 *  \param G Grafo onde será verificada a existência da aresta.
 *  \param v Vértice de origem.
 *  \param w Vértice de destino.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
bool DIGRAPHadj(Digraph, Vertex, Vertex);

/*! \Brief Desaloca um grafo.
 *
 *  Detailed Esta função desaloca o digrafo G entrado, assim como cada vértice contido em seu array de vértices,
 *  além de cada um dos nós das LinkedLists de adjacências de cada um destes vértices.
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

/*Criação de um vértice da LinkedList de adjacência.*/
LinkedList NEWnode(Vertex, int, LinkedList);

/*! \Brief Cria uma aresta para o grafo.
 *
 *  Detailed Esta fun ção simplesmenet inicializa uma estrutura do tipo Edge contendo os valores de aresta que serão pas-
 *	sados a outra função que as utilizará de acordo com o que for implementado.
 *
 *  \param v Vértice de origem.
 *  \param w Vértice de destino.
 *  \param id Identificador da Aresta.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
Edge EDGE(Vertex, Vertex, int);

/*Funções auxiliares de busca de índice do vértice e remoção de vírgulas da string.*/
int VERTEXreturn(Digraph, int);
//void removeComma(char*);
VertexArray cnvInputStrToVertex(char* str);

int FINDreqs_id(int*, int, int);

bool INPUTcheck(int);
bool NAMEcheck(Digraph, char*, int);