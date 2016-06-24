#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

/*Definição do tipo das variáveis de endereçamento dos vértices (int).*/
#define Vertex int

/*	************ALTERAÇÃO PROJETO FINAL************

	** DIGRAPHinit ** (Inicialização do Digrafo)
	Constrói e retorna um digrafo com vértices 0 ... V - 1 e todas arestas definidas no arquivo txt
	passado à função. Primeiramente, a função lê o arquivo, o interpreta e gera os vértices, após
	são lidos os vértices de origem e, por fim, todas as arestas definidas, caso existam. É neces-
	sário que existam no arquivo de origem a definição dos vértices e pelo menos um vértice de ti-
	po origem.
	
	O digrafo definido aqui é composto de um array de LinkedLists de adjacência do tipo VertexArray e 
	em cada um de seus nós foi definida uma LinkedList de adjacência do tipo LinkedList. Além disso, é veri-
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
Digraph DIGRAPHinit(char* file, bool (*inputCheck)(int), bool (*nameCheck)(Digraph, char*, int))
{
	Vertex 	v = 0, 
			w;
	int V = 1, 
		i, 
		id,
		exec,
		duration,
		min_start,
		reqs,
		dep;
	char 	name[100], 
			aux;
	
	/*Declaração e inicialização do digrafo.*/
	Digraph G = (Digraph)malloc(sizeof *G);
	assert(G);
	
	FILE* fp = fopen(file, "r");
	while (fp == NULL) /*Exceção: arquivo inexistente. Usuário é requisitado a digitar o nome correto do arquivo.*/
	{
		printf("Arquivo invalido. Digite o nome correto do arquivo.\n");
		scanf("%30s", file);
		fp = fopen(file, "r");
	} /*while*/
	
	/********************Leitura do Grafo********************/
	/*Contagem do número de linhas do arquivo de entrada.*/
	while(!feof(fp)) 
	{
		aux = fgetc(fp);
		if (aux == '\n') 
		{
			V++;
		} /*if*/
	} /*while*/
	
	rewind(fp);		/*Seta o cursor de leitura de volta ao início do arquivo.*/
	
	/*Alocação de memória do vetor de LinkedLists adjacentes.*/
	/*As LinkedLists de adjacência são inicializadas como nulas.*/
	G->array = (VertexArray*)malloc(V * sizeof(VertexArray));
	assert(G->array);
	for (v = 0; v < V; v++) 
	{
		G->array[v] 		= (struct vertexArray*)malloc(sizeof(struct vertexArray));
		assert(G->array[v]);
		G->array[v]->adj 	= NULL;
		G->array[v]->id 	= -1;
	} /*for*/
	G->V = V;
	G->E = 0;

	/*Leitura das Tarefas*/
	v = 0;
	do {
		fscanf(fp, "%d %*c 	%100[^']' 	%d 		%d 			%d 			%d", 
					&id, 	name,		&exec, 	&duration, 	&min_start, &reqs);

		assert(nameCheck(G, name, v));
		assert(inputCheck(id));
		assert(inputCheck(exec));
		assert(inputCheck(duration) || duration == 0);
		assert(inputCheck(min_start));
		assert(inputCheck(reqs));

		strcpy(G->array[v]->name, name);
		G->array[v]->id 		= id;
		G->array[v]->exec 		= exec; /*Evitar warning de incompatibilidade int/bool.*/
		G->array[v]->duration 	= duration;
		G->array[v]->min_start 	= min_start;
		G->array[v]->reqs 		= reqs;

		if (G->array[v]->reqs > 0) 
		{
			G->array[v]->reqs_id = (int*)malloc(G->array[v]->reqs * sizeof(int));
			for (i = 0; i < G->array[v]->reqs; i++) 
			{
				fscanf(fp, "%d", &dep);
				w = VERTEXreturn(G, dep);
				while (w == -1) /*Garante que as dependências só podem ser de tarefas acima da atual.*/ 
				{
					printf("ID inexistente. Corrija o arquivo ou entre outro valor: ");	
					scanf("%d", &dep);
					w = VERTEXreturn(G, dep);
				} /*while*/

				DIGRAPHinsertE(G, EDGE(w, v, G->array[v]->id));
				G->array[v]->reqs_id[i] = dep;
			} /*for*/
		} else
		{
			G->array[v]->reqs_id = NULL;
		} /*if*/

		TIME(G, v);		/*Calcula o tempo necessário para executar a tarefa lida contida em G->array[v].*/
		v++;
	} while(!feof(fp) && v < G->V);
	
	fclose(fp);
	return G;
}

bool INPUTcheck(int value) 
{
	if (value < 0)
	{
		return false;
	} else
	{
		return true;
	}
}

bool NAMEcheck(Digraph G, char* name, int V)
{
	Vertex w;
	for (w = 0; w < V; w++) 
	{
		if (strcmp(G->array[w]->name, name) == 0) 
		{
			return 0;
		} /*if*/
	} /*for*/
	return 1;
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

	Esta função insere um novo vértice no digrafo. Como o array de vértices não foi feito utilizando
	uma LinkedList encadeada, é necessário realocar a sua memória de forma a caber mais uma entrada no vetor.
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
void DIGRAPHinsertV(Digraph G, bool (*inputCheck)(int), bool (*nameCheck)(Digraph, char*, int)) 
{
	Vertex w;
	char name[100];
	int id,
		duration,
		min_start,
		reqs,
		i;
	char exec;

	printf("Entre com o ID da tarefa: ");
	scanf("%d", &id);

	while ((VERTEXreturn(G, id) != -1) || (VERTEXreturn(G, id) < -2))
	{
		printf("ID invalida ou existente. Entre com outro valor: ");
		scanf("%d", &id);
	} /*while*/

	G->V++;
	G->array 				= (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));
	assert(G->array);
	G->array[G->V - 1] 		= (struct vertexArray*)malloc(sizeof(struct vertexArray));
	assert(G->array[G->V - 1]);
	G->array[G->V - 1]->id 	= id;

	printf("Entre com o nome da tarefa (ate 100 caracteres): ");
	getchar();
	scanf("%100[^\n]s", name);

	while (!nameCheck(G, name, G->V - 1)) {
		printf("Nome ja existente. Entre com outra string (ate 100 caracteres: ");
		scanf("%100s", name);
	}
	strcpy(G->array[G->V - 1]->name, name);


	printf("Tarefa ja executada? s/n\n");
	scanf(" %c", &exec);
	switch(exec)
	{
		case 'S':
		case 's':
			G->array[G->V - 1]->exec = true;
			break;
		case 'N':
		case 'n':
			G->array[G->V - 1]->exec = false;
			break;
		default:
			G->array[G->V - 1]->exec = false;
			break;
	} /*switch*/

	printf("Entre com a duracao da tarefa: ");
	scanf("%d", &duration);
	while (!inputCheck(duration) || duration == 0) 
	{
		printf("(!) Duracao invalida. Entre outro valor: ");
		scanf("%d", &duration);
	} /*while*/
	G->array[G->V - 1]->duration = duration;

	printf("Entre com o inicio minimo da tarefa: ");
	scanf("%d", &min_start);
	while (!inputCheck(min_start)) 
	{
		printf("(!) Inicio invalido. Entre outro valor: ");
		scanf("%d", &min_start);
	} /*while*/
	G->array[G->V - 1]->min_start = min_start;

	printf("Esta tarefa possui quantos requisitos? ");
	scanf("%d", &reqs);
	while ((!inputCheck(reqs)) || (reqs >= G->V)) 
	{
		printf("Numero invalido ou excede o maximo de requisitos possiveis. Entre outro valor: ");
		scanf("%d", &reqs);
	} /*while*/
	//G->array[G->V - 1]->reqs = reqs;
	G->array[G->V - 1]->reqs = 0;

	if (inputCheck(G->array[G->V - 1]->reqs)) 
	{
		G->array[G->V - 1]->reqs_id = (int*)malloc(/*G->array[G->V - 1]->*/reqs * sizeof(int));
		assert(G->array[G->V - 1]->reqs_id);
		for (i = 0; i < reqs/*G->array[G->V - 1]->reqs*/; i++) 
		{
			printf("Entre com o ID do pre-requisito %d: ", i + 1);
			scanf("%d", &id);
			w = VERTEXreturn(G, id);
			while ((w == -1) || (w >= G->V - 1) || (FINDreqs_id(G->array[G->V - 1]->reqs_id, G->array[G->V - 1]->reqs, id) != -1)) 
			{
				printf("ID invalido ou inexistente. Entre outro valor: ");
				scanf("%d", &id);
				w = VERTEXreturn(G, id);
			} /*while*/
			DIGRAPHinsertE(G, EDGE(w, G->V - 1, G->array[G->V - 1]->id));
			G->array[G->V - 1]->reqs_id[i] = id;
			G->array[G->V - 1]->reqs++;		/*Evita o acesso à partes inicializadas do array por FINDreqs_id().*/
		} /*for*/
		G->array[G->V - 1]->reqs = reqs;
	} else 
	{
		G->array[G->V - 1]->reqs_id = NULL;
	} /*if*/

	G->array[G->V - 1]->adj = NULL;

	TIME(G, G->V - 1);
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

	for (i = 0; i < reqs; i++) 
	{
		if (id == reqs_id[i])
		{
			return i;
		}
	} /*for*/
	return -1;
}