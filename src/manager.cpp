#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <assert.h>
#include "../include/Digraph.h"
#include "../include/manager.h"
#include "../include/GUI.h"

/*	Funcão TIME que calcula o tempo necessário para executar uma tarefa em "v".
*	Esta função começa o cálculo a partir de tarefas que não possuem dependências,
*	pois o cálculo é trivial: o ciclo mínimo de início + duração da tarefa. Nos
*	outros casos, considera-se que todas as dependências estejam acima da tarefa
*	atual, o que significa que seu tempo já foi calculado. Dessa forma, existem
*	dois casos: o que o ciclo de início < tempo mais longo das dependências e o
*	caso em que o ciclo é maior. Na primeira situação, o tempo da tarefa atual
*	será o tempo de executar a dependência mais demorada + tempo de duração da
*	tarefa atual, já que o ciclo de início da tarefa já terá passado. Já no se-
*	gundo caso, como o tempo de início da tarefa > tempo das dependências, pode-
*	-se ignorar o tempo de execução das dependências e somar o ciclo de início
*	da tarefa à sua duração.
*
*	Em resumo:
*
*	reqs == 0:	(tarefa sem dependências)
*		time = min_start + duration
*
*	reqs > 0:	(tarefa com pelo menos uma dependência)
*		min_start < longest_time (tempo mais longo de todas dependências)
*			time = longest_time + duration
*		min_start >= longest_time
*			time = min_start + duration
*
*	@param: digrafo G e vértice da tarefa atual v.
*	@return: void.
*/
void TIME(Digraph G, Vertex v) 
{

	/*Caso não haja dependências*/
	if (G->array[v]->reqs == 0) 
	{
		G->array[v]->time = G->array[v]->min_start + G->array[v]->duration;	//time = min_start + duration*/
		return;
	}

	else {
		int i, 
			longest_time = 0;
		Vertex w;
		for(i = 0; i < G->array[v]->reqs; i++) 
		{
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);				//Encontra o vértice da dependência reqs_id[i]*/
			if(G->array[w]->time > longest_time && !G->array[w]->exec) 
			{	//Se a instrução já tiver sido executada, seu tempo de executação não é levado em conta.*/
				longest_time = G->array[w]->time;						//Procura pela dependência de maior duração.*/
			} /*if*/
		} /*for*/
		if (G->array[v]->min_start < longest_time) //Se min_start < longest_time.*/
		{						
			G->array[v]->time = longest_time + G->array[v]->duration;	//time = longest_time + duration.*/
		} else //Se min_start >= longest_time.*/
		{
			G->array[v]->time = G->array[v]->min_start + G->array[v]->duration;		//time = min_start + duration.*/
		} /*if*/
		return;
	}
}

int NEWid(Digraph G, Vertex v) 
{
	int new_id, 
		i;
	LinkedList l;
	Vertex w;

	//Leitura do novo ID.*/
	printf("> Entre com o novo ID: ");
	scanf("%d", &new_id);
	w = VERTEXreturn(G, new_id);
	while (w != -1 || w < -2) 
	{
		printf("(!) ID invalida ou ja existente. Entre com outro valor: ");
		scanf("%d", &new_id);
		w = VERTEXreturn(G, new_id);
	} /*while*/

	//Atualização do vetor de IDs das tarefas que dependem desta tarefa em "v"
	l = G->array[v]->adj;
	while (l != NULL) 
	{
		i = FINDreqs_id(G->array[l->w]->reqs_id, G->array[l->w]->reqs, G->array[v]->id);
		G->array[l->w]->reqs_id[i] = new_id;
		l = l->next;
	} /*while*/

	//Atualização da LinkedList de adjacência das tarefas que "v" depende
	for (i = 0; i < G->array[v]->reqs; i++) 
	{
		w = VERTEXreturn(G, G->array[v]->reqs_id[i]);
		l = G->array[w]->adj;
		while (l->next != NULL && l->w != v) 
		{
			l = l->next;
		} /*while*/
		if (l->id == G->array[v]->id)
		{
			l->id = new_id;
		} /*if*/
	} /*for*/
	return new_id;
}

void NEWname(Digraph G, Vertex v) 
{
	Vertex w;
	char name[100];
	printf("> Entre com o novo nome (ate 100 caracteres): ");
	getchar();
	scanf("%99[^\n]s", name);
	for (w = 0; w < G->V-1; w++) 
	{
		if (strcmp(name, G->array[w]->name) == 0)
		{
			printf("(!) Nome ja existente. Entre com outra string (ate 100 caracteres): ");
			scanf("%99s", name);
			w = -1;
		} /*if*/
	} /*for*/
	strcpy(G->array[v]->name, name);
}

bool NEWexec(bool exec) 
{
	char option;
	if(exec == true) 
	{
		printf("> A tarefa ja foi executada.\n");
	} else 
	{
		printf("> A tarefa ainda nao foi executada.\n");
	} /*if*/
	printf("> Inverter o estado? s/n\n");
	scanf (" %c", &option);
	while(option != 's' && option != 'S' && option != 'n' && option != 'N') 
	{
		printf("(!) Opcao inexistente. Entre [s/n]: ");
		scanf(" %c", &option);
	} /*while*/
	switch (option) 
	{
		case 'S':
		case 's':
			exec = !exec;
			break;
		case 'N':
		case 'n':
			break;
		default:
			break;
	} /*switch*/
	return exec;
}

int NEWduration() 
{
	int duration;
	printf("> Entre com a nova duracao: ");
	scanf("%d", &duration);
	while (duration < 0) 
	{
		printf("(!) Duracao invalida. Entre outro valor: ");
		scanf("%d", &duration);
	} /*while*/
	return duration;
}

int NEWmin_start() 
{
	int min_start;
	printf("> Entre com o novo inicio minimo: ");
	scanf("%d", &min_start);
	while (min_start < 0) 
	{
		printf("(!) Inicio invalido. Entre outro valor: ");
		scanf("%d", &min_start);
	} /*while*/
	return min_start;
}

int NEWreqs(Digraph G, Vertex v)
{
	int reqs, 
		i, 
		j, 
		k, 
		id;
	Vertex w;
	printf("> Entre com o novo numero de pre-requisitos: ");
	scanf("%d", &reqs);
	/*	Uma tarefa só pode tem como requisito "v" tarefas acima dela,
	*	estando a tarefa atual na posição "v + 1".
	*/
	while ((reqs < 0) || (reqs >= v + 1))
	{
		printf("(!) Numero invalido ou excede o total de dependecias possivel. Entre outro valor: ");
		scanf("%d", &reqs);
	} /*while*/

	/*Análise dos casos para "reqs" válido.*/
	if (reqs == G->array[v]->reqs) /*Caso 1 - Sem alteração.*/
	{
		printf("> Nada alterado.\n");
		return reqs;
	} else if (reqs == 0) /*Caso 2 - Retirar todas dependências.*/
	{
		printf("> Todas dependencias foram retiradas.\n");
		for (i = 0; i < G->array[v]->reqs; i++) 
		{
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);
			DIGRAPHremoveE(G, EDGE(w, v, G->array[v]->id));
		} /*for*/
		free(G->array[v]->reqs_id);
		G->array[v]->reqs_id = NULL;
	} else if (reqs < G->array[v]->reqs) /*Caso 3 - Número menor de dependências em relação ao atual.*/
	{
		printf("> %d tarefa(s) sera(ao) removida(s).\nDependencias de %s:", G->array[v]->reqs - reqs, G->array[v]->name);
		for(i = 0; i < G->array[v]->reqs; i++) 
		{
			printf(" %d", G->array[v]->reqs_id[i]);
		}
		printf("\n");
		for (i = 0; i < G->array[v]->reqs - reqs; i++) //Executa "reqs_atual - reqs_novo" vezes.*/
		{				
			printf("> Entre a ID a ser removida: ");
			scanf("%d", &id);
			//"reqs - i", porque G->array[v]->reqs só é atualizado ao final da função.*/
			j = FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs - i, id);
			while (j == -1) 
			{
				printf("(!) ID invalida. Entre com outra ID: ");
				scanf("%d", &id);
				j = FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs, id);
			} /*while*/
			/*Remover arestas w->v ("tarefa que nossa tarefa depende" -> "nossa tarefa")*/
			w = VERTEXreturn(G, G->array[v]->reqs_id[j]);
			DIGRAPHremoveE(G, EDGE(w, v, G->array[v]->id));				//Remove aresta tarefa -> tarefa dependente.*/
			
			/*Remover a tarefa à qual dependemos do array de IDs*/
			for (k = j; k < G->array[v]->reqs-1; k++) //Coloca as posições a serem removidas no final do array.*/
			{					
				//Array será realocado ao final após o "for" atual, então estas posições serão removidas.*/
				G->array[v]->reqs_id[k] = G->array[v]->reqs_id[k+1];
			} /*for*/
		}
		//Realoca array de IDs que a tarefa depende.*/
		G->array[v]->reqs_id = (int*)realloc(G->array[v]->reqs_id, reqs * sizeof(int));
		assert(G->array[v]->reqs_id);
	} else 	/*Caso 4 - Número maior de dependências em relação ao atual*/
	{
		printf("> %d tarefa(s) sera(ao) inseridas(s).\n", reqs - G->array[v]->reqs);
		/*Impressão das dependências. Caso não existam, imprime aviso.*/
		if (G->array[v]->reqs_id != NULL)
		{
			printf("Dependencias de %s:", G->array[v]->name);
			for (i = 0; i < G->array[v]->reqs; i++) 
			{
				printf(" %d", G->array[v]->reqs_id[i]);
			} /*for*/
		} else 
		{
			printf("%s nao possui dependencias.", G->array[v]->name);
		} /*if*/
		//Extensão do array de IDs.*/
		G->array[v]->reqs_id = (int*)realloc(G->array[v]->reqs_id, reqs * sizeof(int));
		assert(G->array[v]->reqs_id);
		printf("\n");
		//Pede por novas IDs para a LinkedList de  "reqs_novo - reqs_atual" vezes.*/
		for (i = 0; i < reqs - G->array[v]->reqs; i++) 
		{
			printf("> Entre a ID a ser inserida: ");
			scanf("%d", &id);
			w = VERTEXreturn(G, id);
			//reqs+i: G->array[v]->reqs só é atualizada ao final do programa.*/
			while ((w == -1) || (w >= v) || (FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs + i, id) != -1)) {
				printf("(!) ID invalida. Entre com outra ID: ");
				scanf("%d", &id);
				w = VERTEXreturn(G, id);
			} /*while*/
			/*Insere o ID da tarefa atual à LinkedList de adjacência da tarefa em w.*/
			DIGRAPHinsertE(G, EDGE(w, v, G->array[v]->id));
			//Adiciona o ID ao array de dependências.*/
			G->array[v]->reqs_id[G->array[v]->reqs + i] = id;
		} /*for*/
	} /*if*/
	return reqs;
}

void NEWreqs_id(Digraph G, Vertex v) 
{
	int old_id, 
		new_id, 
		i;
	Vertex w;

	printf("> Qual dependencia deseja alterar?\n");
	scanf("%d", &old_id);
	i = 0;
	while (G->array[v]->reqs_id[i] != old_id) 
	{
		if (i == G->array[v]->reqs) 
		{
			printf("(!) ID nao encontrada. Escolha uma opcao dentre as dependencias mostradas acima.\nEntre com uma nova ID: ");
			scanf("%d", &old_id);
			i = -1;
		} /*if*/
		i++;
	} /*while*/
	w = VERTEXreturn(G, old_id);
	DIGRAPHremoveE(G, EDGE(w, v, G->array[v]->id));
	printf("> Insira nova ID: ");
	scanf("%d", &new_id);
	if (FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs, new_id) != -1) 
	{
		printf("(!) ID ja existente nas dependencias. Nada alterado.\n");
	} else 
	{
		w = VERTEXreturn(G, new_id);
		while ((w == -1) || (w >= v)) 
		{
			printf("(!) ID invalida. Entre com outra ID: ");
			scanf("%d", &new_id);
			w = VERTEXreturn(G, new_id);
		} /*while*/
		DIGRAPHinsertE(G, EDGE(w, v, G->array[v]->id));
		G->array[v]->reqs_id[i] = new_id;
	} /*if*/
}

void modify(WINDOW* win, Digraph G, int id, int choice) 
{
	//int id, 
	//	option, 
	int	i;
	char exit;
	Vertex 	v, 
			w;

	// printf("> Insira a ID da tarefa a ser modificada: ");
	// scanf("%d", &id);

	v = VERTEXreturn(G, id);
	// while (v == -1) 
	// {
	// 	printf("> ID nao existente. Entre com outro valor: ");
	// 	scanf("%d", &id);
	// 	v = VERTEXreturn(G, id);
	// } /*while*/

	// printf("\n> Qual campo deseja alterar?\n");
	// printf(	"\tID:\t\t\t\t[1]\n"
	// 		"\tNome:\t\t\t\t[2]\n"
	// 		"\tTarefa Executada:\t\t[3]\n"
	// 		"\tDuracao:\t\t\t[4]\n"
	// 		"\tInicio Minimo:\t\t\t[5]\n"
	// 		"\tNumero de Pre-Requisitos:\t[6]\n"
	// 		"\tAlterar Pre-Requisito:\t\t[7]\n"
	// 		"\tSair:\t\t\t\t[-1]\n");
	// scanf("%d", &option);

	// while (option != -1) 
	// {
		switch (choice) 
		{
			case 1:
				wgetch(win);
				mvwprintw(win, 28, 1, "> ID atual: %d", id);
				//G->array[v]->id = NEWid(G, v);
				break;
			case 2:
				mvwprintw(win, 15, 1, "> Nome atual: %s", G->array[v]->name);
				NEWname(G, v);
				break;
			case 3:
				G->array[v]->exec = NEWexec(G->array[v]->exec);
				for (w = 0; w < G->V; w++) {
					TIME(G, w);
				}
				break;
			case 4:
				mvwprintw(win, 15, 1, "> Duracao atual: %d", G->array[v]->duration);
				G->array[v]->duration = NEWduration();
				for (w = 0; w < G->V; w++) 
				{
					TIME(G, w);
				} /*for*/
				break;
			case 5:
				mvwprintw(win, 15, 1, "> Inicio minimo atual: %d", G->array[v]->min_start);
				G->array[v]->min_start = NEWmin_start();
				for (w = 0; w < G->V; w++) 
				{
					TIME(G, w);
				} /*for*/
				break;
			case 6:
				mvwprintw(win, 15, 1, "> Numero de pre-requisitos atual: %d", G->array[v]->reqs);
				if (v == 0)
				{
					mvwprintw(win, 28, 1, "Nao eh possivel inserir dependencias na tarefa inicial.");
				} else 
				{
					G->array[v]->reqs = NEWreqs(G, v);
					for (w = 0; w < G->V; w++) 
					{
						TIME(G, w);
					} /*for*/
				} /*if*/
				break;
			case 7:
				if (G->array[v]->reqs > 0) 
				{
					mvwprintw(win, 15, 1, "> Dependencias atuais:");
					for (i = 0; i < G->array[v]->reqs; i++) 
					{
						wprintw(win, " %d", G->array[v]->reqs_id[i]);
					} /*for*/
					printf("\n");
					NEWreqs_id(G, v);
					for (w = 0; w < G->V; w++) 
					{
						TIME(G, w);
					} /*for*/
				} else 
				{
					mvwprintw(win, 28, 1, "> Esta tarefa nao possui dependencias. Modifique o numero de dependencias antes de adiciona-las.\n");
				} /*if*/
				break;
			// default:
			// 	printf("> Opcao invalida. Return.\n");
			// 	break;
		} /*switch*/

		// printf("> Continuar alterando? [s/n]\n");
		// scanf(" %c", &exit);
		// while (exit != 's' && exit != 'S' && exit != 'n' && exit != 'N') 
		// {
		// 	printf("Opcao invalida, entre s/n: ");
		// 	scanf(" %c", &exit);
		// } /*while*/
		// switch (exit) 
		// {
		// 	case 'S':
		// 	case 's':
		// 		printf("> Qual campo deseja alterar?\n");
		// 		printf(	"\tID:\t\t\t\t[1]\n"
		// 				"\tNome:\t\t\t\t[2]\n"
		// 				"\tTarefa Executada:\t\t[3]\n"
		// 				"\tDuracao:\t\t\t[4]\n"
		// 				"\tInicio Minimo:\t\t\t[5]\n"
		// 				"\tNumero de Pre-Requisitos:\t[6]\n"
		// 				"\tAlterar Pre-Requisito:\t\t[7]\n");
		// 		scanf("%d", &option);
		// 		break;
		// 	case 'N':
		// 	case 'n':
		// 		option = -1;
		// 		break;
		// } /*switch*/
	// } /*while*/
}

void execution(Digraph G, int end) 
{
	bool 	same = true;
	LinkedList 	l1, 		/*Listas de adjacência de dois vértices que serão comparadas.*/
			l2;
	int cycle 	= 0, 	/*Contador de ciclos de execução.*/
		j 		= 0, 	/*Contador do índice do array de caminho de execução "path".*/
		k 		= 0,	/*Contador do índice do array de caminho alternativo "alt".*/
		w 		= 0,	/*Contador de índices que guarda a posição do vertice que possui o campo "time" == "time da tarefa atual".*/
		z		= 0,	/*Contador de índices para fazer buscas em "alt".*/
		cont 	= 0,	/*Contador de índices genérico.*/
		flag 	= 0;	/*Contador que indica o final da rotina de avaliação das tarefas.*/
	Vertex i;			/*Contador que dá o índice do vértice do digrafo.*/
	int * path;			/*Armazena o caminho de execução.*/
	int * alt;			/*Armazena os caminhos alternativos do caminho de execução.*/

	path = (int*)malloc(G->V * sizeof(int));
	assert(path);
	if (G->V < 10) 
	{
		alt = (int*)malloc(9 * sizeof(int)); /*Para G->V menor que 10, "alt" precisará no máximo de 9 posições.*/
	}
	else {
		/*Tamanho de "alt" para G->V >= 10 no pior caso será (G->V/2 - 1) * 3, pois considera-se que será
		* necessário formar pares de IDs + -1 para o fim do par.
		* Exemplo:
		* G->V = 10, então [100][101][102][103][104][105][106][107][108][109]
		* No pior caso, as duplas, ou algo equivalente, (102, 103), (104, 105), (106, 107), (108, 109) serão formadas, então "alt"
		* necessitará de 3 posições para cada dupla (cada ID mais -1 indicando fim da sequência).
		* 4 duplas + 3 separadores = 12 posições.
		* Isso significa: (10/2 - 1) * 3 = 12.
		* Portanto, (G->V/2 - 1) * 3.
		*/
		alt = (int*)malloc(((G->V/2 - 1) * 3) * sizeof(int));
	}
	assert(alt);

	while ((flag != G->V) || (end > -1)) 
	{
		flag = 0;
		mvprintw(43, 80, "Ciclo %d\n", cycle);
		for (i = 0; i < G->V; i++) 
		{	
			/*Verificação de execução e montagem dos caminhos de execução das tarefas.*/
			if ((G->array[i]->time == cycle) && (G->array[i]->exec == false)) 
			{
				G->array[i]->exec 	= true;
				
				path[j] 			= G->array[i]->id;
				j++;

				/*Caminho de execução e caminhos alternativos:
				*	i > 1, pois as tarefas 0 e 1 sempre serão executadas, por dois motivo:
				*		1) A tarefa 0 nunca possui dependências e é necessária a todos as outras tarefas.
				*		2) A tarefa 1, portanto, não concorre com a tarefa 0. Não há caminho alternativo.
				*/
				if (i > 1) 
				{
					/*Caso duas tarefas possuam o tempo de execução igual
					* verifica-se se as suas adjacências são iguais e, se sim,
					* adiciona-os ao array.
					*/
					/*Busca-se pela primeira tarefa cujo tempo de execução é igual à da tarefa em "i".*/
					for (w = i + 1; (w < G->V) && (G->array[i]->time != G->array[w]->time); w++)
					{
						/*Corpo em branco.*/
					} /*for*/
					if (w < G->V)
					{
						if (G->array[i]->time == G->array[w]->time) /*Compara novamente por segurança de alinhamento do índice.*/
						{
							/*Salva as adjacências agora para comparar caso uma seja nula e a outra não logo abaixo.*/
							same 	= true;
							l1 		= G->array[i]->adj;	
							l2 		= G->array[w]->adj;

							/*Verifica se as LinkedLists de adjacência são iguais (as mesmas tarefas dependentes).*/
							for (; l1 != NULL && same == true; l1 = l1->next) 
							{
								same = false;
								for (; l2 != NULL; l2 = l2->next) 
								{
									if (l1->id == l2->id) 
									{
										same = true;
									} /*if*/
								} /*for*/
							} /*for*/

							/*Se l1 for NULL e l2 não, então não são iguais.*/
							if (l1 == NULL && l2 != NULL) 
							{
								same = false;
							} /*if*/

							/*Salva no vetor de caminho alternativo*/
							if (same == true) 
							{
								/*Verificação feita para não acessar posições inválidas do array.*/
								if (k > 0) 
								{
									/*Busca se a ID da tarefa atual já está no array de caminhos alternativos.*/
									for (z = 0; z < k; z++)
									{
										if (alt[z] == G->array[i]->id) {
											break;
										} /*if*/
									} /*for*/
									/*Se z < k, significa que G->array[i]->id já está no array,
									* portanto é necessário rearranjar os elementos do array para
									* inserir G->array[w]->id.
									*/
									if (z < k) 
									{
										for (cont = k; cont >= z + 1; cont--) {
											alt[cont + 1] = alt[cont];
										}
										alt[z + 1] = G->array[w]->id;
										k++;
									} else 
									{
										alt[k] = G->array[i]->id;
										k++;
										alt[k] = G->array[w]->id;
										k++;
										alt[k] = -1;
										k++;
									} /*if*/
								} else {
									alt[k] = G->array[i]->id;
									k++;
									alt[k] = G->array[w]->id;
									k++;
									alt[k] = -1;
									k++;
								} /*if*/
							} /*if*/
						} /*if*/
					} /*if*/
				} /*if*/
			} /*if*/
		} /*for*/
		//printf("Tarefas executadas:\n");
		for (i = 0; i < G->V; i++) 
		{
			if (G->array[i]->exec) 
			{
				//printf("%s\n", G->array[i]->name);
				flag++;
			} /*if*/
		} /*for*/
		if (end > -1) 
		{
			end--;
			flag = 0;
			if (end == -1) 
			{
				flag = G->V;
			} /*if*/
		} /*if*/
		print_instructions(G);
		refresh();
		cycle++;
		getchar();
	}

	mvprintw(50, 1, "\n");
	w = 0;
	alt[k] = -1;
	if (k > 0) {
		mvprintw(50, 1, "Caminhos de execução:");
		for (cont = 0; cont < j; cont++) {
			if (alt[w] == path[cont]) {	/*Verifica se o valor a ser impresso está no vetor de caminho alternativo.*/
				if (w == 0) {			//Se for o primeiro valor do vetor, imprime "(valor", assim no final teremos (valor1 ou valor2 ou ...).*/
					printw(" (%d", path[cont]);
					w++;
				} else if (w == k - 1) 
				{
					printw(" ou %d)", path[cont]);
				} else if (alt[w - 1] == -1) 
				{
					printw(" (%d", path[cont]);
					w++;
				}else if (alt[w + 1] == -1) 
				{
					printw(" ou %d)", path[cont]);
					w = w + 2;
				} else 
				{
					printw(" ou %d", path[cont]);
					w++;
				} /*if*/
			} else 
			{
				printw(" %d", path[cont]);
			} /*if*/
		} /*for*/
		printf("\n");
	} else
	 {
		mvprintw(50, 1, "Caminho de execução:");
		for (cont = 0; cont < j; cont++) 
		{
			printf(" %d", path[cont]);
		} /*for*/
		printf("\n");
	} /*if*/

	free(alt);
	free(path);
}