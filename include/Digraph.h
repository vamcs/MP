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
 *  Detailed Constrói e retorna um digrafo com vértices 0 ... V-1 e todas arestas definidas no arquivo input.txt
 *	passado à função. Apenas faz a alocação de memória para o digrafo.
 *
 *	Funções Clientes: todo o programa.
 *
 *	Funções Servidoras: -
 *
 *	Assertivas de Entrada: -
 *
 *	Assertivas de Saída:
 *	- Retorna uma estrutura Digraph G com memória alocada. É verificado se o malloc funcionou corretamente.
 *	Caso contrário, o programa é abortado.
 *
 *  \param
 *  \return Digrafo do tipo Digraph contendo todas as definições lidas a partir do arquivo de entrada.
 */
Digraph DIGRAPHinit();

/*! \Brief Insere uma aresta no grafo.
 *
 *  Detailed Insere um arco v-w contido na struct Edge 'e' no digrafo 'G'.
 *
 *	Funções Cliente: DIGRAPHinsertV(), NEWreqs(), NEWreqs_id().
 *
 *	Funções Servidoras: NEWnode(), DIGRAPHadj().
 *
 *	Assertivas de Entrada:
 *	- Digraph G contendo todas as tarefas
 *	- Edge e contendo uma aresta v->w que deverá ser checada.
 *
 *	Assertivas de Saída:
 *	- Insere uma aresta caso v->w não exista e se e.id seja igual à ID de w.
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
 *	Funções Clientes: main.
 *
 *	Funções Servidoras: -
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
/*!	\Brief Esta função cria um novo nó da lista de adjacências ao receber as informações necessárias contidas em cada um dos nós.
*	
*	Detailed Função que cria e retorna um novo nó para uma lista de adjência contendo um índice w para
* 	um vértice do digrafo, a ID desse vértice e um ponteiro para o próximo nó da lista de adjacência.
*	
*	Funções Clientes: DIGRAPHinsertE().
*
*	Funções Servidoras: -
*
*	Assertivas de Entrada:
*	- Índice v de um vértice contendo uma tarefa
*	- ID do tipo inteiro
*	- Ponteiro do tipo LinkedList para o próximo elemento na lista.
*
*	Assertivas de Saída:
*		Retorna o novo nó da lista de adjacência para a função DIGRAPHinsertE().
*
*  	\param v Vértice de destino.
*  	\param int contendo o id do vértice de destino.
*	\param ponteiro do tipo LinkedList contendo o próximo elemento da lista de adjacências.
*  	\return novo nó do tipo LinkedList para uma lista de adjacências.
*/
LinkedList NEWnode(Vertex, int, LinkedList);

/*! \Brief Cria uma aresta para o grafo.
 *
 *  Detailed Esta função simplesmenet inicializa uma estrutura do tipo Edge contendo os valores de aresta que serão pas-
 *	sados a outra função que as utilizará de acordo com o que for implementado.
 *
 *	Funções Clientes: DIGRAPHinsertV(), DIGRAPHremoveV().
 *
 *	Funções Servidoras: 
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

/*! Brief/ Busca por um ID no digrafo.
*
*	Detailed Esta função percorre o array de vértices em busca de um ID específico.
*
*	Funções Cliente: DIGRAPHinsertV(), DIGRAPHremoveV(), TIME(), NEWid(), NEWreqs(), NEWreqs_id(), modify(), execution(), modification_window().
*
*	Funções Servidoras: -
*
*	Assertivas de Entrada:
*	- Digraph G contendo o digrafo com as tarefas.
*	- Inteiro contendo o ID a ser buscado.
*
*	Assertivas de Saída:
*	- A função busca pelo ID e caso seja encontrado retorna a sua posição no array de vértices.
*	- Caso o digrafo esteja vazio ou o ID seja inválido, retorna -1.
*
*	\param Digraph G
*	\param int id
*	\return int posição do vértice que contém id
*/
int VERTEXreturn(Digraph, int);

/*! Brief/ Faz a conversão da string lida para um vértice do digrafo do tipo VertexArray.
*
*	Detailed Esta função cria um nó do array de vértices do tipo VertexArray a partir de uma string no mesmo
*	formato do arquivo de entrada "input.txt". Todos os campos são separados e formatados corretamente.
*
*	Funções Cliente: DIGRAPHread(), main().
*
*	Funções Servidoras: -
*
*	Assertivas de Entrada:
*	- String contendo uma linha completa conforme arquivo de entrada.
*
*	Assertivas de Saída:
*	- Nó do array de vértices do digrafo é retornado se toda string estiver correta. Caso haja erros, retorna NULL.
*
*	\param char* str contendo uma linha no formato padrão de uma tarefa.
*	\return	um nó do tipo VertexArray contendo uma nova tarefa.
*/
VertexArray cnvInputStrToVertex(char* str);

/*! Brief/ Busca por um ID dentro de um array de IDs.
*
*	Detailed Dada uma tarefa, é feita uma busca por um ID em seu array de dependências. O parâmetro reqs garante
*	que apenas posições permitidas serão acessadas.
*
*	Funções Cliente: DIGRAPHinsertV(), DIGRAPHremoveV().
*
*	Funções Servidoras: -
*
*	Assertivas de Entrada:
*	- Array com pré-requisitos de uma tarefa
*	- Número de elementos nesse array (reqs)
*	- Elemento a ser buscado
*
*	Assertivas de Saída:
*	- Retorna a posição do valor caso seja encontrado.
*	- Retorna zero se ID é invalido ou não está presente.
*
*	\param ponteiro para int reqs_id
*	\param int reqs
*	\param int id
*	\return posição de id se for sucesso. -1 no erro.
*/
int FINDreqs_id(int*, int, int);

/*! Brief/ Verificação de inteiros.
*
*	Detailed Verifica se o valor entradado é maior ou igual a zero.
*
*	Funções Cliente: DIGRAPHinsertV(), DIGRAPHread().
*
*	Funções Servidoras: -
*
*	Assertivas de Entrada:
*	- Valor inteiro a ser checado.
*
*	Assertivas de Saída:
*	- Retorna true caso o valor ser maior ou igual a zero.
*	- Retorna false caso contrário.
*
*	\param valor inteiro
*	\return true se valor >= 0, false caso contrário.
*/
bool INPUTcheck(int);

/*! Brief/ Verifica se o nome entrado já existe no digrafo.
*
*	Detailed A função percorre o array de vértices em busca da string entrada.
*
*	Funções Cliente: DIGRAPHinsertV(), DIGRAPHread().
*
*	Funções Servidoras: -
*
*	Assertivas de Entrada:
*	- Digrafo G contendo todas as tarefas.
*	- String a ser verificada.
*	- Valor inteiro com o tamanho do array de G.
*
*	Assertivas de Saída:
*	- Caso o nome já tiver sido usado, retorna false. 
*	- Caso contrário, é um novo nome. 
*
*	\param digrafo G contendo as tarefas
*	\param string contendo o novo nome
*	\param inteiro contendo o tamanho do array de vértices
*	\return true se a string não existe em G e false se um vértice já possuir esse nome.
*/
bool NAMEcheck(Digraph, char*, int);

/*! Brief/ Leitura do arquivo de entrada e inserção dos valores no digrafo.
*
*	Detailed Faz a leitura do arquivo de entrada, cria a string contendo cada linha e a insere no digrafo.
*
*	Funções Cliente: main().
*
*	Funções Servidoras: DIGRAPHinit(), readFileLine(), cnvInputStrToVertex(), DIGRAPHinsertV(), NAMEcheck(), INPUTcheck().
*
*	Assertivas de Entrada:
*	- Digraph G inicializado por DIGRAPHinit().
*
*	Assertivas de Saída:
*	- O grafo completo de acordo com o arquivo "input.txt" é inicializado ao fim desta função.
*	- Verificações de erro de inserção de vértice são feitas, caso as linhas lidas não atendem aos requisitos.
*
*	\param Digraph vazio e inicializado
*	\return
*/
void DIGRAPHread(Digraph);