#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Digraph.h"
#include "manager.h"

//Defini��o do tipo das vari�veis de endere�amento dos v�rtices (int).
#define Vertex int

//Defini��o do tipo booleano utilizado para compara��es.
#define bool int
#define true 1
#define false 0

#define INFINITO 1000000000000.0f	//Valor virtualmente infinito utilizado no algoritmo de Dijkstra.
#define maxV 20						//Valor m�ximo da franja da arborec�ncia em SPT.

/*	************ALTERA��O PROJETO FINAL************

	** DIGRAPHinit ** (Inicializa��o do Digrafo)
	Constr�i e retorna um digrafo com v�rtices 0 ... V-1 e todas arestas definidas no arquivo txt
	passado � fun��o. Primeiramente, a fun��o l� o arquivo, o interpreta e gera os v�rtices, ap�s
	s�o lidos os v�rtices de origem e, por fim, todas as arestas definidas, caso existam. � neces-
	s�rio que existam no arquivo de origem a defini��o dos v�rtices e pelo menos um v�rtice de ti-
	po origem.
	
	O digrafo definido aqui � composto de um array de listas de adjac�ncia do tipo VertexArray e 
	em cada um de seus n�s foi definida uma lista de adjac�ncia do tipo link. Al�m disso, � veri-
	ficado caso o arquivo exista, se n�o ser� requisitado do usu�rio a entrada de um arquivo v�-
	lido.
	
	Aten��o, arquivo de entrada "file.txt" deve seguir o padr�o:
	
	A, B, C, D 		//V�rtices
	A, B 			//V�rtices de origem
	A, C, 5.0		//Pode n�o haver defini��o de arestas
	B, D, 2.0
	C, D, 1.0
	
	Utiliza��o da fun��o:
	
	DIGRAPHinit("file.txt");
	Sendo que file.txt contenha a defini��o de um digrafo de acordo com a especifica��o acima.
	
	- Par�metros da fun��o: string contendo o nome do arquivo de defini��es do grafo.
	
	- Retorno da fun��o: digrafo do tipo Digraph contendo todas as defini��es lidas no arquivo txt.
*/
Digraph DIGRAPHinit(char* file){
	Vertex v = 0, w;
	int V = 1, i, dep;
	//char string[100], origem[100], dest[100];
	char aux;
	//float weight;
	
	//Declara��o e inicializa��o do digrafo.
	Digraph G = (Digraph)malloc(sizeof *G);
	
	FILE* fp = fopen(file, "r");
	while (fp == NULL){	//Exce��o: arquivo inexistente. Usu�rio � requisitado a digitar o nome correto do arquivo.
		printf("Arquivo invalido. Digite o nome correto do arquivo.\n");
		scanf("%30s", file);
		fp = fopen(file, "r");
	}
	
	/********************Leitura do Grafo********************/
	
	/*TRABALHO 2*/
	//Contagem dos V�rtices.
	//Leitura da primeira linha do arquivo para definir G->V.
	// do{
	// 	fscanf(fp, "%s", string);
	// 	//N�o � necess�rio tratar a string agora, j� que apenas uma contagem � feita.
	// 	V++;
	// 	fscanf(fp, "%c", &aux);	//L� espa�os ('\b') e '\n'.
	// }
	// while(aux != '\n');

	/*Contagem do n�mero de linhas do arquivo de entrada.*/
	while(!feof(fp)){
		aux = fgetc(fp);
		if(aux == '\n'){
			V++;
		}
	}	
	
	rewind(fp);		//Seta o cursor de leitura de volta ao in�cio do arquivo.
	
	//Aloca��o de mem�ria do vetor de listas adjacentes.
	//As listas de adjac�ncia s�o inicializadas como nulas.
	G->array = (VertexArray*)malloc(V * sizeof(VertexArray));
	for (v = 0; v < V; v++){
		G->array[v] = malloc(sizeof(struct vertexArray));
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

		if (G->array[v]->reqs > 0){
			G->array[v]->reqs_id = (int*)malloc(G->array[v]->reqs * sizeof(int));
			for (i = 0; i < G->array[v]->reqs; i++){
				fscanf(fp, "%d", &dep);
				w = VERTEXreturn(G, dep);
				while(w == -1){				//Garante que as depend�ncias s� podem ser de tarefas acima da atual.
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

		TIME(G, v);		//Calcula o tempo necess�rio para executar a tarefa lida contida em G->array[v].

		v++;
	}
	while(!feof(fp) && v < G->V);
	
	/*TRABALHO 2*/
	//Leitura dos V�rtices
	// for(v = 0; v < V; v++){					//A leitura � feita apenas V vezes sem necessidade da vari�vel auxiliar.
	// 	fscanf(fp, "%s", string);			//L� o nome do v�rtice.
	// 	removeComma(string);				//Tratamento da string lida.
	// 	strcpy(G->array[v]->name, string);	//Seta o nome do v�rtice.
	// 	G->array[v]->source = false;		//Inicializa os v�rtices como n�o sendo origem.
	// }
	
	/*TRABALHO 2*/
	//Leitura dos V�rtices de Origem
	// do{
	// 	fscanf(fp, "%s", string);		//Leitura do nome do v�rtice.
	// 	removeComma(string);			//Remo��o da v�rgula da string.
	// 	v = VERTEXreturn(G, string);	//Busca pelo v�rtice que cont�m a string desejada.
	// 	if(strcmp(G->array[v]->name, string) == 0) 
	// 		G->array[v]->source = true;	//Se encontr�-lo, diz que � origem.
	// 	fscanf(fp, "%c", &aux);			//L� '\b' e '\n'.
	// }
	// while(aux != '\n');
	
	/*TRABALHO 2*/
	//Leitura das Arestas (Edges)
	// while (!feof(fp)){					//Caso j� tenha atingindo o fim do arquivo, n�o h� arestas.
	// 	fscanf(fp, "%s", origem);		//L� o n� de origem da aresta
	// 	removeComma(origem);			//Remove a v�rgula
		
	// 	fscanf(fp, "%s", dest);			//L� o n� de destino da aresta
	// 	removeComma(dest);				//Remove a v�rgula
		
	// 	fscanf(fp, "%f", &weight);		//L� o peso da aresta.
		
	// 	v = VERTEXreturn(G, origem);	//Encontra a posi��o do n� de origem no array de listas.
	// 	w = VERTEXreturn(G, dest);		//Encontra a posi��o do n� de destino no array de listas.

	// 	if(G->array[v]->adj != NULL){	//Se j� existir um lista de adjac�ncia no n�, utiliza-se a fun��o DIGRAPHinsertE para inserir a aresta.
	// 		DIGRAPHinsertE(G, EDGE(v, w, weight));
	// 	}
	// 	else if(strcmp(G->array[v]->name, origem) == 0 && strcmp(G->array[w]->name, dest) == 0 && !G->array[w]->source){
	// 		//Insere a aresta definida se os nomes confirmarem e se o v�rtice de destino n�o seja origem.
	// 		G->array[v]->adj = NEWnode(w, weight, G->array[v]->adj);	//Insere a aresta definida.
	// 		G->E++;		//M�todo InsertE n�o foi usado porque este verifica por arestas existentes, as quais ainda n�o existem.
	// 	}
	// 	else{	//Impress�o da mensagem de erro. N�o � poss�ve adicionar uma aresta com destino em um v�rtice de origem.
	// 		printf("Aresta %s -> %s invalida.\n%s eh vertice de origem.\n\n", origem, dest, dest);
	// 	}
	// };
	
	fclose(fp);
	return G;
}

/*	************ALTERA��O PROJETO FINAL************

	Insere um arco v-w contido na struct Edge 'e' no digrafo 'G'. Se o digrafo j� possui v-w, a fun��o 
	imprime uma mensagem de erro. Caso a aresta contida em Edge "e" j� exista no digrafo, uma men-
	sagem de erro ser� impressa, j� que a cria��o de uma aresta redundante � uma poss�vel forma de
	vazamento de mem�ria durante a execu��o do programa.
	
	Utiliza��o da fun��o:
	
	DIGRAPHinsertE(G, e);
	
	'G' pode ser inicializado utilizando DIGRAPHinit(G) e 'e' utilizando a fun��o e = EDGE(v, w, weight).
	
	- Param�tros da fun��o: ponteiro de digrafo do tipo Digrafo e estrutura contendo informa��es de uma aresta
	do tipo Edge.
	
	- Retorno da fun��o: void.
*/
void DIGRAPHinsertE(Digraph G, Edge e){
	//Leitura de Edge 'e'.
	Vertex v = e.v, w = e.w;
	int id = e.id;
	if (!DIGRAPHadj(G, v, w)){	//Verifica se a aresta j� existe.
		//Cria um novo node e o insere no in�cio da lista de adjac�ncia.
		G->array[v]->adj = NEWnode(w, id, G->array[v]->adj);
		G->E++;	//Incremento n�mero de arestas adicionadas no grafo.
	}
	else{
		printf("Aresta ja existente no digrafo ou vertice de destino eh origem.\n%s->%s nao foi inserida.\n", G->array[v]->name, G->array[w]->name);
	}
	return;
}

/*	************ALTERA��O PROJETO FINAL************

	Esta fun��o cria um novo n� da lista de adjac�ncias ao receber as informa��es necess�rias
	contidas em cada um dos n�s.
	
	Utiliza��o da fun��o:
	
	link l = NEWnode(w, weight, l);
	
	A fun��o ent�o cria um n� e o aponta para 'l':
		new_node --> l
	
	- Par�metros da fun��o: v�rtice do tipo Vertex para o qual o v�rtice que possui esta lista
	de adjac�ncias ir� apontar, weight do tipo float que cont�m o peso da aresta criada e n� de
	origem da lista de adjac�ncia, j� que novos n�s s�o adicionados ao in�cio.
	
	- Retorno da fun��o: novo ponteiro do tipo link ao n� criado.
*/
link NEWnode(Vertex w, int id, link next){
	link a;
	a = (link)malloc(sizeof (struct node));
	a->w = w;
	a->id = id;
	a->next = next;
	return a;
}

/*	************ALTERA��O PROJETO FINAL************

	Remove um arco v-w contido na struct Edge 'e' do digrafo 'G'. Caso o digrafo n�o possuir o
	arco, a fun��o se encerra e imprime uma mensagem de erro ao usu�rio. A fun��o busca na lista
	de adjac�ncia do v�rtice 'v' pela aresta v-w e, ao encontr�-la, remove o n� designado e libe-
	ra a mem�ria deste n�, al�m de decrementar a quantidade de arestas do grafo. Al�m disso, tam-
	b�m � feita a verifica��o se o peso da aresta a ser removida est� correto, caso contr�rio uma
	mensagem de erro � impressa. Dessa forma, � poss�vel garantir a consist�ncia da opera��o e a
	remo��o das arestas corretas.
	
	Utiliza��o da fun��o:
	
	DIGRAPHremoveE(G, e);
	
	Seja G:
	||Vertex A||
			  -(1.0)->[Vertex B]
			  -(2.0)->[Vertex C]
	||Vertex B||
	||Vertex C||
	
	E Edge e:
	e = {0, 1, 1.0} (sendo 0 e 1 os �ndices de A e B encontrados utilizando VERTEXreturn(G, nome))
	
	Ap�s chamar a fun��o teremos:
	||Vertex A||
				-(2.0)->[Vertex C]
	||Vertex B||
	||Vertex C||
	
	- Param�tros da fun��o: Digrafo do tipo Digraph e aresta do tipo Edge.
	
	-Retorno da fun��o: void.
*/
void DIGRAPHremoveE(Digraph G, Edge e){
	Vertex v = e.v, w = e.w;
	link current, prev;
	if(DIGRAPHadj(G, v, w)){
		for (current = G->array[v]->adj; current != NULL && current->w != w; current = current->next);	//Encontra o n� a ser removido
		if(G->array[v]->adj != current){																//Caso este n� n�o seja o primeiro da lista
			for (prev = G->array[v]->adj; prev != NULL && prev->next != current; prev = prev->next);	//Procura pelo n� imediatamente anterior a esse
			if(e.id == current->id || e.id == -1){ //Exce��o -1 criada para DIGRAPHremoveV.
				prev->next = current->next;
				free(current);
				G->E--;
			}
			else{
				printf("Arestra nao removida. ID incorreto.\n");
			}
		}
		else{
			G->array[v]->adj = current->next;		//� o primeiro da lista, ent�o passa o seguinte � cabe�a da lista e desloca o n�.
			free(current);
			G->E--;
		}
	}
	else{
		printf("Aresta %d->%d nao encontrada. ", G->array[v]->id, G->array[w]->id);
		printf("Nao foi possivel remove-la.\n");
	}
}

/*	************ALTERA��O PROJETO FINAL************

	Esta fun��o insere um novo v�rtice no digrafo. Como o array de v�rtices n�o foi feito utilizando
	uma lista encadeada, � necess�rio realocar a sua mem�ria de forma a caber mais uma entrada no vetor.
	Assim, primeiramente � feita a verifica��o se o v�rtice inserido j� existe no array, caso seja ver-
	dade uma mensagem de erro � impressa. Caso contr�rio, o n�mero de v�rtices no grafo � incrementado,
	a mem�ria do array de v�rtices � realocada e um novo v�rtice � inicializado ao final do array.
	
	Utiliza��o da fun��o:
	
	DIGRAPHinsertV(G, nome);
	
	Caso G for
	
	[Vertex A]
	[Vertex B]
	[Vertex C]
	
	e nome = 'D', ap�s chamar a fun��o teremos:
	
	[Vertex A]
	[Vertex B]
	[Vertex C]
	[Vertex D]
	
	- Param�tros da fun��o: Digrafo do tipo Digraph e uma string que cont�m o nome do novo v�rtice.
	
	- Retorno da fun��o: void.
	
*/
void DIGRAPHinsertV(Digraph G){
	Vertex w;
	int id, i;
	char exec;

	printf("Entre com o ID da tarefa:\n");
	scanf("%d", &id);

	while (VERTEXreturn(G, id) != -1){
		printf("ID ja existente. Entre com outro valor: ");
		scanf("%d", &id);
	}

	G->V++;
	G->array = realloc(G->array, G->V * sizeof(VertexArray));
	G->array[G->V-1] = malloc(sizeof(struct vertexArray));
	
	G->array[G->V-1]->id = id;

	printf("Entre com o nome da tarefa (ate 100 caracteres):\n");
	scanf("%100s", G->array[G->V-1]->name);

	for (w = 0; w < G->V-1; w++){
		if (strcmp(G->array[G->V-1]->name, G->array[w]->name) == 0){
			printf("Nome ja existente. Entre com outra string (ate 100 caracteres: ");
			scanf("%100s", G->array[G->V-1]->name);
			w = -1;
		}
	}

	printf("Tarefa ja executada? s/n\n");
	scanf(" %c", &exec);
	switch(exec){
		case 's':
			G->array[G->V-1]->exec = true;
			break;
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

	while (G->array[G->V-1]->reqs > G->V-1){
		printf("Numero excede o maximo de requisitos possiveis. Entre outro valor: ");
		scanf("%d", &G->array[G->V-1]->reqs);
	}

	if(G->array[G->V-1]->reqs > 0){
		G->array[G->V-1]->reqs_id = (int*)malloc(G->array[G->V-1]->reqs * sizeof(int));
		for(i = 0; i < G->array[G->V-1]->reqs; i++){
			printf("Entre com o ID do pre-requisito %d: ", i+1);
			scanf("%d", &id);
			w = VERTEXreturn(G, id);
			while(w == -1){
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


/*	************ALTERA��O PROJETO FINAL************

	Como nesta implementa��o foi criado um array de v�rtices em vez de uma lista encadeada, algumas
	limita��es s�o encontradas ao remover um elemento deste array. Ent�o, para isso, � necessaria a
	realoca��o de mem�ria do array, al�m do rearranjo dos elementos de acordo com elemento que est�
	sendo removido. Primeiramente, encontra-se o v�rtice a ser removido, desaloca toda sua lista de
	adjac�ncia e remove todas as outras arestas existentes no grafo que apontem para esse v�rtice (
	caso n�o seja um v�rtice de origem), move o v�rtice a ser removido para o fim do vetor, realoca
	o array de v�rtices para uma unidade menor do que a atual e desaloca o vetor removido. Al�m dis-
	so, caso o v�rtice removido n�o for o �ltimo v�rtice do array, tamb�m � necess�rio mudar a refe-
	r�ncia dos v�rtices em todas as listas de adjac�ncia, j� que ter�o diminuido em 1 unidade.
	
	Utiliza��o da fun��o:
	
	DIGRAPHremoveV(G, v);
	
	Caso G for:
	
	[Vertex A]
	[Vertex B]
	[Vertex C]
	
	e v = 2 (�ndice de C encontrado com VERTEXreturn(G, "C")), teremos:
	
	[Vertex A]
	[Vertex B]
	
	- Param�tros da fun��o: Digrafo do tipo Digraph e string do nome do v�rtice a ser removido.
	
	- Retorno da fun��o: void.	
*/
void DIGRAPHremoveV(Digraph G, int id){
	Vertex v = VERTEXreturn(G, id);

	if (v == -1){
		return;
	}
	
	Vertex w;
	int i , j;
	link temp = G->array[v]->adj;
	link l = G->array[v]->adj;
	VertexArray temp_array = G->array[v];

	//Remover refer�ncias ao v�rtice
	//Verificar depend�ncias
	if (G->array[v]->reqs > 0){
		for (i = 0; i < G->array[v]->reqs; i++){
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);
			DIGRAPHremoveE(G, EDGE(w, v, id));
		}
		free(G->array[v]->reqs_id);
	}

	//Verificar lista de adjac�ncia (v�rtices que dependem deste)
	while (l != NULL){
		w = l->w;
		for (i = 0; i < G->array[w]->reqs && G->array[v]->id != G->array[w]->reqs_id[i]; i++);	//Encontra a posi��o de ID no array de IDs do v�rtice w.
		for(j = i; j < G->array[w]->reqs-1; j++){		//Remo��o da ID do array de IDs
			G->array[w]->reqs_id[j] = G->array[w]->reqs_id[j+1];
		}
		G->array[w]->reqs--;
		G->array[w]->reqs_id = (int*)realloc(G->array[w]->reqs_id, G->array[w]->reqs * sizeof(int));
		temp = l;
		l = l->next;
		free(temp);
		G->E--;
	}

	//Remover lista de adjac�ncia
	// while (l != NULL){
	// 	temp = l;
	// 	l = l->next;
	// 	free(temp);
	// 	G->E--;
	// };
	
	//Remover arestas que apontam ao v�rtice v.
	// if(!G->array[v]->source){
	// 	for(i = 0; i < G->V; i++){
	// 		if(i != v) DIGRAPHremoveE(G, EDGE(i, v, -1));
	// 	}
	// }
	
	//Rearranjo dos v�rtices no vetor para exclus�o do v�rtice desejado.
	for(i = v; i < G->V-1; i++){
		G->array[i] = G->array[i+1];
	}
	G->V--;
	free(temp_array);
	G->array = (VertexArray*)realloc(G->array, G->V * sizeof(VertexArray));

	//Caso o v�rtice removido n�o tiver sido o �ltimo do vetor, � necess�rio corrigir os �ndices
	//dos n�s das listas de adjac�ncia.
	if(v < G->V+1){
		for(i = 0; i < G->V; i++){										//Percorre o array de v�rtices.
			if(G->array[i]->adj != NULL) {								//Verifica se o v�rtice possui lista de adjc�ncia.
				for(l = G->array[i]->adj; l != NULL; l = l->next){		//Percorre a lista de adjac�ncia do v�rtice.
					if(l->w >= v) l->w--;								//Se o �ndice dessa lista for maior ou igual ao �ndice do v�rtice removido, atualize-o.
				}
			}
			TIME(G, i);
		}
	}
}

/*	************ALTERA��O PROJETO FINAL************

	Cria e retorna uma aresta (edge) do tipo Edge dos v�rtices v-w contendo um peso (weight). Esta fun-
	��o simplesmenet inicializa uma estrutura do tipo Edge contendo os valores de aresta que ser�o pas-
	sados a outra fun��o que as utilizar� de acordo com o que for implementado.
	
	Utiliza��o da fun��o:
	
	e = EDGE(v, w, weight);
	
	Sendo v e w do tipo Vertex e weight float. Isto cria a aresta e = {v, w, weight}.
	
	- Param�tros da fun��o: dois v�rtices do tipo Vertex e o peso do tipo float dessa aresta v-w.
	
	- Retorno da fun��o: aresta do tipo Edge.
*/
Edge EDGE(Vertex v, Vertex w, int id){
	Edge e;
	if(v >= 0 && w >= 0 && id >= -1){
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
	Esta fun��o faz a verifica��o da exist�ncia de uma aresta checando a lista de adjac�ncia de um
	v�rtice do digrafo G.
	
	Utiliza��o da fun��o:
	
	bool checa = DIGRAPHadj(G, v, w);
	
	- Param�tros da fun��o: digrafo do tipo Digraph e dois v�rtices do tipo Vertex.
	
	- Retorno da fun��o: true (1) caso a aresta exista, false (0) caso contr�rio.
*/
bool DIGRAPHadj(Digraph G, Vertex v, Vertex w){
	link x;
	for (x = G->array[v]->adj; x != NULL; x = x->next)
		if(x->w == w)
			return true;
	return false;
}

/*	************ALTERA��O PROJETO FINAL************
	*****************N�O UTILIZADA*****************

	Fun��o auxiliar que remove a v�rgula das strings lidas do arquivo.
	
	Utiliza��o da fun��o:
	
	removeComma(teste);
	
	Sendo teste = "teste,", a fun��o modifica a string para "teste".
	
	- Param�tros da fun��o: vetor de caracteres (string).
	
	- Retorno da fun��o: void.
*/
// void removeComma(char* string){
// 	int i;
// 	for (i = 0; string[i] != '\0' && i < 100; i++){
// 		if(string[i] == ',') string[i] = '\0';		//Remove a v�rgula substituindo por fim de string.
// 	}
// }

/*	************ALTERA��O PROJETO FINAL************

	Esta fun��o desaloca o digrafo G entrado, assim como cada v�rtice contido em seu array de v�rtices,
	al�m de cada um dos n�s das listas de adjac�ncias de cada um destes v�rtices. Isso previne o vaza-
	mento de mem�ria e a correta devolu��o da mem�ria ao sistema.
	
	Utiliza��o da fun��o:
	
	DIGRAPHdestro(G);
	
	Desloca toda mem�ria alocada ao digrafo G.
	
	- Param�tros da fun��o: Digrafo do tipo Digraph.
	
	- Retorno da fun��o: void.
*/
void DIGRAPHdestroy(Digraph G){
	int v;
	link temp, l;
	
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

	//Desalocar vetor de IDs
	for (v = 0; v < G->V; v++){
		if (G->array[v]->reqs > 0){
			free(G->array[v]->reqs_id);
		}
	}
	
	//Desalocar array de listas
	for (v = 0; v < G->V; v++){
		free(G->array[v]);
	}
	free(G->array);
	G->array = NULL;
	
	//Desalocar grafo
	free(G);
	G = NULL;
}

/*	************ALTERA��O PROJETO FINAL************

	Fun��o de impress�o do digrafo explicitado por seus vetores e l�stas de adjac�ncia. A impress�o � feita
	no formato:
	
	||Vertex A||:
			--(5.0)-->[Vertex G]
			--(10.0)-->[Vertex F]
	||Vertex B||:
			...
	
	Utiliza��o da fun��o:
	
	DIGRAPHshow(G);
	
	Resultados obtidos como mostrado acima.
	
	- Param�tros da fun��o: digrafo do tipo Digraph.
	- Retorno da fun��o: void.
*/
void DIGRAPHshow(Digraph G){
	link l;
	Vertex v;
	int i;
	
	printf("Grafo:\tV:%d\tE:%d\n\n", G->V, G->E);
	printf("ID\tNOME\t\tEXEC\tDURACAO\tINICIO_MIN\tPRE_REQS\tDEPS\n");
	for (v = 0; v < G->V; v++){
		printf("%d\t%s\t\t%d\t%d\t%d\t\t%d\t", 
				G->array[v]->id, G->array[v]->name, G->array[v]->exec, G->array[v]->duration, G->array[v]->min_start, G->array[v]->reqs);
		if (G->array[v]->reqs > 0){
			for(i = 0; i < G->array[v]->reqs; i++) printf("\t%d", G->array[v]->reqs_id[i]);
		}

		else {
			printf("\t-");
		}
		printf("\n");
		if (G->array[v]->adj != NULL){
			printf("Adj:");
			for (l = G->array[v]->adj; l != NULL; l = l->next){
				printf(" %d", l->id);
			}
			printf("\n");
		}
		printf("Time: %d\n", G->array[v]->time);
		printf("\n");
	}
}

/*	************ALTERA��O PROJETO FINAL************

	Esta fun��o auxiliar apenas faz uma busca no array de v�rtices do digrafo G pelo �ndice do vetor contendo
	o nome passado � fun��o. Ela imprime um erro e retorna -1 caso o v�rtice n�o exista no digrafo.
	
	Utiliza��o da fun��o:
	
	Vertex i = VERTEXreturn(G, "A");
	
	Retornaria i = 0 de acordo com os exemplos anteriores.
	
	- Param�tros da fun��o: Digrafo do tipo Digraph e string contendo o nome do v�rtice.
	
	- Retorno da fun��o: Vertex contendo o �ndice do v�rtice com nome "vertexName".
*/
Vertex VERTEXreturn(Digraph G, int id){
	Vertex v = 0;
	while(G->array[v]->id != id && v < G->V-1) v++;
	if(G->array[v]->id == id) return v;
	else{
		printf("Vertice nao encontrado. Error code: -1.\n");
		return -1;
	}
}

/*
	Esta fun��o imprime em um arquivo "output.txt" o grafo no mesmo padr�o do arquivo de entrada utilizado em
	DIGRAPHinit. Este arquivo pode ser utilizado para a inicializa��o de outro digrafo normalmente, j� que se-
	gue o padr�o conforme dito.
	
	Utiliza��o da fun��o:
	
	DIGRAPHsave(G);
	
	Gerar� um arquivo "output.txt" na pasta src do projeto.
	
	- Param�tros da fun��o: Digrafo do tipo Digraph.
	
	- Retorno da fun��o: void.
*/
// void DIGRAPHsave(Digraph G){
// 	FILE* p = fopen("output.txt", "w");
// 	link aux;
// 	int i, count = 0, count2 = 0;
	
// 	for(i = 0; i < G->V; i++){
// 		fprintf(p, "%s", G->array[i]->name);
// 		if(i != G->V-1) fprintf(p, ", ");
// 		else fprintf(p, "\n");
// 	}
	
// 	for(i = 0; i < G->V; i++){
// 		if(G->array[i]->source) count++;
// 	}
	
// 	for(i = 0; i < G->V; i++){
// 		if(G->array[i]->source){
// 			fprintf(p, "%s", G->array[i]->name);
// 			count2++;
// 			if(count2 < count) fprintf(p, ", ");
// 		}
// 	}
// 	fprintf(p, "\n");
	
// 	for(i = 0; i < G->V; i++){
// 		aux = G->array[i]->adj;
// 		while(aux != NULL){
// 			fprintf(p, "%s, ", G->array[i]->name);
// 			fprintf(p, "%s, ", G->array[aux->w]->name);
// 			fprintf(p, "%.1f\n", aux->weight);
// 			aux = aux->next;
// 		}
// 	}
	
// 	fclose(p);
//}

/*
	Esta fun��o verifica se o grafo � conexo a partir dos v�rtices de origem. Ela utiliza o algoritmo de Dijkstra
	definido em SPT (Shortest Path Tree), a qual al�m de retornar a conectividade do digrafo, tamb�m retorna o menor
	caminho a partir de um n� a todos os outros, o que � utilizado em FindPath(Digraph).
	
	Utiliza��o da fun��o:
	
	bool checa = isConnected(G);
	
	- Param�tros da fun��o: Digrafo do tipo Digraph.
	
	- Retorno da fun��o: true (1) se a partir das origens � poss�vel chegar a qualquer outra aresta. False (0) caso
	contr�rio.
*/
// bool isConnected(Digraph G){
// 	bool flag = true;
// 	int* parent = (int*)malloc(G->V * sizeof(int));
// 	float* dist = (float*)malloc(G->V * sizeof(float));
// 	Vertex i, v;
// 	for(i = 0; i < G->V && flag; i++){
// 		if(G->array[i]->source){
// 			SPT(G, i, parent, dist);
// 			for(v = 0; v < G->V; v++)
// 				if(parent[v] == -1) flag = false;
// 		}
// 	}
// 	free(parent);
// 	free(dist);
// 	return flag;
// }

/*
	Esta fun��o encontra o menor caminho entre dois v�rtices v-w. Caso n�o exista, -1 � retornado. Ela usa o algoritmo
	de Dijkstra para encontra a SPT (Shortest Path Tree) do digrafo a partir de um n� �ncora. Os array parent e dist
	cont�m, respectivamente, os v�rtices "pais" de um v�rtice "v" (parent[w] � o pai de w) e a dist�ncia de v at� w 
	indexado por dist[w].
	
	Utiliza��o da fun��o:
	
	float dist = FindPath(G, v, w);
	
	Nota: v e w s�o encontrados utilizando a fun��o VERTEXreturn(G, string).
	
	- Param�tros da fun��o: Digrafo do tipo Digraph e dois v�rtices do tipo Vertex.
	
	- Retorno da fun��o: dist�ncia m�nima entre v e w (float).
*/
// float FindPath(Digraph G, char* origem, char* dest){
// 	Vertex v = VERTEXreturn(G, origem);
// 	Vertex w = VERTEXreturn(G, dest);
// 	int* parent = (int*)malloc(G->V * sizeof(int));
// 	float* dist = (float*)malloc(G->V * sizeof(float));
// 	float retorno;
	
// 	SPT(G, v, parent, dist);

// 	if(-1*(dist[w] - INFINITO) > 1) 
// 		retorno = dist[w];
// 	else 
// 		retorno = -1;
// 	free(parent);
// 	free(dist);
// 	return retorno;
//}

/*
	As duas seguintes fun��es implementam o algorito de Dijkstra para a SPT (Shortest Path Tree) baseado no livro de Segdewick
	"Algorithms in C", se��o "Dijkstra's Algorithm". Pequenas mudan�as foram feitas para se adaptar ao digrafo atual.
	
	Em resumo, o algoritmo retorna a �rvore de menor caminho a partir de um n� �ncora, incluindo os n�s parentes e a menor dis-
	t�ncia deste n� at� todos os outros. A fun��o initialize inicializa os vetores parent, dist e frj para que o algoritmo seja
	executado corretamente. As fun��es FindPath e IsConnected utilizam este algoritmo para verificar a exist�ncia de um caminho
	entre dois v�rtices do digrafo e se o digrafo � conexo, respectivamente.
	
	Mais detalhes podem ser encontrados em http://www.ime.usp.br/~pf/algoritmos_para_grafos/aulas/dijkstra.html
*/
// void SPT(Digraph G, Vertex s, Vertex parent[], float dist[]){
	
// 	Vertex v0, w, frj[maxV];
// 	link a; float c;
// 	initialize( G, s, parent, dist, frj);

// 	while (1) {
// 		float mindist = INFINITO;
// 		for (w = 0; w < G->V; w++) 
// 			if (parent[w] == -1 && mindist > dist[w])
// 				mindist = dist[v0=w]; 
// 		if (-1*(mindist - INFINITO) < 1) break;	//N�o � poss�vel garantir mindist == INFINITO
// 		parent[v0] = frj[v0];
// 		for (a = G->array[v0]->adj; a != NULL; a = a->next) {
// 			w = a->w, c = a->weight;
// 			if (parent[w] == -1 && dist[w] > dist[v0] + c) { 
// 				dist[w] = dist[v0] + c; 
// 				frj[w] = v0; 
// 			} 
// 		}
// 	}
// }

// void initialize( Digraph G, Vertex s, Vertex parent[], float dist[], Vertex frj[]){ 
// 	Vertex w;
// 	link a;
// 	float c;
// 	for (w = 0; w < G->V; w++) {
// 		parent[w] = -1;
// 		dist[w] = INFINITO;
// 	}
// 	parent[s] = s;
// 	dist[s] = 0.0;
// 	for (a = G->array[s]->adj; a != NULL; a = a->next) {
// 		w = a->w, c = a->weight;
// 		dist[w] = c; 
// 		frj[w] = s; 
// 	} 
//}