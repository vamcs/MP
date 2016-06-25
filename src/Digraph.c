#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

/*Definição do tipo das variáveis de endereçamento dos vértices (int).*/
#define Vertex int

Digraph DIGRAPHinit()
{
	Vertex 	v = 0; 
	//		w;
	int V = 0; 
	// 	i, 
	// 	id,
	// 	exec,
	// 	duration,
	// 	min_start,
	// 	reqs,
	// 	dep;
	// char 	name[100], 
			// aux;
	
	/*Declaração e inicialização do digrafo.*/
	Digraph G = (Digraph)malloc(sizeof *G);
	assert(G);
	
	/*Alocação de memória do vetor de LinkedLists adjacentes.*/
	/*As LinkedLists de adjacência são inicializadas como nulas.*/
	G->array = (VertexArray*)malloc(V * sizeof(VertexArray));
	assert(G->array);

	// for (v = 0; v < V; v++) {
	// 	G->array[v] 		= (struct vertexArray*)malloc(sizeof(struct vertexArray));
	// 	assert(G->array[v]);
	// }

	G->V = v;
	G->E = 0;

	return G;
}

int DIGRAPHinsertV(Digraph G, VertexArray w, bool (*inputCheck)(int), bool (*nameCheck)(Digraph, char*, int)) 
{
	while ((VERTEXreturn(G, w->id) != -1) || (VERTEXreturn(G, w->id) < -2)) {
		return DigraphInvalidVertexIDError;
	}

	if (!inputCheck(w->duration) || w->duration == 0) {
		return DigraphInvalidVertexDurationError;
	}

	if (!inputCheck(w->min_start)) {
		return DigraphInvalidVertexMinStartError;
	}

	if ((!inputCheck(w->reqs)) || (w->reqs > G->V))  {
		return DigraphInvalidVertexReqsError;
	} 
	if(w->reqs == 0){
		assert(w->reqs_id == NULL);
	}else{
		assert(w->reqs_id);
	}
	int i =0;
	for (i = 0; i < w->reqs; i++) {
		Vertex k = VERTEXreturn(G, w->reqs_id[i]);
		if ((k == -1) || (G->array[k]->id >= w->id) || (FINDreqs_id(w->reqs_id, w->reqs, k) != -1)){
			return DigraphInvalidVertexReqsError;
		} 
	}

	G->V++;
	G->array 				= (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));
	assert(G->array);
	G->array[G->V - 1] 		= w;
	assert(G->array[G->V-1]);

	for (i = 0; i < w->reqs; i++) {
		Vertex k = VERTEXreturn(G, w->reqs_id[i]);
		DIGRAPHinsertE(G, EDGE(k, G->V - 1, w->id));
	}

	G->array[G->V - 1]->adj = NULL;

	//printf("\n\n%d\n\n",G->array[G->V - 1]->id);
	TIME(G, G->V - 1);
	return 0;
}

VertexArray cnvInputStrToVertex(char* str){
	VertexArray w = (VertexArray)malloc(sizeof(struct vertexArray));
	//printf("%s\n",str);
	char* resp = strtok(str, "\'");
    int i=0;
	for(i = 0; i < 6; i++) { 
    	switch(i){
    		char* ch;
    		case 0:
    			w->id = strtol (resp,NULL,0);
    			//printf("(w->id = %d)\n",w->id);
    			break;
    		case 1:
    			ch = resp;
    			int j =0;
    			for(j =0; j<sizeof(w->name); j++){
    				if(*ch == '\''){
    					w->name[j] = '\0';
    					break;
    				}
    				w->name[j] = *ch;
    				ch = ch+1;
    			}
    			//printf("( w->name= %s)\n",w->name);
    			break;
    		case 2:
    			w->exec = strtol (resp,NULL,0);
    			//printf("(w->exec = %d)\n",w->exec);
    			break;
    		case 3:
    			w->duration = strtol (resp,NULL,0);
    			//printf("(w->duration = %d)\n",w->duration);
    			break;
    		case 4:
    			w->min_start = strtol (resp,NULL,0);
    			//printf("(w->min_start = %d)\n",w->min_start);
    			break;
    		case 5:
    			w->reqs = strtol (resp,NULL,0);
    			//printf("( w->reqs= %d)\n",w->reqs);
    			break;
    		default:
    			break;
    		}
    		if(i == 0){
                resp = strtok(NULL, "\'");
            }else if(i != 5){
    		  resp = strtok (NULL, " "); 
    	   }
    	}
    	//printf("%s\n",str);
    	if(w->reqs > 0){
    		int k = 0;
    		w->reqs_id = (int*)malloc(w->reqs * sizeof(int));
 			for (k = 0; k < w->reqs; k++ ) {
 				resp = strtok (NULL, " ");
 				//printf("(HERE == %s)\n",resp);
 				w->reqs_id[k] = strtol (resp,NULL,0);
 			}
    	}
    	else {
    		w->reqs_id = NULL;
    	}
    	free(str);
    	str = NULL;
    	return w;
}

bool INPUTcheck(int value) 
{
	if (value < 0)
	{
		return false;
	}

	return true;
}

bool NAMEcheck(Digraph G, char* name, int V)
{
	Vertex w;
	for (w = 0; w < V; w++) {
		if (strcmp(G->array[w]->name, name) == 0) {
			return 0;
		}
	} 
	return 1;
}

void DIGRAPHinsertE(Digraph G, Edge e) 
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
		printf("Aresta ja existente no digrafo ou vertice de destino eh origem.\n%s->%s nao foi inserida.\n", 
																	G->array[v]->name, G->array[w]->name);
	} /*if*/
	return;
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Esta função cria um novo nó da LinkedList de adjacências ao receber as informações necessárias
	contidas em cada um dos nós.
	
	Utilização da função:
	
	LinkedList l = NEWnode(w, weight, l);
	
	A função então cria um nó e o aponta para 'l':
		new_node --> l
	
	- Parâmetros da função: vértice do tipo Vertex para o qual o vértice que possui esta LinkedList
	de adjacências irá apontar, weight do tipo float que contém o peso da aresta criada e nó de
	origem da LinkedList de adjacência, já que novos nós são adicionados ao início.
	
	- Retorno da função: novo ponteiro do tipo LinkedList ao nó criado.
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

/*	************ALTERAÇÃO PROJETO FINAL************

	Remove um arco v-w contido na struct Edge 'e' do digrafo 'G'. Caso o digrafo não possuir o
	arco, a função se encerra e imprime uma mensagem de erro ao usuário. A função busca na LinkedList
	de adjacência do vértice 'v' pela aresta v-w e, ao encontrá-la, remove o nó designado e libe-
	ra a memória deste nó, além de decrementar a quantidade de arestas do grafo. Além disso, tam-
	bém é feita a verificação se o peso da aresta a ser removida está correto, caso contrário uma
	mensagem de erro é impressa. Dessa forma, é possível garantir a consistência da operação e a
	remoção das arestas corretas.
	
	Utilização da função:
	
	DIGRAPHremoveE(G, e);
	
	Seja G:
	||Vertex A||
			  -(1.0)->[Vertex B]
			  -(2.0)->[Vertex C]
	||Vertex B||
	||Vertex C||
	
	E Edge e:
	e = {0, 1, 1.0} (sendo 0 e 1 os índices de A e B encontrados utilizando VERTEXreturn(G, nome))
	
	Após chamar a função teremos:
	||Vertex A||
				-(2.0)->[Vertex C]
	||Vertex B||
	||Vertex C||
	
	- Paramêtros da função: Digrafo do tipo Digraph e aresta do tipo Edge.
	
	-Retorno da função: void.
*/
void DIGRAPHremoveE(Digraph G, Edge e)
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
			if((e.id == current->id) || (e.id == -1)) /*Exceção -1 criada para DIGRAPHremoveV.*/
			{ 
				prev->next = current->next;
				free(current);
				G->E--;
			} else 
			{
				printf("Arestra nao removida. ID incorreto.\n");
			} /*if*/
		} else 
		{
			G->array[v]->adj = current->next;		/*É o primeiro da LinkedList, então passa o seguinte à cabeça da LinkedList e desloca o nó.*/
			free(current);
			G->E--;
		} /*if*/
	} else 
	{
		printf("Aresta %d->%d nao encontrada. ", G->array[v]->id, G->array[w]->id);
		printf("Nao foi possivel remove-la.\n");
	} /*if*/
}


/*	************ALTERAÇÃO PROJETO FINAL************

	Como nesta implementação foi criado um array de vértices em vez de uma LinkedList encadeada, algumas
	limitações são encontradas ao remover um elemento deste array. Então, para isso, é necessaria a
	realocação de memória do array, além do rearranjo dos elementos de acordo com elemento que está
	sendo removido. Primeiramente, encontra-se o vértice a ser removido, desaloca toda sua LinkedList de
	adjacência e remove todas as outras arestas existentes no grafo que apontem para esse vértice (
	caso não seja um vértice de origem), move o vértice a ser removido para o fim do vetor, realoca
	o array de vértices para uma unidade menor do que a atual e desaloca o vetor removido. Além dis-
	so, caso o vértice removido não for o último vértice do array, também é necessário mudar a refe-
	rência dos vértices em todas as LinkedLists de adjacência, já que terão diminuido em 1 unidade.
	
	Utilização da função:
	
	DIGRAPHremoveV(G, v);
	
	Caso G for:
	
	[Vertex A]
	[Vertex B]
	[Vertex C]
	
	e v = 2 (índice de C encontrado com VERTEXreturn(G, "C")), teremos:
	
	[Vertex A]
	[Vertex B]
	
	- Paramêtros da função: Digrafo do tipo Digraph e string do nome do vértice a ser removido.
	
	- Retorno da função: void.	
*/
void DIGRAPHremoveV(Digraph G, int id)
{
	Vertex v = VERTEXreturn(G, id);

	if (v == -1)
	{
		printf("ID inexistente. Nenhuma tarefa removida.\n");
		return;
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

	/*Verificar LinkedList de adjacência (vértices que dependem deste).*/
	while (l != NULL) 
	{
		w = l->w;
		/*Encontra a posição de ID no array de IDs do vértice w.*/
		i = FINDreqs_id(G->array[w]->reqs_id, G->array[w]->reqs, G->array[v]->id);
		for(j = i; j < G->array[w]->reqs-1; j++) /*Remoção da ID do array de IDs*/
		{		
			G->array[w]->reqs_id[j] = G->array[w]->reqs_id[j+1];
		} /*for*/
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
	assert(G->array);

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
						l->w--; /*Se o índice dessa LinkedList for maior ou igual ao índice do vértice removido, atualize-o.*/
					}
				} /*for*/
			} /*if*/
			TIME(G, i);	 /*Recalcula o tempo da tarefa em i*/
		} /*for*/
	} /*if*/
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Cria e retorna uma aresta (edge) do tipo Edge dos vértices v-w contendo um peso (weight). Esta fun-
	ção simplesmenet inicializa uma estrutura do tipo Edge contendo os valores de aresta que serão pas-
	sados a outra função que as utilizará de acordo com o que for implementado.
	
	Utilização da função:
	
	e = EDGE(v, w, weight);
	
	Sendo v e w do tipo Vertex e weight float. Isto cria a aresta e = {v, w, weight}.
	
	- Paramêtros da função: dois vértices do tipo Vertex e o peso do tipo float dessa aresta v-w.
	
	- Retorno da função: aresta do tipo Edge.
*/
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
		e.v 	= -1;
		e.id 	= -1;
	} /*if*/
	return e;
}

/*
	Esta função faz a verificação da existência de uma aresta checando a LinkedList de adjacência de um
	vértice do digrafo G.
	
	Utilização da função:
	
	bool checa = DIGRAPHadj(G, v, w);
	
	- Paramêtros da função: digrafo do tipo Digraph e dois vértices do tipo Vertex.
	
	- Retorno da função: true (1) caso a aresta exista, false (0) caso contrário.
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

/*	************ALTERAÇÃO PROJETO FINAL************

	Esta função desaloca o digrafo G entrado, assim como cada vértice contido em seu array de vértices,
	além de cada um dos nós das LinkedLists de adjacências de cada um destes vértices. Isso previne o vaza-
	mento de memória e a correta devolução da memória ao sistema.
	
	Utilização da função:
	
	DIGRAPHdestroy(G);
	
	Desloca toda memória alocada ao digrafo G.
	
	- Paramêtros da função: Digrafo do tipo Digraph.
	
	- Retorno da função: void.
*/
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
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Função de impressão do digrafo explicitado por seus vetores e lístas de adjacência. A impressão é feita
	no formato:
	
	||Vertex A||:
			--(5.0)-->[Vertex G]
			--(10.0)-->[Vertex F]
	||Vertex B||:
			...
	
	Utilização da função:
	
	DIGRAPHshow(G);
	
	Resultados obtidos como mostrado acima.
	
	- Paramêtros da função: digrafo do tipo Digraph.
	- Retorno da função: void.
*/
void DIGRAPHshow(Digraph G) 
{
	LinkedList l;
	Vertex v;
	int i;
	
	printf("Grafo:\tV:%d\tE:%d\n\n", G->V, G->E);
	printf("ID\tNOME\t\tEXEC\tDURACAO\tINICIO_MIN\tPRE_REQS\tDEPS\n");
	for (v = 0; v < G->V; v++) 
	{
		printf("%d\t%s\t\t%d\t%d\t%d\t\t%d\t", 
				G->array[v]->id, G->array[v]->name, G->array[v]->exec, G->array[v]->duration, G->array[v]->min_start, G->array[v]->reqs);
		if (G->array[v]->reqs > 0) 
		{
			for (i = 0; i < G->array[v]->reqs; i++) 
			{
				printf("\t%d", G->array[v]->reqs_id[i]);
			} /*for*/
		} else 
		{
			printf("\t-");
		} /*if*/
		printf("\n");
		if (G->array[v]->adj != NULL) 
		{
			printf("Adj:");
			for (l = G->array[v]->adj; l != NULL; l = l->next) 
			{
				printf(" %d", l->id);
			} /*for*/
			printf("\n");
		} /*if*/
		printf("Time: %d\n", G->array[v]->time);
		printf("\n");
	} /*for*/
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Esta função auxiliar apenas faz uma busca no array de vértices do digrafo G pelo índice do vetor contendo
	o nome passado à função. Ela imprime um erro e retorna -1 caso o vértice não exista no digrafo.
	
	Utilização da função:
	
	Vertex i = VERTEXreturn(G, "A");
	
	Retornaria i = 0 de acordo com os exemplos anteriores.
	
	- Paramêtros da função: Digrafo do tipo Digraph e string contendo o nome do vértice.
	
	- Retorno da função: Vertex contendo o índice do vértice com nome "vertexName".
*/
Vertex VERTEXreturn(Digraph G, int id)
{
	Vertex v = 0;
	
	if(G->V == 0){
		return -1;
	}

	while (G->array[v]->id != id && v < G->V - 1) 
	{
		v++;
	} /*while*/
	if (G->array[v]->id == id) 
	{
		return v;
	} else
	{
		/*printf("Vertice nao encontrado. Error code: -1.\n");*/
		return -1;
	} /*if*/
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Esta função imprime em um arquivo "output.txt" o grafo no mesmo padrão do arquivo de entrada utilizado em
	DIGRAPHinit. Este arquivo pode ser utilizado para a inicialização de outro digrafo normalmente, já que se-
	gue o padrão conforme dito.
	
	Utilização da função:
	
	DIGRAPHsave(G);
	
	Gerará um arquivo "output.txt" na pasta src do projeto.
	
	- Paramêtros da função: Digrafo do tipo Digraph.
	
	- Retorno da função: void.
*/
void DIGRAPHsave(Digraph G) 
{
	FILE* fp = fopen("output.txt", "w");
	int i, j, id, exec, duration, min_start, reqs;
	char name[100];
	
	for (i = 0; i < G->V; i++) 
	{
		id 			= G->array[i]->id;
		strcpy(name, G->array[i]->name);
		exec 		= G->array[i]->exec;
		duration 	= G->array[i]->duration;
		min_start 	= G->array[i]->min_start;
		reqs 		= G->array[i]->reqs;

		fprintf(fp, "%d '%s' %d %d %d %d", 
					id, name, exec, duration, min_start, reqs);
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
}


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