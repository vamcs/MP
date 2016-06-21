#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

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
void TIME(Digraph G, Vertex v) {

	/*Caso não haja dependências*/
	if (G->array[v]->reqs == 0) {
		G->array[v]->time = G->array[v]->min_start + G->array[v]->duration;	//time = min_start + duration
		return;
	}

	else {
		int i, longest_time = 0;
		Vertex w;
		for(i = 0; i < G->array[v]->reqs; i++) {
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);				//Encontra o vértice da dependência reqs_id[i]
			if(G->array[w]->time > longest_time && !G->array[w]->exec)	//Se a instrução já tiver sido executada, seu tempo de executação não é levado em conta.
				longest_time = G->array[w]->time;						//Procura pela dependência de maior duração
		}
		if(G->array[v]->min_start < longest_time) {						//Se min_start < longest_time
			G->array[v]->time = longest_time + G->array[v]->duration;	//time = longest_time + duration
		}

		else{		//Se min_start >= longest_time
			G->array[v]->time = G->array[v]->min_start + G->array[v]->duration;		//time = min_start + duration
		}
		return;
	}
}

int NEWid(Digraph G, Vertex v) {
	int new_id, i;
	link l;
	Vertex w;

	//Leitura do novo ID
	printf("> Entre com o novo ID: ");
	scanf("%d", &new_id);
	w = VERTEXreturn(G, new_id);
	while (w != -1 || w < -2) {
		printf("(!) ID invalida ou ja existente. Entre com outro valor: ");
		scanf("%d", &new_id);
		w = VERTEXreturn(G, new_id);
	}

	//Atualização do vetor de IDs das tarefas que dependem desta tarefa em "v"
	l = G->array[v]->adj;
	while (l != NULL) {
		i = FINDreqs_id(G->array[l->w]->reqs_id, G->array[l->w]->reqs, G->array[v]->id);
		G->array[l->w]->reqs_id[i] = new_id;
		l = l->next;
	}

	//Atualização da lista de adjacência das tarefas que "v" depende
	for (i = 0; i < G->array[v]->reqs; i++) {
		w = VERTEXreturn(G, G->array[v]->reqs_id[i]);
		l = G->array[w]->adj;
		while (l->next != NULL && l->w != v) {
			l = l->next;
		}
		if (l->id == G->array[v]->id)
			l->id = new_id;
	}

	return new_id;
}

void NEWname(Digraph G, Vertex v) {
	Vertex w;
	char name[100];
	printf("> Entre com o novo nome (ate 100 caracteres): ");
	getchar();
	scanf("%99[^\n]s", name);
	for (w = 0; w < G->V-1; w++) {
		if (strcmp(name, G->array[w]->name) == 0) {
			printf("(!) Nome ja existente. Entre com outra string (ate 100 caracteres): ");
			scanf("%99s", name);
			w = -1;
		}
	}
	strcpy(G->array[v]->name, name);
}

bool NEWexec(bool exec) {
	char option;
	if(exec == true)
		printf("> A tarefa ja foi executada.\n");
	else
		printf("> A tarefa ainda nao foi executada.\n");
	printf("> Inverter o estado? s/n\n");
	scanf (" %c", &option);
	while(option != 's' && option != 'S' && option != 'n' && option != 'N') {
		printf("(!) Opcao inexistente. Entre [s/n]: ");
		scanf(" %c", &option);
	}
	switch (option) {
		case 'S':
		case 's':
			exec = !exec;
			break;
		case 'N':
		case 'n':
			break;
		default:
			break;
	}
	return exec;
}

int NEWduration() {
	int duration;
	printf("> Entre com a nova duracao: ");
	scanf("%d", &duration);
	while (duration < 0) {
		printf("(!) Duracao invalida. Entre outro valor: ");
		scanf("%d", &duration);
	}
	return duration;
}

int NEWmin_start() {
	int min_start;
	printf("> Entre com o novo inicio minimo: ");
	scanf("%d", &min_start);
	while (min_start < 0) {
		printf("(!) Inicio invalido. Entre outro valor: ");
		scanf("%d", &min_start);
	}
	return min_start;
}

int NEWreqs(Digraph G, Vertex v) {
	int reqs, i, j, k, id;
	Vertex w;
	printf("> Entre com o novo numero de pre-requisitos: ");
	scanf("%d", &reqs);
	while (reqs < 0 || reqs >= v + 1) {
		printf("(!) Numero invalido ou excede o total de dependecias possivel. Entre outro valor: ");
		scanf("%d", &reqs);
	}
	if (reqs == G->array[v]->reqs) {
		printf("> Nada alterado.\n");
		return reqs;
	}
	else if (reqs == 0) {
		printf("> Todas dependencias foram retiradas.\n");
		for (i = 0; i < G->array[v]->reqs; i++) {
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);
			DIGRAPHremoveE(G, EDGE(w, v, G->array[v]->id));
		}
		free(G->array[v]->reqs_id);
		G->array[v]->reqs_id = NULL;
	}
	else if (reqs < G->array[v]->reqs) {
		printf("> %d tarefa(s) sera(ao) removida(s).\nDependencias de %s:", G->array[v]->reqs - reqs, G->array[v]->name);
		for(i = 0; i < G->array[v]->reqs; i++) {
			printf(" %d", G->array[v]->reqs_id[i]);
		}
		printf("\n");
		for (i = 0; i < G->array[v]->reqs - reqs; i++) {				//Executa "reqs_atual - reqs_novo" vezes
			printf("> Entre a ID a ser removida: ");
			scanf("%d", &id);
			j = FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs - i, id);		// reqs - i: porque G->array[v]->reqs só é atualizado ao final da função.
			while (j == -1) {
				printf("(!) ID invalida. Entre com outra ID: ");
				scanf("%d", &id);
				j = FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs, id);
			}
			/*Remover arestas w->v (tarefa que nossa tarefa depende -> nossa tarefa)*/
			w = VERTEXreturn(G, G->array[v]->reqs_id[j]);
			DIGRAPHremoveE(G, EDGE(w, v, G->array[v]->id));				//Remove aresta tarefa -> tarefa dependente
			
			/*Remover a tarefa à qual dependemos do array de IDs*/
			for(k = j; k < G->array[v]->reqs-1; k++) {					//Coloca as posições a serem removidas no final do array
				G->array[v]->reqs_id[k] = G->array[v]->reqs_id[k+1];	//Array será realocado ao final após o "for" atual, então estas posições serão removidas.
			}
		}
		G->array[v]->reqs_id = (int*)realloc(G->array[v]->reqs_id, reqs * sizeof(int));		//Realoca array de IDs que a tarefa depende.
	}
	else {		//novo reqs > reqs atual
		printf("> %d tarefa(s) sera(ao) inseridas(s).\n", reqs - G->array[v]->reqs);
		if (G->array[v]->reqs_id != NULL) {
			printf("Dependencias de %s:", G->array[v]->name);
			for(i = 0; i < G->array[v]->reqs; i++) {
				printf(" %d", G->array[v]->reqs_id[i]);
			}
		}
		else {
			printf("%s nao possui dependencias.", G->array[v]->name);
		}
		G->array[v]->reqs_id = (int*)realloc(G->array[v]->reqs_id, reqs * sizeof(int));		//Extensão do array de IDs.
		printf("\n");
		for (i = 0; i < reqs - G->array[v]->reqs; i++) {				//Executa "reqs_novo - reqs_atual" vezes
			printf("> Entre a ID a ser inserida: ");
			scanf("%d", &id);
			w = VERTEXreturn(G, id);
			while (w == -1 || w >= v || FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs + i, id) != -1) {	//reqs+i: G->array[v]->reqs só é atualizada ao final do programa.
				printf("(!) ID invalida. Entre com outra ID: ");
				scanf("%d", &id);
				w = VERTEXreturn(G, id);
			}
			DIGRAPHinsertE(G, EDGE(w, v, G->array[v]->id));
			G->array[v]->reqs_id[G->array[v]->reqs + i] = id;		//i começa em 0, então +1 é necessário. Adiciona o ID ao array de dependências.
		}
	}
	return reqs;
}

void NEWreqs_id(Digraph G, Vertex v) {
	int old_id, new_id, i;
	Vertex w;

	printf("> Qual dependencia deseja alterar?\n");
	scanf("%d", &old_id);
	i = 0;
	while (G->array[v]->reqs_id[i] != old_id) {
		if(i == G->array[v]->reqs) {
			printf("(!) ID nao encontrada. Escolha uma opcao dentre as dependencias mostradas acima.\nEntre com uma nova ID: ");
			scanf("%d", &old_id);
			i = -1;
		}
		i++;
	}
	w = VERTEXreturn(G, old_id);
	DIGRAPHremoveE(G, EDGE(w, v, G->array[v]->id));
	printf("> Insira nova ID: ");
	scanf("%d", &new_id);
	if (FINDreqs_id(G->array[v]->reqs_id, G->array[v]->reqs, new_id) != -1) {
		printf("(!) ID ja existente nas dependencias. Nada alterado.\n");
	}
	else {
		w = VERTEXreturn(G, new_id);
		while (w == -1 || w >= v) {
			printf("(!) ID invalida. Entre com outra ID: ");
			scanf("%d", &new_id);
			w = VERTEXreturn(G, new_id);
		}
		DIGRAPHinsertE(G, EDGE(w, v, G->array[v]->id));
		G->array[v]->reqs_id[i] = new_id;
	}
}

void modify(Digraph G) {
	int id, option, i;
	char sair;
	Vertex v, w;

	printf("> Insira a ID da tarefa a ser modificada: ");
	scanf("%d", &id);

	v = VERTEXreturn(G, id);
	while (v == -1) {
		printf("> ID nao existente. Entre com outro valor: ");
		scanf("%d", &id);
		v = VERTEXreturn(G, id);
	}

	printf("\n> Qual campo deseja alterar?\n");
	printf("\tID:\t\t\t\t[1]\n\tNome:\t\t\t\t[2]\n\tTarefa Executada:\t\t[3]\n\tDuracao:\t\t\t[4]\n\tInicio Minimo:\t\t\t[5]\n\tNumero de Pre-Requisitos:\t[6]\n\tAlterar Pre-Requisito:\t\t[7]\n\tSair:\t\t\t\t[-1]\n");
	scanf("%d", &option);

	while (option != -1) {

		switch (option) {
			case 1:
				printf("> ID atual: %d\n", id);
				G->array[v]->id = NEWid(G, v);
				break;
			case 2:
				printf("> Nome atual: %s\n", G->array[v]->name);
				NEWname(G, v);
				break;
			case 3:
				G->array[v]->exec = NEWexec(G->array[v]->exec);
				for (w = 0; w < G->V; w++) {
					TIME(G, w);
				}
				break;
			case 4:
				printf("> Duracao atual: %d\n", G->array[v]->duration);
				G->array[v]->duration = NEWduration();
				for (w = 0; w < G->V; w++) {
					TIME(G, w);
				}
				break;
			case 5:
				printf("> Inicio minimo atual: %d\n", G->array[v]->min_start);
				G->array[v]->min_start = NEWmin_start();
				for (w = 0; w < G->V; w++) {
					TIME(G, w);
				}
				break;
			case 6:
				printf("> Numero de pre-requisitos atual: %d\n", G->array[v]->reqs);
				if (v == 0){
					printf("Nao eh possivel inserir dependencias na tarefa inicial.\n");
				}
				else {
					G->array[v]->reqs = NEWreqs(G, v);
					for (w = 0; w < G->V; w++) {
						TIME(G, w);
					}
				}
				break;
			case 7:
				if (G->array[v]->reqs > 0) {
					printf("> Dependencias atuais:");
					for (i = 0; i < G->array[v]->reqs; i++)
						printf(" %d", G->array[v]->reqs_id[i]);
					printf("\n");
					NEWreqs_id(G, v);
					for (w = 0; w < G->V; w++) {
						TIME(G, w);
					}
				}
				else {
					printf("> Esta tarefa nao possui dependencias. Modifique o numero de dependencias antes de adiciona-las.\n");
				}
				break;
			default:
				printf("> Opcao invalida. Return.\n");
				break;
		}

		printf("> Continuar alterando? [s/n]\n");
		scanf(" %c", &sair);
		while (sair != 's' && sair != 'S' && sair != 'n' && sair != 'N') {
			printf("Opcao invalida, entre s/n: ");
			scanf(" %c", &sair);
		}
		switch (sair) {
			case 'S':
			case 's':
				printf("> Qual campo deseja alterar?\n");
				printf("\tID:\t\t\t\t[1]\n\tNome:\t\t\t\t[2]\n\tTarefa Executada:\t\t[3]\n\tDuracao:\t\t\t[4]\n\tInicio Minimo:\t\t\t[5]\n\tNumero de Pre-Requisitos:\t[6]\n\tAlterar Pre-Requisito:\t\t[7]\n");
				scanf("%d", &option);
				break;
			case 'N':
			case 'n':
				option = -1;
				break;
		}
	}
}

void execution(Digraph G, int end) {
	bool flag = false;
	int cycle = 0;
	Vertex i;
	while (flag == false || end > -1) {
		printf("\nCiclo Atual: %d\n", cycle);
		for (i = 0; i < G->V; i++) {
			if (G->array[i]->time == cycle && G->array[i]->exec == false) {
				G->array[i]->exec = true;
				flag = true;
			}
			else if (G->array[i]->time != cycle && G->array[i]->exec == false) {
				flag = false;
			}
		}
		printf("Tarefas executadas:\n");
		for (i = 0; i < G->V; i++) {
			if (G->array[i]->exec) {
				printf("%s\n", G->array[i]->name);
			}
		}
		if (end > -1) {
			end--;
			if (end == -1) {
				flag = true;
			}
		}
		getchar();
		cycle++;
	}
}