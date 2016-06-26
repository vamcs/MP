#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/Digraph.h"
#include "../include/manager.h"
#include "../include/fileReader.h"

/*Definição do tipo das variáveis de endereçamento dos vértices (int).*/
#define Vertex int

/*Alocação de memória para o digrafo.*/
Digraph DIGRAPHinit()
{
	Vertex 	v = 0;
	int 	V = 0; 
	
	/*Declaração e inicialização do digrafo.*/
	Digraph G = (Digraph)malloc(sizeof *G);
	assert(G);
	
	/*Alocação de memória do vetor de listas adjacentes.*/
	/*As listas de adjacência são inicializadas como nulas.*/
	G->array = (VertexArray*)malloc(V * sizeof(VertexArray));
	assert(G->array);

	G->V = v;
	G->E = 0;

	return G;
}

/*Leitura do arquivo de entrada e inserção dos valores no digrafo.*/
void DIGRAPHread(Digraph G)
{
	/*Ponteiros de função - Checam a validade de entradas e nomes.*/
	bool (*nameCheck)(Digraph, char*, int);
	nameCheck = NAMEcheck;

	bool (*inputCheck)(int);
	inputCheck = INPUTcheck;

	/*Abertura do arquivo de entrada*/
	FILE* fp = fopen("./supporting_files/input.txt","r");
	assert(fp);
	while (!feof(fp)) {
		/*Lê uma linha inteira do arquivo em vertexSTR.*/
		char* vertexSTR = readFileLine(fp);
		/*Converte essa linha em um vértice do grafo do tipo VertexArray.*/
		VertexArray array = cnvInputStrToVertex(vertexSTR);
		/*Insere o vértice lido em G.*/
		/*DIGRAPHinsertV retorna 0 se funcionou normalmente.*/
		int error_check = DIGRAPHinsertV(G, array, inputCheck, nameCheck);
		assert(error_check == 0);
	}
	fclose(fp);
}

/*Insere um vértice do tipo VertexArray no digrafo G.*/
int DIGRAPHinsertV(Digraph G, VertexArray w, bool (*inputCheck)(int), bool (*nameCheck)(Digraph, char*, int)) 
{
	/*Checa se o nome é válido e não existe no grafo.*/
	if (!nameCheck(G, w->name, G->V)) 
	{
		return DigraphInvalidStringError;
	} /*if*/

	/*Checa se a duração é 0 ou negativa.*/
	if (!inputCheck(w->duration) || w->duration == 0) 
	{
		return DigraphInvalidVertexDurationError;
	} /*if*/

	/*Checa se o início mínimo é negativo.*/
	if (!inputCheck(w->min_start)) 
	{
		return DigraphInvalidVertexMinStartError;
	} /*if*/

	/*Checa se o número de pré-requisitos é negativo ou maior do que o total de vértices.*/
	if ((!inputCheck(w->reqs)) || (w->reqs > G->V))  
	{
		return DigraphInvalidVertexReqsError;
	} /*if*/
    
    /*Se o número de pré-requisitos for igual a 0, verifica se é o array de dependências é nulo.*/
	if (w->reqs == 0)
	{
		assert(w->reqs_id == NULL);
	} else /*Caso contrário, verifica se o array não é nulo.*/
	{
		assert(w->reqs_id);
	} /*if*/

	int i;
	/*Verifica se as dependências são válidas. 
	* Elas só podem ser de tarefas anteriores à tarefa atual no arquivo de entrada.
	*/
	for (i = 0; i < w->reqs; i++) 
	{
		Vertex k = VERTEXreturn(G, w->reqs_id[i]);
		/*Caso k == -1, o ID não existe.*/
		if ((k == -1))
		{
			//printf("Returning Here = %d",FINDreqs_id(w->reqs_id, w->reqs, k));
			return DigraphInvalidVertexReqsError;
		} /*if*/
	} /*for*/

	/*Inserção do novo vértice em G.*/
	G->V++;
	G->array 				= (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));
	assert(G->array);
	G->array[G->V - 1] 		= w;
	assert(G->array[G->V-1]);

	/*Insere o ID da tarefa nas listas de adjacência das tarefas que ela depende.*/
	for (i = 0; i < w->reqs; i++) {
		Vertex k = VERTEXreturn(G, w->reqs_id[i]);
		DIGRAPHinsertE(G, EDGE(k, G->V - 1, w->id));
	} /*for*/

	/*Nenhuma tarefa pode depender desta nova, pois é a última do arquivo de entrada.*/
	G->array[G->V - 1]->adj = NULL;

	/*Calcula o tempo de execução dessa tarefa.*/
	TIME(G, G->V - 1);
	return 0;
}

/*Faz a conversão da string lida para um vértice do digrafo do tipo VertexArray.*/
VertexArray cnvInputStrToVertex(char* str)
{
	/*Aloca memória para o novo vértice.*/
	VertexArray w = (VertexArray)malloc(sizeof(struct vertexArray));
	
	/*A seguir é feito o tratamento da string para separar os campos necessários.*/
	char* resp = strtok(str, "\'");
    char* ch;
    int i = 0,
    	j = 0;

	for (i = 0; i < 6; i++) 
	{
    	switch(i)
    	{
    		case 0:
    			w->id = strtol (resp, NULL, 0);
    			break;
    		case 1:
    			ch = resp;

    			for (j = 0; j < (int) sizeof(w->name); j++)
    			{
    				if (*ch == '\'')
    				{
    					w->name[j] = '\0';
    					break;
    				} /*if*/
    				w->name[j] = *ch;
    				ch = ch+1;
    			} /*if*/
    			break;
    		case 2:
    			w->exec = strtol (resp, NULL, 0);
    			break;
    		case 3:
    			w->duration = strtol (resp, NULL, 0);
    			break;
    		case 4:
    			w->min_start = strtol (resp, NULL, 0);
    			break;
    		case 5:
    			w->reqs = strtol (resp, NULL, 0);
    			break;
    		default:
    			break;
    	} /*switch*/
    	if (i == 0)
    	{
            resp = strtok(NULL, "\'");
            if (resp == NULL)
            {
    			free(w);
    			free(str);
    			return NULL;
    		} /*if*/
        } else if (i != 5)
        {
    		resp = strtok (NULL, " "); 
    		if (resp == NULL)
    		{
    			free(w);
    			free(str);
    			return NULL;
    		} /*if*/
    	} /*if*/
    } /*for*/

    /*Se o número de requisitos lido foi maior do que 0, então há pré-requisitos.*/
    /*Aloca-se memória para o array de pré-requisitos e os insere lá.*/
    if (w->reqs > 0)
    {
    	int k;
    	w->reqs_id = (int*)malloc(w->reqs * sizeof(int));
 		for (k = 0; k < w->reqs; k++) 
 		{
 			resp = strtok(NULL, " ");
 			w->reqs_id[k] = strtol(resp, NULL, 0);
 		} /*for*/
    } else /*Caso contrário, o array de pré-requisitos será nulo.*/
    {
    	w->reqs_id = NULL;
    } /*if*/

    /*Libera memória da string de leitura.*/
    free(str);

    /*Retorna o novo vértice.*/
    return w;
}

/*Faz inserção de uma nova aresta no digrafo.*/
int DIGRAPHinsertE(Digraph G, Edge e) 
{
	/*Leitura de Edge 'e'.*/
	Vertex v = e.v, w = e.w;
	int id = e.id;
	if (!DIGRAPHadj(G, v, w)) /*Verifica se a aresta já existe.*/
	{
		/*Cria um novo node e o insere no início da LinkedList de adjacência.*/
		G->array[v]->adj = NEWnode(w, id, G->array[v]->adj);
		G->E++;	/*Incremento número de arestas adicionadas no grafo.*/
	} else
	{
		return DigraphEdgeAlreadyExistsError;
	} /*if*/
	return 0;
}

/*Função que cria e retorna um novo nó para uma lista de adjência
* contendo um índice w para um vértice do digrafo, a ID desse vér-
* tice e um ponteiro para o próximo nó da lista de adjacência.
*/
LinkedList NEWnode(Vertex w, int id, LinkedList next)
{
	LinkedList a;
	a 		= (LinkedList)malloc(sizeof (struct node));
	a->w 	= w;
	a->id 	= id;
	a->next = next;
	return a;
}

/*Faz a remoção de uma aresta da lista de adjacência de um vértice do digrafo.*/
int DIGRAPHremoveE(Digraph G, Edge e)
{
	Vertex 	v = e.v, 
			w = e.w;
	LinkedList 	current, 
			prev;
	if (DIGRAPHadj(G, v, w))
	{
		/*Encontra o nó a ser removido*/
		for (current = G->array[v]->adj; (current != NULL) && (current->w != w); current = current->next)
		{
			/*Corpo vazio.*/
		} /*for*/
		if (G->array[v]->adj != current) /*Caso este nó não seja o primeiro da LinkedList.*/
		{
			/*Procura pelo nó imediatamente anterior a esse.*/
			for (prev = G->array[v]->adj; (prev != NULL) && (prev->next != current); prev = prev->next)
			{
				/*Corpo vazio.*/
			} /*for*/
			if(e.id == current->id)
			{ 
				prev->next = current->next;
				free(current);
				G->E--;
			} else 
			{
				return DigraphInvalidEdgeIDError;
			} /*if*/
		} else if (e.id == current->id)
		{
			G->array[v]->adj = current->next;		/*É o primeiro da LinkedList, então passa o seguinte à cabeça da LinkedList e desloca o nó.*/
			free(current);
			G->E--;
		} else /*A primeira aresta é inválida.*/
		{
			return DigraphInvalidEdgeIDError;
		}/*if*/
	} else 
	{
		return DigraphEdgeNotFoundError;
	} /*if*/

	return 0;
}

/*Esta função remove um vértice do digrafo. Atualiza as listas de adjacência, assim com*/
int DIGRAPHremoveV(Digraph G, int id)
{
	Vertex v = VERTEXreturn(G, id);

	if (v == -1)
	{
		return DigraphInvalidEdgeIDError;
	} /*if*/
	
	Vertex w;
	int i, 
		j;
	LinkedList temp 				= G->array[v]->adj;
	LinkedList l 				= G->array[v]->adj;
	VertexArray temp_array 	= G->array[v];

	/*Remover referências ao vértice.*/
	/*Verificar dependências.*/
	if (G->array[v]->reqs > 0)
	{
		for (i = 0; i < G->array[v]->reqs; i++)
		{
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);
			DIGRAPHremoveE(G, EDGE(w, v, id));
		} /*for*/
		free(G->array[v]->reqs_id);
	} /*if*/

	/*Verificar lista de adjacência (vértices que dependem deste).*/
	while (l != NULL) 
	{
		w = l->w;
		/*Encontra a posição de ID no array de IDs do vértice w.*/
		i = FINDreqs_id(G->array[w]->reqs_id, G->array[w]->reqs, G->array[v]->id);
		for(j = i; j < G->array[w]->reqs-1; j++) /*Remoção da ID do array de IDs*/
		{		
			G->array[w]->reqs_id[j] = G->array[w]->reqs_id[j+1];
		} /*for*/
		/*Decrementa a quantidade de elementos no array de pré-requisitos e realoca o tamanho do vetor, efetuando a remoção.*/
		G->array[w]->reqs--;
		G->array[w]->reqs_id = (int*)realloc(G->array[w]->reqs_id, G->array[w]->reqs * sizeof(int));
		temp 	= l;
		l 		= l->next;
		free(temp);
		G->E--;
	} /*while*/
	
	/*Rearranjo dos vértices no vetor para exclusão do vértice desejado.*/
	for(i = v; i < G->V - 1; i++) 
	{
		G->array[i] = G->array[i+1];
	} /*if*/
	G->V--;
	free(temp_array);
	G->array = (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));
	if(G->V == 0) {
		G->array = nullptr;
        assert(G->array == nullptr);
    } else {
	    assert(G->array);
    }

	/*Caso o vértice removido não tiver sido o último do vetor, é necessário corrigir os índices
	  dos nós das LinkedLists de adjacência.*/
	if(v < G->V+1) 
	{
		for(i = 0; i < G->V; i++) /*Percorre o array de vértices.*/
		{
			if(G->array[i]->adj != NULL) /*Verifica se o vértice possui LinkedList de adjcência.*/
			{
				for(l = G->array[i]->adj; l != NULL; l = l->next) /*Percorre a LinkedList de adjacência do vértice.*/
				{
					if (l->w >= v)
					{
						l->w--; /*Se o índice dessa lista for maior ou igual ao índice do vértice removido, atualize-o.*/
					}
				} /*for*/
			} /*if*/
			TIME(G, i);	 /*Recalcula o tempo da tarefa em i*/
		} /*for*/
	} /*if*/

	return 0;
}


/*Cria e retorna uma aresta do digrafo contendo dois índices que indicam a direção da aresta e o ID do vértice w.*/
Edge EDGE(Vertex v, Vertex w, int id)
{
	Edge e;
	if (v >= 0 && w >= 0 && id >= -1)
	{
		e.v = v;
		e.w = w;
		e.id = id;
	} else 
	{
		printf("Aresta invalida. Valores menores do que zero inseridos.\n");
		printf("Error code: -1.\n");
		e.v 	= -1;
		e.w 	= -1;
		e.id 	= -1;
	} /*if*/
	return e;
}

/*
	Esta função faz a verificação da existência de uma aresta checando a lista de adjacência de um
	vértice do digrafo G.
	
	Utilização da função:
	
	bool checa = DIGRAPHadj(G, v, w);
	
	- Paramêtros da função: digrafo do tipo Digraph e dois vértices do tipo Vertex.
	
	- Retorno da função: true (1) caso a aresta exista, false (0) caso contrário.

	As funções de inserção e remoção de arestras fazem uso desta função.
*/
bool DIGRAPHadj(Digraph G, Vertex v, Vertex w) 
{
	LinkedList x;
	for (x = G->array[v]->adj; x != NULL; x = x->next)
	{
		if (x->w == w) 
		{
			return true;
		} /*if*/
	} /*for*/
	return false;
}

/*Desaloca todo o digrafo, incluindo listas de adjacência, arrays de IDs e array de vértices.*/
void DIGRAPHdestroy(Digraph G) 
{
	int v;
	LinkedList temp, l;
	
	/*Desalocar LinkedLists encadeadas*/
	for (v = 0; v < G->V; v++) 
	{
		l = G->array[v]->adj; 
		while (l != NULL) 
		{
			temp 	= l;
			l 		= l->next;
			free(temp);
		} /*while*/
		l = NULL;
	} /*for*/

	/*Desalocar vetor de IDs*/
	for (v = 0; v < G->V; v++) 
	{
		if (G->array[v]->reqs > 0) 
		{
			free(G->array[v]->reqs_id);
		} /*if*/
	} /*for*/
	
	/*Desalocar array de LinkedLists*/
	for (v = 0; v < G->V; v++) 
	{
		free(G->array[v]);
	} /*for*/
	free(G->array);
	G->array = NULL;
	
	/*Desalocar grafo*/
	free(G);
	G = NULL;
}

/*Busca por um ID no digrafo. Retorna a posição caso encontre ou -1 se ocorrer um erro.*/
Vertex VERTEXreturn(Digraph G, int id)
{
	Vertex v = 0;
	
	/*Caso não haja arestas no grafo.*/
	if (G->V == 0)
	{
		return -1;
	} /*if*/

	/*Percorre o array de vértices em busca da ID desejada.*/
	while (G->array[v]->id != id && v < G->V - 1) 
	{
		v++;
	} /*while*/
	if (G->array[v]->id == id) 
	{
		return v;
	} else
	{
		return -1;
	} /*if*/
}

/*Salva a configuração atual do digrafo em um arquivo de saída no mesmo formato do arquivo de entrada.*/
int DIGRAPHsave(Digraph G) 
{
	FILE* fp = fopen("output.txt", "w");
	int i, j, id, exec, duration, min_start, reqs;
	char name[100];
	
	if(fp == NULL){
		return -1;
	}
	/*Imprime os G->V vértices do digrafo.*/
	for (i = 0; i < G->V; i++) 
	{
		id 			= G->array[i]->id;
		strcpy(name, G->array[i]->name);
		exec 		= G->array[i]->exec;
		duration 	= G->array[i]->duration;
		min_start 	= G->array[i]->min_start;
		reqs 		= G->array[i]->reqs;

		/*Impressão de uma linha.*/
		fprintf(fp, "%d '%s' %d %d %d %d", 
					id, name, exec, duration, min_start, reqs);
		
		/*Se houver pré-requisitos, imprime-os a seguir.*/
		if (reqs > 0) 
		{
			for (j = 0; j < reqs; j++) 
			{
				fprintf(fp, " %d", G->array[i]->reqs_id[j]);
			} /*for*/
		} /*if*/
		if (i < G->V - 1) 
		{
			fprintf(fp, "\n");
		} /*if*/
	} /*for*/
	fclose(fp);
	return 0;
}

/*Busca por um ID dentro de um array de IDs. 
* Retorna a posição caso encontre ou -1 se não existir.
*/
int FINDreqs_id(int* reqs_id, int reqs, int id) 
{
	int i;

	for (i = 0; i < reqs; i++) {
		if (id == reqs_id[i]) {
			return i;
		}
	}
	return -1;
}

/*Esta função faz a simples verificação de um valor menor do que zero.
* Se menor do que zero, retorna falso. Caso contrário, verdadeiro.
*/
bool INPUTcheck(int value) 
{
	if (value < 0)
	{
		return false;
	} /*if*/

	return true;
}

/*Esta função verifica a existência de um nome no digrafo.
* Isto serve pra evitar a inserção de duplicatas.
*/
bool NAMEcheck(Digraph G, char* name, int V)
{
	Vertex w;
	
	for (w = 0; w < V; w++) 
	{
		if (strcmp(G->array[w]->name, name) == 0) 
		{
			return false;
		} /*if*/
	} /*for*/
	
	return true;
}
