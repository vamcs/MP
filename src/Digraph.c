#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

//Definição do tipo das variáveis de endereçamento dos vértices (int).
#define Vertex int

//Definição do tipo booleano utilizado para comparações.
// #define bool int
// #define true 1
// #define false 0

#define INFINITO 1000000000000.0f	//Valor virtualmente infinito utilizado no algoritmo de Dijkstra.
#define maxV 20						//Valor máximo da franja da arborecência em SPT.

/*	************ALTERAÇÃO PROJETO FINAL************

	** DIGRAPHinit ** (Inicialização do Digrafo)
	Constrói e retorna um digrafo com vértices 0 ... V-1 e todas arestas definidas no arquivo txt
	passado à função. Primeiramente, a função lê o arquivo, o interpreta e gera os vértices, após
	são lidos os vértices de origem e, por fim, todas as arestas definidas, caso existam. É neces-
	sário que existam no arquivo de origem a definição dos vértices e pelo menos um vértice de ti-
	po origem.
	
	O digrafo definido aqui é composto de um array de listas de adjacência do tipo VertexArray e 
	em cada um de seus nós foi definida uma lista de adjacência do tipo link. Além disso, é veri-
	ficado caso o arquivo exista, se não será requisitado do usuário a entrada de um arquivo vá-
	lido.
	
	Atenção, arquivo de entrada "file.txt" deve seguir o padrão:
	
	A, B, C, D 		//Vértices
	A, B 			//Vértices de origem
	A, C, 5.0		//Pode não haver definição de arestas
	B, D, 2.0
	C, D, 1.0
	
	Utilização da função:
	
	DIGRAPHinit("file.txt");
	Sendo que file.txt contenha a definição de um digrafo de acordo com a especificação acima.
	
	- Parâmetros da função: string contendo o nome do arquivo de definições do grafo.
	
	- Retorno da função: digrafo do tipo Digraph contendo todas as definições lidas no arquivo txt.
*/
Digraph DIGRAPHinit(char* file) {
	Vertex v = 0, w;
	int V = 1, i, dep;
	//char string[100], origem[100], dest[100];
	char aux;
	//float weight;
	
	//Declaração e inicialização do digrafo.
	Digraph G = (Digraph)malloc(sizeof *G);
	
	FILE* fp = fopen(file, "r");
	while (fp == NULL) {	//Exceção: arquivo inexistente. Usuário é requisitado a digitar o nome correto do arquivo.
		printf("Arquivo invalido. Digite o nome correto do arquivo.\n");
		scanf("%30s", file);
		fp = fopen(file, "r");
	}
	
	/********************Leitura do Grafo********************/

	/*Contagem do número de linhas do arquivo de entrada.*/
	while(!feof(fp)) {
		aux = fgetc(fp);
		if(aux == '\n') {
			V++;
		}
	}	
	
	rewind(fp);		//Seta o cursor de leitura de volta ao início do arquivo.
	
	//Alocação de memória do vetor de listas adjacentes.
	//As listas de adjacência são inicializadas como nulas.
	G->array = (VertexArray*)malloc(V * sizeof(VertexArray));
	for (v = 0; v < V; v++) {
		G->array[v] = (struct vertexArray*)malloc(sizeof(struct vertexArray));
		G->array[v]->adj = NULL;
		G->array[v]->id = -1;
	}
	G->V = V;
	G->E = 0;

	//Leitura das Tarefas
	v = 0;
	do {
		fscanf(fp, "%d %*c %100[^']'", &G->array[v]->id, G->array[v]->name);
		fscanf(fp, "%d %d %d %d", &G->array[v]->exec, &G->array[v]->duration, &G->array[v]->min_start, &G->array[v]->reqs);

		if (G->array[v]->reqs > 0) {
			G->array[v]->reqs_id = (int*)malloc(G->array[v]->reqs * sizeof(int));
			for (i = 0; i < G->array[v]->reqs; i++) {
				fscanf(fp, "%d", &dep);
				w = VERTEXreturn(G, dep);
				while(w == -1) {				//Garante que as dependências só podem ser de tarefas acima da atual.
					printf("ID inexistente. Corrija o arquivo ou entre outro valor: ");	
					scanf("%d", &dep);
					w = VERTEXreturn(G, dep);
				}

				DIGRAPHinsertE(G, EDGE(w, v, G->array[v]->id));
				G->array[v]->reqs_id[i] = dep;
			}
		}

		else{
			G->array[v]->reqs_id = NULL;
		}

		TIME(G, v);		//Calcula o tempo necessário para executar a tarefa lida contida em G->array[v].

		v++;
	}
	while(!feof(fp) && v < G->V);
	
	fclose(fp);
	return G;
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Insere um arco v-w contido na struct Edge 'e' no digrafo 'G'. Se o digrafo já possui v-w, a função 
	imprime uma mensagem de erro. Caso a aresta contida em Edge "e" já exista no digrafo, uma men-
	sagem de erro será impressa, já que a criação de uma aresta redundante é uma possível forma de
	vazamento de memória durante a execução do programa.
	
	Utilização da função:
	
	DIGRAPHinsertE(G, e);
	
	'G' pode ser inicializado utilizando DIGRAPHinit(G) e 'e' utilizando a função e = EDGE(v, w, weight).
	
	- Paramêtros da função: ponteiro de digrafo do tipo Digrafo e estrutura contendo informações de uma aresta
	do tipo Edge.
	
	- Retorno da função: void.
*/
void DIGRAPHinsertE(Digraph G, Edge e) {
	//Leitura de Edge 'e'.
	Vertex v = e.v, w = e.w;
	int id = e.id;
	if (!DIGRAPHadj(G, v, w)) {	//Verifica se a aresta já existe.
		//Cria um novo node e o insere no início da lista de adjacência.
		G->array[v]->adj = NEWnode(w, id, G->array[v]->adj);
		G->E++;	//Incremento número de arestas adicionadas no grafo.
	}
	else{
		printf("Aresta ja existente no digrafo ou vertice de destino eh origem.\n%s->%s nao foi inserida.\n", G->array[v]->name, G->array[w]->name);
	}
	return;
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Esta função cria um novo nó da lista de adjacências ao receber as informações necessárias
	contidas em cada um dos nós.
	
	Utilização da função:
	
	link l = NEWnode(w, weight, l);
	
	A função então cria um nó e o aponta para 'l':
		new_node --> l
	
	- Parâmetros da função: vértice do tipo Vertex para o qual o vértice que possui esta lista
	de adjacências irá apontar, weight do tipo float que contém o peso da aresta criada e nó de
	origem da lista de adjacência, já que novos nós são adicionados ao início.
	
	- Retorno da função: novo ponteiro do tipo link ao nó criado.
*/
link NEWnode(Vertex w, int id, link next) {
	link a;
	a = (link)malloc(sizeof (struct node));
	a->w = w;
	a->id = id;
	a->next = next;
	return a;
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Remove um arco v-w contido na struct Edge 'e' do digrafo 'G'. Caso o digrafo não possuir o
	arco, a função se encerra e imprime uma mensagem de erro ao usuário. A função busca na lista
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
void DIGRAPHremoveE(Digraph G, Edge e) {
	Vertex v = e.v, w = e.w;
	link current, prev;
	if(DIGRAPHadj(G, v, w)) {
		for (current = G->array[v]->adj; current != NULL && current->w != w; current = current->next);	//Encontra o nó a ser removido
		if(G->array[v]->adj != current) {																//Caso este nó não seja o primeiro da lista
			for (prev = G->array[v]->adj; prev != NULL && prev->next != current; prev = prev->next);	//Procura pelo nó imediatamente anterior a esse
			if(e.id == current->id || e.id == -1) { //Exceção -1 criada para DIGRAPHremoveV.
				prev->next = current->next;
				free(current);
				G->E--;
			}
			else{
				printf("Arestra nao removida. ID incorreto.\n");
			}
		}
		else{
			G->array[v]->adj = current->next;		//É o primeiro da lista, então passa o seguinte à cabeça da lista e desloca o nó.
			free(current);
			G->E--;
		}
	}
	else{
		printf("Aresta %d->%d nao encontrada. ", G->array[v]->id, G->array[w]->id);
		printf("Nao foi possivel remove-la.\n");
	}
}

/*	************ALTERAÇÃO PROJETO FINAL************

	Esta função insere um novo vértice no digrafo. Como o array de vértices não foi feito utilizando
	uma lista encadeada, é necessário realocar a sua memória de forma a caber mais uma entrada no vetor.
	Assim, primeiramente é feita a verificação se o vértice inserido já existe no array, caso seja ver-
	dade uma mensagem de erro é impressa. Caso contrário, o número de vértices no grafo é incrementado,
	a memória do array de vértices é realocada e um novo vértice é inicializado ao final do array.
	
	Utilização da função:
	
	DIGRAPHinsertV(G, nome);
	
	Caso G for
	
	[Vertex A]
	[Vertex B]
	[Vertex C]
	
	e nome = 'D', após chamar a função teremos:
	
	[Vertex A]
	[Vertex B]
	[Vertex C]
	[Vertex D]
	
	- Paramêtros da função: Digrafo do tipo Digraph e uma string que contém o nome do novo vértice.
	
	- Retorno da função: void.
	
*/
void DIGRAPHinsertV(Digraph G) {
	Vertex w;
	int id, i;
	char exec;

	printf("Entre com o ID da tarefa:\n");
	scanf("%d", &id);

	while (VERTEXreturn(G, id) != -1 || VERTEXreturn(G, id) < -2) {
		printf("ID invalida ou existente. Entre com outro valor: ");
		scanf("%d", &id);
	}

	G->V++;
	G->array = (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));
	G->array[G->V-1] = (struct vertexArray*)malloc(sizeof(struct vertexArray));
	
	G->array[G->V-1]->id = id;

	printf("Entre com o nome da tarefa (ate 100 caracteres):\n");
	scanf("%100s", G->array[G->V-1]->name);

	for (w = 0; w < G->V-1; w++) {
		if (strcmp(G->array[G->V-1]->name, G->array[w]->name) == 0) {
			printf("Nome ja existente. Entre com outra string (ate 100 caracteres: ");
			scanf("%100s", G->array[G->V-1]->name);
			w = -1;
		}
	}

	printf("Tarefa ja executada? s/n\n");
	scanf(" %c", &exec);
	switch(exec) {
		case 'S':
		case 's':
			G->array[G->V-1]->exec = true;
			break;
		case 'N':
		case 'n':
			G->array[G->V-1]->exec = false;
			break;
		default:
			G->array[G->V-1]->exec = false;
			break;
	}

	printf("Entre com a duracao da tarefa:\n");
	scanf("%d", &G->array[G->V-1]->duration);

	printf("Entre com o ciclo minimo de inicio da tarefa:\n");
	scanf("%d", &G->array[G->V-1]->min_start);

	printf("Esta tarefa possui quantos requisitos?\n");
	scanf("%d", &G->array[G->V-1]->reqs);

	while (G->array[G->V-1]->reqs > G->V-1) {
		printf("Numero excede o maximo de requisitos possiveis. Entre outro valor: ");
		scanf("%d", &G->array[G->V-1]->reqs);
	}

	if(G->array[G->V-1]->reqs > 0) {
		G->array[G->V-1]->reqs_id = (int*)malloc(G->array[G->V-1]->reqs * sizeof(int));
		for(i = 0; i < G->array[G->V-1]->reqs; i++) {
			printf("Entre com o ID do pre-requisito %d: ", i+1);
			scanf("%d", &id);
			w = VERTEXreturn(G, id);
			while(w == -1) {
				printf("ID inexistente. Entre outro valor: ");
				scanf("%d", &id);
				w = VERTEXreturn(G, id);
			}
			DIGRAPHinsertE(G, EDGE(w, G->V-1, G->array[G->V-1]->id));
			G->array[G->V-1]->reqs_id[i] = id;
		}
	}

	else {
		G->array[G->V-1]->reqs_id = NULL;
	}

	G->array[G->V-1]->adj = NULL;

	TIME(G, G->V-1);
}


/*	************ALTERAÇÃO PROJETO FINAL************

	Como nesta implementação foi criado um array de vértices em vez de uma lista encadeada, algumas
	limitações são encontradas ao remover um elemento deste array. Então, para isso, é necessaria a
	realocação de memória do array, além do rearranjo dos elementos de acordo com elemento que está
	sendo removido. Primeiramente, encontra-se o vértice a ser removido, desaloca toda sua lista de
	adjacência e remove todas as outras arestas existentes no grafo que apontem para esse vértice (
	caso não seja um vértice de origem), move o vértice a ser removido para o fim do vetor, realoca
	o array de vértices para uma unidade menor do que a atual e desaloca o vetor removido. Além dis-
	so, caso o vértice removido não for o último vértice do array, também é necessário mudar a refe-
	rência dos vértices em todas as listas de adjacência, já que terão diminuido em 1 unidade.
	
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
void DIGRAPHremoveV(Digraph G, int id) {
	Vertex v = VERTEXreturn(G, id);

	if (v == -1) {
		return;
	}
	
	Vertex w;
	int i , j;
	link temp = G->array[v]->adj;
	link l = G->array[v]->adj;
	VertexArray temp_array = G->array[v];

	//Remover referências ao vértice
	//Verificar dependências
	if (G->array[v]->reqs > 0) {
		for (i = 0; i < G->array[v]->reqs; i++) {
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);
			DIGRAPHremoveE(G, EDGE(w, v, id));
		}
		free(G->array[v]->reqs_id);
	}

	//Verificar lista de adjacência (vértices que dependem deste)
	while (l != NULL) {
		w = l->w;
		i = FINDreqs_id(G->array[w]->reqs_id, G->array[w]->reqs, G->array[v]->id);
		//for (i = 0; i < G->array[w]->reqs && G->array[v]->id != G->array[w]->reqs_id[i]; i++);	//Encontra a posição de ID no array de IDs do vértice w.
		for(j = i; j < G->array[w]->reqs-1; j++) {		//Remoção da ID do array de IDs
			G->array[w]->reqs_id[j] = G->array[w]->reqs_id[j+1];
		}
		G->array[w]->reqs--;
		G->array[w]->reqs_id = (int*)realloc(G->array[w]->reqs_id, G->array[w]->reqs * sizeof(int));
		temp = l;
		l = l->next;
		free(temp);
		G->E--;
	}

	//Remover lista de adjacência
	// while (l != NULL) {
	// 	temp = l;
	// 	l = l->next;
	// 	free(temp);
	// 	G->E--;
	// };
	
	//Remover arestas que apontam ao vértice v.
	// if(!G->array[v]->source) {
	// 	for(i = 0; i < G->V; i++) {
	// 		if(i != v) DIGRAPHremoveE(G, EDGE(i, v, -1));
	// 	}
	// }
	
	//Rearranjo dos vértices no vetor para exclusão do vértice desejado.
	for(i = v; i < G->V-1; i++) {
		G->array[i] = G->array[i+1];
	}
	G->V--;
	free(temp_array);
	G->array = (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));

	//Caso o vértice removido não tiver sido o último do vetor, é necessário corrigir os índices
	//dos nós das listas de adjacência.
	if(v < G->V+1) {
		for(i = 0; i < G->V; i++) {										//Percorre o array de vértices.
			if(G->array[i]->adj != NULL) {								//Verifica se o vértice possui lista de adjcência.
				for(l = G->array[i]->adj; l != NULL; l = l->next) {		//Percorre a lista de adjacência do vértice.
					if(l->w >= v) l->w--;								//Se o índice dessa lista for maior ou igual ao índice do vértice removido, atualize-o.
				}
			}
			TIME(G, i);													//Recalcula o tempo da tarefa em i
		}
	}
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
Edge EDGE(Vertex v, Vertex w, int id) {
	Edge e;
	if(v >= 0 && w >= 0 && id >= -1) {
		e.v = v;
		e.w = w;
		e.id = id;
	}
	else{
		printf("Aresta invalida. Valores menores do que zero inseridos.\n");
		printf("Error code: -1.\n");
		e.v = -1;
		e.v = -1;
		e.id = -1;
	}
	return e;
}

/*
	Esta função faz a verificação da existência de uma aresta checando a lista de adjacência de um
	vértice do digrafo G.
	
	Utilização da função:
	
	bool checa = DIGRAPHadj(G, v, w);
	
	- Paramêtros da função: digrafo do tipo Digraph e dois vértices do tipo Vertex.
	
	- Retorno da função: true (1) caso a aresta exista, false (0) caso contrário.
*/
bool DIGRAPHadj(Digraph G, Vertex v, Vertex w) {
	link x;
	for (x = G->array[v]->adj; x != NULL; x = x->next)
		if(x->w == w)
			return true;
	return false;
}

/*	************ALTERAÇÃO PROJETO FINAL************
	*****************NÃO UTILIZADA*****************

	Função auxiliar que remove a vírgula das strings lidas do arquivo.
	
	Utilização da função:
	
	removeComma(teste);
	
	Sendo teste = "teste,", a função modifica a string para "teste".
	
	- Paramêtros da função: vetor de caracteres (string).
	
	- Retorno da função: void.
*/
// void removeComma(char* string) {
// 	int i;
// 	for (i = 0; string[i] != '\0' && i < 100; i++) {
// 		if(string[i] == ',') string[i] = '\0';		//Remove a vírgula substituindo por fim de string.
// 	}
// }

/*	************ALTERAÇÃO PROJETO FINAL************

	Esta função desaloca o digrafo G entrado, assim como cada vértice contido em seu array de vértices,
	além de cada um dos nós das listas de adjacências de cada um destes vértices. Isso previne o vaza-
	mento de memória e a correta devolução da memória ao sistema.
	
	Utilização da função:
	
	DIGRAPHdestro(G);
	
	Desloca toda memória alocada ao digrafo G.
	
	- Paramêtros da função: Digrafo do tipo Digraph.
	
	- Retorno da função: void.
*/
void DIGRAPHdestroy(Digraph G) {
	int v;
	link temp, l;
	
	//Desalocar listas encadeadas
	for (v = 0; v < G->V; v++) {
		l = G->array[v]->adj; 
		while (l != NULL) {
			temp = l;
			l = l->next;
			free(temp);
		};
		l = NULL;
	}

	//Desalocar vetor de IDs
	for (v = 0; v < G->V; v++) {
		if (G->array[v]->reqs > 0) {
			free(G->array[v]->reqs_id);
		}
	}
	
	//Desalocar array de listas
	for (v = 0; v < G->V; v++) {
		free(G->array[v]);
	}
	free(G->array);
	G->array = NULL;
	
	//Desalocar grafo
	free(G);
	G = NULL;
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
void DIGRAPHshow(Digraph G) {
	link l;
	Vertex v;
	int i;
	
	printf("Grafo:\tV:%d\tE:%d\n\n", G->V, G->E);
	printf("ID\tNOME\t\tEXEC\tDURACAO\tINICIO_MIN\tPRE_REQS\tDEPS\n");
	for (v = 0; v < G->V; v++) {
		printf("%d\t%s\t\t%d\t%d\t%d\t\t%d\t", 
				G->array[v]->id, G->array[v]->name, G->array[v]->exec, G->array[v]->duration, G->array[v]->min_start, G->array[v]->reqs);
		if (G->array[v]->reqs > 0) {
			for(i = 0; i < G->array[v]->reqs; i++) printf("\t%d", G->array[v]->reqs_id[i]);
		}

		else {
			printf("\t-");
		}
		printf("\n");
		if (G->array[v]->adj != NULL) {
			printf("Adj:");
			for (l = G->array[v]->adj; l != NULL; l = l->next) {
				printf(" %d", l->id);
			}
			printf("\n");
		}
		printf("Time: %d\n", G->array[v]->time);
		printf("\n");
	}
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
Vertex VERTEXreturn(Digraph G, int id) {
	Vertex v = 0;
	while(G->array[v]->id != id && v < G->V-1) v++;
	if(G->array[v]->id == id) return v;
	else{
		//printf("Vertice nao encontrado. Error code: -1.\n");
		return -1;
	}
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
void DIGRAPHsave(Digraph G) {
	FILE* fp = fopen("output.txt", "w");
	int i, j;
	int id, exec, duration, min_start, reqs;
	char name[100];
	
	for (i = 0; i < G->V; i++) {
		id = G->array[i]->id;
		strcpy(name, G->array[i]->name);
		exec = G->array[i]->exec;
		duration = G->array[i]->duration;
		min_start = G->array[i]->min_start;
		reqs = G->array[i]->reqs;

		fprintf(fp, "%d '%s' %d %d %d %d", 
					id, name, exec, duration, min_start, reqs);
		if (reqs > 0) {
			for (j = 0; j < reqs; j++) {
				fprintf(fp, " %d", G->array[i]->reqs_id[j]);
			}
		}
		if (i < G->V-1) fprintf(fp, "\n");
	}
	
	fclose(fp);
}


int FINDreqs_id(int* reqs_id, int reqs, int id) {
	int i;

	for (i = 0; i < reqs && id != reqs_id[i]; i++);
	if(i < reqs)
		return i;
	else
		return -1;
}