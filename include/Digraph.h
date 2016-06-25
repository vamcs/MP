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
 *  Detailed Remove uma aresta v->w contido na struct Edge 'e' no digrafo 'G'.
 *
 *	Funções Cliente: DIGRAPHremoveV(), NEWreqs(), NEWreqs_id().
 *
 *	Funções Servidoras: DIGRAPHadj(), VERTEXarray().
 *
 *	Assertivas de Entrada:
 *	- Digraph G contendo todas as tarefas
 *	- Edge e contendo uma aresta v->w que deverá ser checada.
 *
 *	Assertivas de Saída:
 *	- Remove uma aresta caso v->w exista e se e.id seja igual à ID de w.
 *	- 
 *
 *
 *  \param G Grafo onde será removida a aresta 'e'
 *  \param e Aresta a ser removida no grafo 'G'
 *  \return
 */
void DIGRAPHremoveE(Digraph, Edge);

/*! \Brief Insere um vértice no grafo.
 *
 *	Detailed Esta função insere um novo vértice no grafo, inserindo também todas as referências necessárias para
 *	a nova tarefa. São feitas várias verificações de consistência do nó inserido.
 *
 *	Funções Cliente: GUI(), DIGRAPHread().
 *
 *	Funções Servidoras: GUI(), TIME(), cnvInputStrToVertex().
 *
 *	Assertivas de Entrada:
 *	- Digrafo contendo todas as tarefas.
 *	- Novo vértice contendo uma nova tarefa.
 *
 *	Assertivas de Saída:
 *	- Verificações quanto a consistência dos valores contidos em VertexArray w que são:
 *		- Se o ID já existe no digrafo.
 *		- Se o nome já existe no digrafo.
 *		- Se a execução é um valor maior do que zero.
 *		- Se a duração é maior do que zero.
 *		- Se o início mínimo é maior ou igual a zero.
 *		- Se o número de pré-requisitos é válido:
 *			- Maior ou igual a zero;
 *			- Menor do que o índice da tarefa. Não é possível referenciar tarefas que venha abaixo da tarefa atual no arquivo de entrada.
 *	- São adicionados nós às listas de adjacência dos pré-requisitos da tarefa atual e estas tarefas salvas no array de pré-requisitos.
 *	- Retorna 0  caso o VertexArray está de acordo com o esperado ou um valor negativo de acordo com a tabela acima, indicando o erro.
 * 	- O tempo de execução da tarefa é calculado.
 *
 *  \param G Grafo onde será inserido o vértice.
 *	\param ponteiro para função INPUTcheck.
 *	\param ponteiro para função NAMEcheck.
 *  \return 0 para sucesso, valor negativo para erro. Checar tabela.
 */
int DIGRAPHinsertV(Digraph, VertexArray w, bool (*inputCheck)(int), bool (*nameCheck)(Digraph, char*, int));

/*! \Brief Remove um vértice no digrafo.
 *
 *	Detailed Esta função faz a remoção completa de um vértice do digrafo. Primeiro, recebe-se uma ID e esta é buscada
 *	dentro do digrafo G. Caso seja encontrada, todas as referências a entre vértice serão removidas e o vértice será 
 *	desalocado por completo.
 *
 *	Funções Clientes: GUI().
 *
 *	Funções Servidoras: GUI(), TIME(), VERTEXreturn(), DIGRAPHremoveE().
 *
 *	Assertivas de Entrada:
 *	- Digrafo G do tipo Digraph contendo todas as tarefas.
 *	- ID do tipo inteiro contendo o ID da tarefa a ser removida.
 *
 *	Assertivas de Saída:
 *		Caso o ID exista, a tarefa será removida e várias referências a esta tarefa precisam ser tratadas:
 *		- Se existir outras tarefas que possuam a tarefa a ser removida como dependência, será necessário deletar o ID dos arrays de pré-requisitos.
 *		- Se esta tarefa possuir pré-requisitos, será necessário remover as arestas existentes nas tarefas no array de pré-requisitos da tarefa atual.
 *		- Completa deleção da tarefa e desalocação da memória.
 *		- Os tempos de execução de todas tarefas são recalculados.
 *
 *  \param G Grafo onde será removido o vértice.
 *  \return 0 para sucesso, -1 para erro.
 */
int DIGRAPHremoveV(Digraph, int);

/*! \Brief Esta função faz a verificação da existência de uma aresta checando a lista de adjacência de um
 *  vértice do digrafo G.
 *
 *	Funções Clientes: DIGRAPHremoveE().
 *
 *	Funções Servidoras: -
 *
 *	Assertivas de Entrada:
 *	- Digrafo G.
 *	- Vértice válido de origem v.
 *	- Vértice válido de origem w.
 *
 *	Assertivas de Saída:
 *		A função verifica se a aresta v->w existe em G, retornando true para sucesso e false para insucesso. Além
 *		disso, caso algum dos nós não exista, a função retorna com falso.
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
 *	Assertivas de Entrada:
 *	- Digrafo do tipo G alocado e contendo todas as tarefas.
 *	
 *	Assertivas de Saída:
 *		Digrafo G desalocado com todas a memória alocada para si liberada ao sistema.
 *
 *  \param G Grafo onde será verificada a existência da aresta.
 *  \param v Vértice de origem.
 *  \param w Vértice de destino.
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo passado.
 */
void DIGRAPHdestroy(Digraph);

/*! \Brief Criação de um arquivo de saída.
 *
 *  Detailed Esta função salva a configuração atual do digrafo G em um arquivo output.txt de acordo com as especificações
 *	e o arquivo de entrada.
 *
 *	Assertivas de Entrada:
 *	- Digrafo do tipo Digraph G contendo todas as tarefas.
 *
 *	Assertivas de Saída:
 *		Imprime e salva um arquivo txt contendo as tarefas do digrafo e todos os seus campos.
 *
 *  \param G Digraph contendo o digrafo com todas as tarefas.
 *  \return Um arquivo txt contendo todas as definições
 */
void DIGRAPHsave(Digraph);

/*Criação de um vértice da LinkedList de adjacência.*/
LinkedList NEWnode(Vertex, int, LinkedList);

/*! \Brief Cria uma aresta para o grafo.
 *
 *  Detailed Esta função simplesmenet inicializa uma estrutura do tipo Edge contendo os valores de aresta que serão pas-
 *	sados a outra função que as utilizará de acordo com o que for implementado.
 *
 *	Assertivas de Entrada:
 *	- Dois vértices do tipo Vertex que compõe a aresta no sentido v -> w
 *	- ID do segundo vértice (w).
 *
 *	Assertivas de Saída:
 *		Retorna uma estrutura Edge contendo dois vértices e uma ID. Caso a ID não seja igual a ID de w, um erro ocorrerá
 *		ao inserir ou remover esta aresta.
 *
 *  \param v Vértice de origem.
 *  \param w Vértice de destino.
 *  \param id Identificador da Aresta.
 *  \return Edge e contendo a definição de uma aresta.
 */
Edge EDGE(Vertex, Vertex, int);

/*Funções auxiliares de busca de índice do vértice e remoção de vírgulas da string.*/
int VERTEXreturn(Digraph, int);

VertexArray cnvInputStrToVertex(char* str);

int FINDreqs_id(int*, int, int);

bool INPUTcheck(int);

bool NAMEcheck(Digraph, char*, int);

void DIGRAPHread(Digraph);