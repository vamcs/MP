#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Digraph.h"

//Definição do tipo das variáveis de endereçamento dos vértices (int).
#define Vertex int

//Definição do tipo booleano utilizado para comparações.
//#define bool int
//#define true 1
//#define false 0

#define INFINITO 1000000000000.0f	//Valor virtualmente infinito utilizado no algoritmo de Dijkstra.
#define maxV 20						//Valor máximo da franja da arborecência em SPT.

/*
	** DIGRAPHinit ** (Inicialização do Digrafo)
	Constrói e retorna um digrafo com vértices 0 ... V-1 e todas arestas definidas no arquivo txt
	passado à função. Primeiramente, a função lê o arquivo, o interpreta e gera os vértices, após
	são lidos os vértices de origem e, por fim, todas as arestas definidas, caso existam. É neces-
	sário que existam no arquivo de origem a definição dos vértices e pelo menos um vértice de ti-
	po origem.
	
	O digrafo definido aqui é composto de um array de listas de adjacência do tipo VertexArray e 
	em cada um de seus nós foi definida uma lista de adjacência do tipo lista. Além disso, é veri-
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
Digraph DIGRAPHinit(char* file){
	Vertex v, w;
	int V = 0;
	char string[100], origem[100], dest[100], aux;
	float weight;
	
	//Declaração e inicialização do digrafo.
	Digraph G = (Digraph)malloc(sizeof *G);
	
	FILE* fp = fopen(file, "r");
	while (fp == NULL){	//Exceção: arquivo inexistente. Usuário é requisitado a digitar o nome correto do arquivo.
		printf("Arquivo invalido. Digite o nome correto do arquivo.\n");
		scanf("%s", file);
		fp = fopen(file, "r");
	}
	
	/********************Leitura do Grafo********************/
	
	//Contagem dos Vértices.
	//Leitura da primeira linha do arquivo para definir G->V.
	do{
		fscanf(fp, "%s", string);
		//Não é necessário tratar a string agora, já que apenas uma contagem é feita.
		V++;
		fscanf(fp, "%c", &aux);	//Lê espaços ('\b') e '\n'.
	}
	while(aux != '\n');	
	
	rewind(fp);		//Seta o cursor de leitura de volta ao início do arquivo.
	
	//Alocação de memória do vetor de listas adjacentes.
	//As listas de adjacência são inicializadas como nulas.
	G->array = (VertexArray*)malloc(V * sizeof(VertexArray));
	for(v = 0; v < V; v++){
		G->array[v] = (VertexArray)malloc(sizeof(struct vertexArray));
		G->array[v]->adj = NULL;
	}
	G->V = V;
	G->E = 0;
	
	//Leitura dos Vértices
	for(v = 0; v < V; v++){					//A leitura é feita apenas V vezes sem necessidade da variável auxiliar.
		fscanf(fp, "%s", string);			//Lê o nome do vértice.
		removeComma(string);				//Tratamento da string lida.
		strcpy(G->array[v]->name, string);	//Seta o nome do vértice.
		G->array[v]->source = false;		//Inicializa os vértices como não sendo origem.
	}
	
	//Leitura dos Vértices de Origem
	do{
		fscanf(fp, "%s", string);		//Leitura do nome do vértice.
		removeComma(string);			//Remoção da vírgula da string.
		v = VERTEXreturn(G, string);	//Busca pelo vértice que contém a string desejada.
		if(strcmp(G->array[v]->name, string) == 0) 
			G->array[v]->source = true;	//Se encontrá-lo, diz que é origem.
		fscanf(fp, "%c", &aux);			//Lê '\b' e '\n'.
	}
	while(aux != '\n');
	
	//Leitura das Arestas (Edges)
	while (!feof(fp)){					//Caso já tenha atingindo o fim do arquivo, não há arestas.
		fscanf(fp, "%s", origem);		//Lê o nó de origem da aresta
		removeComma(origem);			//Remove a vírgula
		
		fscanf(fp, "%s", dest);			//Lê o nó de destino da aresta
		removeComma(dest);				//Remove a vírgula
		
		fscanf(fp, "%f", &weight);		//Lê o peso da aresta.
		
		v = VERTEXreturn(G, origem);	//Encontra a posição do nó de origem no array de listas.
		w = VERTEXreturn(G, dest);		//Encontra a posição do nó de destino no array de listas.

		if(G->array[v]->adj != NULL){	//Se já existir um lista de adjacência no nó, utiliza-se a função DIGRAPHinsertE para inserir a aresta.
			DIGRAPHinsertE(G, EDGE(v, w, weight));
		}
		else if(strcmp(G->array[v]->name, origem) == 0 && strcmp(G->array[w]->name, dest) == 0 && !G->array[w]->source){
			//Insere a aresta definida se os nomes confirmarem e se o vértice de destino não seja origem.
			G->array[v]->adj = NEWnode(w, weight, G->array[v]->adj);	//Insere a aresta definida.
			G->E++;		//Método InsertE não foi usado porque este verifica por arestas existentes, as quais ainda não existem.
		}
		else{	//Impressão da mensagem de erro. Não é possíve adicionar uma aresta com destino em um vértice de origem.
			printf("Aresta %s -> %s invalida.\n%s eh vertice de origem.\n\n", origem, dest, dest);
		}
	};
	
	fclose(fp);
	return G;
}

/*
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
void DIGRAPHinsertE(Digraph G, Edge e){
	//Leitura de Edge 'e'.
	Vertex v = e.v, w = e.w;
	float weight = e.weight;
	if (!DIGRAPHadj(G, v, w) && !G->array[w]->source){	//Verifica se a aresta já existe.
		//Cria um novo node e o insere no início da lista de adjacência.
		G->array[v]->adj = NEWnode(w, weight, G->array[v]->adj);
		G->E++;	//Incremento número de arestas adicionadas no grafo.
	}
	else{
		printf("Aresta ja existente no digrafo ou vertice de destino eh origem.\n%s->%s nao foi inserida.\n", G->array[v]->name, G->array[w]->name);
	}
	return;
}

/*
	Esta função cria um novo nó da lista de adjacências ao receber as informações necessárias
	contidas em cada um dos nós.
	
	Utilização da função:
	
	lista l = NEWnode(w, weight, l);
	
	A função então cria um nó e o aponta para 'l':
		new_node --> l
	
	- Parâmetros da função: vértice do tipo Vertex para o qual o vértice que possui esta lista
	de adjacências irá apontar, weight do tipo float que contém o peso da aresta criada e nó de
	origem da lista de adjacência, já que novos nós são adicionados ao início.
	
	- Retorno da função: novo ponteiro do tipo lista ao nó criado.
*/
lista NEWnode(Vertex w, float weight, lista next){
	lista a;
	a = (lista)malloc(sizeof (struct node));
	a->w = w;
	a->weight = weight;
	a->next = next;
	return a;
}

/*
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
void DIGRAPHremoveE(Digraph G, Edge e){
	Vertex v = e.v, w = e.w;
	lista current, prev;
	if(DIGRAPHadj(G, v, w)){
		for (current = G->array[v]->adj; current != NULL && current->w != w; current = current->next);
		if(G->array[v]->adj != current){
			for (prev = G->array[v]->adj; prev != NULL && prev->next != current; prev = prev->next);
			if(e.weight == current->weight || e.weight == -1){ //Exceção -1 criada para DIGRAPHremoveV.
				prev->next = current->next;
				free(current);
				G->E--;
			}
			else{
				printf("Arestra nao removida. Peso incorreto.\n");
			}
		}
		else{
			G->array[v]->adj = current->next;
			free(current);
			G->E--;
		}
	}
	else{
		printf("Aresta %s->%s nao encontrada. ", G->array[v]->name, G->array[w]->name);
		printf("Nao foi possivel remove-la.\n");
	}
}

/*
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
void DIGRAPHinsertV(Digraph G, char* name){
	Vertex v;
	for(v = 0; v < G->V; v++){
		if(strcmp(G->array[v]->name, name) == 0){
			printf("Vertice ja existente. Insercao cancelada.\n");
			return;
		}
	}
	G->V++;
	G->array = (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));
	G->array[G->V-1] = (VertexArray)malloc(sizeof(struct vertexArray));
	
	strcpy(G->array[G->V-1]->name, name);
	G->array[G->V-1]->source = false;
	G->array[G->V-1]->adj = NULL;
}

/*
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
void DIGRAPHremoveV(Digraph G, char* name){
	Vertex v = VERTEXreturn(G, name);
	
	if(v == -1){
		return;
	}
	
	lista temp;
	lista l = G->array[v]->adj; 
	Vertex i;
	VertexArray temp_array = G->array[v];
	
	//Remover lista de adjacência
	while (l != NULL){
		temp = l;
		l = l->next;
		free(temp);
		G->E--;
	};
	
	//Remover arestas que apontam ao vértice v.
	if(!G->array[v]->source){
		for(i = 0; i < G->V; i++){
			if(i != v) DIGRAPHremoveE(G, EDGE(i, v, -1));
		}
	}
	
	//Rearranjo dos vértices no vetor para exclusão do vértice desejado.
	for(i = v; i < G->V-1; i++){
		G->array[i] = G->array[i+1];
	}
	G->V--;
	free(temp_array);
	G->array = (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));

	//Caso o vértice removido não tiver sido o último do vetor, é necessário corrigir os índices
	//dos nós das listas de adjacência.
	if(v < G->V+1){
		for(i = 0; i < G->V; i++){
			if(G->array[i]->adj != NULL) {
				for(l = G->array[i]->adj; l != NULL; l = l->next){
					l->w--;
				}
			}
		}
	}
}

/*
	Cria e retorna uma aresta (edge) do tipo Edge dos vértices v-w contendo um peso (weight). Esta fun-
	ção simplesmenet inicializa uma estrutura do tipo Edge contendo os valores de aresta que serão pas-
	sados a outra função que as utilizará de acordo com o que for implementado.
	
	Utilização da função:
	
	e = EDGE(v, w, weight);
	
	Sendo v e w do tipo Vertex e weight float. Isto cria a aresta e = {v, w, weight}.
	
	- Paramêtros da função: dois vértices do tipo Vertex e o peso do tipo float dessa aresta v-w.
	
	- Retorno da função: aresta do tipo Edge.
*/
Edge EDGE(Vertex v, Vertex w, float weight){
	Edge e;
	if(v >= 0 && w >= 0 && weight >= -1){
		e.v = v;
		e.w = w;
		e.weight = weight;
	}
	else{
		printf("Aresta invalida. Valores menores do que zero inseridos.\n");
		printf("Error code: -1.\n");
		e.v = -1;
		e.v = -1;
		e.weight = -1;
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
bool DIGRAPHadj(Digraph G, Vertex v, Vertex w){
	lista x;
	for (x = G->array[v]->adj; x != NULL; x = x->next)
		if(x->w == w)
			return true;
	return false;
}

/*
	Função auxiliar que remove a vírgula das strings lidas do arquivo.
	
	Utilização da função:
	
	removeComma(teste);
	
	Sendo teste = "teste,", a função modifica a string para "teste".
	
	- Paramêtros da função: vetor de caracteres (string).
	
	- Retorno da função: void.
*/
void removeComma(char* string){
	int i;
	for (i = 0; string[i] != '\0' && i < 100; i++){
		if(string[i] == ',') string[i] = '\0';		//Remove a vírgula substituindo por fim de string.
	}
}

/*
	Esta função desaloca o digrafo G entrado, assim como cada vértice contido em seu array de vértices,
	além de cada um dos nós das listas de adjacências de cada um destes vértices. Isso previne o vaza-
	mento de memória e a correta devolução da memória ao sistema.
	
	Utilização da função:
	
	DIGRAPHdestro(G);
	
	Desloca toda memória alocada ao digrafo G.
	
	- Paramêtros da função: Digrafo do tipo Digraph.
	
	- Retorno da função: void.
*/
void DIGRAPHdestroy(Digraph G){
	int v;
	lista temp, l;
	
	//Desalocar listas encadeadas
	for (v = 0; v < G->V; v++){
		l = G->array[v]->adj; 
		while (l != NULL){
			temp = l;
			l = l->next;
			free(temp);
		};
		l = NULL;
	}
	
	//Desalocar array de listas
	for (v = 0; v < G->V; v++){
		free(G->array[v]);
	}
	free(G->array);
	G->array = NULL;
	
	//Desalocar grafo
	free(G);
}

/*
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
void DIGRAPHshow(Digraph G){
	lista l;
	Vertex v;
	
	printf("Grafo:\tV:%d\tE:%d\n", G->V, G->E);
	for(v = 0; v < G->V; v++){
		printf("||Vertex %s||:\n", G->array[v]->name);
		for(l = G->array[v]->adj; l != NULL; l = l->next){
			printf("\t--(%.1f)-->[Vertex %s]\n", l->weight, G->array[l->w]->name);
		}
	}
}

/*
	Esta função auxiliar apenas faz uma busca no array de vértices do digrafo G pelo índice do vetor contendo
	o nome passado à função. Ela imprime um erro e retorna -1 caso o vértice não exista no digrafo.
	
	Utilização da função:
	
	Vertex i = VERTEXreturn(G, "A");
	
	Retornaria i = 0 de acordo com os exemplos anteriores.
	
	- Paramêtros da função: Digrafo do tipo Digraph e string contendo o nome do vértice.
	
	- Retorno da função: Vertex contendo o índice do vértice com nome "vertexName".
*/
Vertex VERTEXreturn(Digraph G, char* vertexName){
	Vertex v = 0;
	while(strcmp(G->array[v]->name, vertexName) != 0 && v < G->V-1) v++;
	if(strcmp(G->array[v]->name, vertexName) == 0) return v;
	else{
		printf("Vertice nao encontrado. Error code: -1.\n");
		return -1;
	}
}

/*
	Esta função imprime em um arquivo "output.txt" o grafo no mesmo padrão do arquivo de entrada utilizado em
	DIGRAPHinit. Este arquivo pode ser utilizado para a inicialização de outro digrafo normalmente, já que se-
	gue o padrão conforme dito.
	
	Utilização da função:
	
	DIGRAPHsave(G);
	
	Gerará um arquivo "output.txt" na pasta src do projeto.
	
	- Paramêtros da função: Digrafo do tipo Digraph.
	
	- Retorno da função: void.
*/
void DIGRAPHsave(Digraph G){
	FILE* p = fopen("output.txt", "w");
	lista aux;
	int i, count = 0, count2 = 0;
	
	for(i = 0; i < G->V; i++){
		fprintf(p, "%s", G->array[i]->name);
		if(i != G->V-1) fprintf(p, ", ");
		else fprintf(p, "\n");
	}
	
	for(i = 0; i < G->V; i++){
		if(G->array[i]->source) count++;
	}
	
	for(i = 0; i < G->V; i++){
		if(G->array[i]->source){
			fprintf(p, "%s", G->array[i]->name);
			count2++;
			if(count2 < count) fprintf(p, ", ");
		}
	}
	fprintf(p, "\n");
	
	for(i = 0; i < G->V; i++){
		aux = G->array[i]->adj;
		while(aux != NULL){
			fprintf(p, "%s, ", G->array[i]->name);
			fprintf(p, "%s, ", G->array[aux->w]->name);
			fprintf(p, "%.1f\n", aux->weight);
			aux = aux->next;
		}
	}
	
	fclose(p);
}

/*
	Esta função verifica se o grafo é conexo a partir dos vértices de origem. Ela utiliza o algoritmo de Dijkstra
	definido em SPT (Shortest Path Tree), a qual além de retornar a conectividade do digrafo, também retorna o menor
	caminho a partir de um nó a todos os outros, o que é utilizado em FindPath(Digraph).
	
	Utilização da função:
	
	bool checa = isConnected(G);
	
	- Paramêtros da função: Digrafo do tipo Digraph.
	
	- Retorno da função: true (1) se a partir das origens é possível chegar a qualquer outra aresta. False (0) caso
	contrário.
*/
bool isConnected(Digraph G){
	bool flag = true;
	int* parent = (int*)malloc(G->V * sizeof(int));
	float* dist = (float*)malloc(G->V * sizeof(float));
	Vertex i, v;
	for(i = 0; i < G->V && flag; i++){
		if(G->array[i]->source){
			SPT(G, i, parent, dist);
			for(v = 0; v < G->V; v++)
				if(parent[v] == -1) flag = false;
		}
	}
	free(parent);
	free(dist);
	return flag;
}

/*
	Esta função encontra o menor caminho entre dois vértices v-w. Caso não exista, -1 é retornado. Ela usa o algoritmo
	de Dijkstra para encontra a SPT (Shortest Path Tree) do digrafo a partir de um nó âncora. Os array parent e dist
	contêm, respectivamente, os vértices "pais" de um vértice "v" (parent[w] é o pai de w) e a distância de v até w 
	indexado por dist[w].
	
	Utilização da função:
	
	float dist = FindPath(G, v, w);
	
	Nota: v e w são encontrados utilizando a função VERTEXreturn(G, string).
	
	- Paramêtros da função: Digrafo do tipo Digraph e dois vértices do tipo Vertex.
	
	- Retorno da função: distância mínima entre v e w (float).
*/
float FindPath(Digraph G, char* origem, char* dest){
	Vertex v = VERTEXreturn(G, origem);
	Vertex w = VERTEXreturn(G, dest);
	int* parent = (int*)malloc(G->V * sizeof(int));
	float* dist = (float*)malloc(G->V * sizeof(float));
	float retorno;
	
	SPT(G, v, parent, dist);

	if(-1*(dist[w] - INFINITO) > 1) 
		retorno = dist[w];
	else 
		retorno = -1;
	free(parent);
	free(dist);
	return retorno;
}

/*
	As duas seguintes funções implementam o algorito de Dijkstra para a SPT (Shortest Path Tree) baseado no livro de Segdewick
	"Algorithms in C", seção "Dijkstra's Algorithm". Pequenas mudanças foram feitas para se adaptar ao digrafo atual.
	
	Em resumo, o algoritmo retorna a árvore de menor caminho a partir de um nó âncora, incluindo os nós parentes e a menor dis-
	tância deste nó até todos os outros. A função initialize inicializa os vetores parent, dist e frj para que o algoritmo seja
	executado corretamente. As funções FindPath e IsConnected utilizam este algoritmo para verificar a existência de um caminho
	entre dois vértices do digrafo e se o digrafo é conexo, respectivamente.
	
	Mais detalhes podem ser encontrados em http://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/dijkstra.html
*/
void SPT(Digraph G, Vertex s, Vertex parent[], float dist[]){
	
	Vertex v0, w, frj[maxV];
	lista a; float c;
	initialize( G, s, parent, dist, frj);

	while (1) {
		float mindist = INFINITO;
		for (w = 0; w < G->V; w++) 
			if (parent[w] == -1 && mindist > dist[w])
				mindist = dist[v0=w]; 
		if (-1*(mindist - INFINITO) < 1) break;	//Não é possível garantir mindist == INFINITO
		parent[v0] = frj[v0];
		for (a = G->array[v0]->adj; a != NULL; a = a->next) {
			w = a->w, c = a->weight;
			if (parent[w] == -1 && dist[w] > dist[v0] + c) { 
				dist[w] = dist[v0] + c; 
				frj[w] = v0; 
			} 
		}
	}
}

void initialize( Digraph G, Vertex s, Vertex parent[], float dist[], Vertex frj[]){ 
	Vertex w;
	lista a;
	float c;
	for (w = 0; w < G->V; w++) {
		parent[w] = -1;
		dist[w] = INFINITO;
	}
	parent[s] = s;
	dist[s] = 0.0;
	for (a = G->array[s]->adj; a != NULL; a = a->next) {
		w = a->w, c = a->weight;
		dist[w] = c; 
		frj[w] = s; 
	} 
}