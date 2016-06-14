#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Digraph.h"
#include "manager.h"

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
void TIME(Digraph G, Vertex v){

	/*Caso não haja dependências*/
	if (G->array[v]->reqs == 0){
		G->array[v]->time = G->array[v]->min_start + G->array[v]->duration;	//time = min_start + duration
		return;
	}

	else {
		int i, longest_time = 0;
		Vertex w;
		for(i = 0; i < G->array[v]->reqs; i++){
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);				//Encontra o vértice da dependência reqs_id[i]
			if(G->array[w]->time > longest_time && !G->array[w]->exec)	//Se a instrução já tiver sido executada, seu tempo de executação não é levado em conta.
				longest_time = G->array[w]->time;						//Procura pela dependência de maior duração
		}
		if(G->array[v]->min_start < longest_time){						//Se min_start < longest_time
			G->array[v]->time = longest_time + G->array[v]->duration;	//time = longest_time + duration
		}

		else{		//Se min_start >= longest_time
			G->array[v]->time = G->array[v]->min_start + G->array[v]->duration;		//time = min_start + duration
		}
		return;
	}
}

void modify(Digraph G){
	int id, new_id, option, duration, min_start, reqs, reqs_id, i;
	char exec, sair, name[100];
	Vertex v, w;

	printf("> Insira a ID da tarefa a ser modificada: ");
	scanf("%d", &id);

	v = VERTEXreturn(G, id);
	while (v == -1){
		printf("> ID nao existente. Entre com outro valor: ");
		scanf("%d", &id);
		v = VERTEXreturn(G, id);
	}

	printf("\n> Qual campo deseja alterar?\n");
	printf("\tID:\t\t\t\t[1]\n\tNome:\t\t\t\t[2]\n\tTarefa Executada:\t\t[3]\n\tDuracao:\t\t\t[4]\n\tInicio Minimo:\t\t\t[5]\n\tNumero de Pre-Requisitos:\t[6]\n\tDependencias:\t\t\t[7]\n\tSair:\t\t\t\t[-1]\n");
	scanf("%d", &option);

	while (option != -1){

		switch (option){
			case 1:
				printf("> ID atual: %d\n", id);
				printf("> Entre com o novo ID: ");
				scanf("%d", &new_id);
				w = VERTEXreturn(G, new_id);
				while (w != -1 || w < -2) {
					printf("(!) ID invalida ou ja existente. Entre com outro valor: ");
					scanf("%d", &new_id);
					w = VERTEXreturn(G, new_id);
				}
				G->array[v]->id = new_id;
				break;
			case 2:
				printf("> Nome atual: %s\n", G->array[v]->name);
				printf("> Entre com o novo nome (ate 100 caracteres): ");
				scanf("%100s", name);
				for (w = 0; w < G->V-1; w++){
					if (strcmp(name, G->array[w]->name) == 0){
						printf("> Nome ja existente. Entre com outra string (ate 100 caracteres): ");
						scanf("%100s", name);
						w = -1;
					}
				}
				strcpy(G->array[v]->name, name);
				break;
			case 3:
				if(G->array[v]->exec == true)
					printf("> A tarefa ja foi executada.\n");
				else
					printf("> A tarefa ainda nao foi executada.\n");
				printf("> Inverter o estado? s/n\n");
				scanf(" %c", &exec);
				switch (exec){
					case 'S':
					case 's':
						G->array[v]->exec = !G->array[v]->exec;
						break;
					case 'N':
					case 'n':
						break;
					default:
						break;
				}
				break;
			case 4:
				printf("> Duracao atual: %d\n", G->array[v]->duration);
				printf("> Entre com a nova duracao: ");
				scanf("%d", &duration);
				while (duration < 0){
					printf("> Duracao invalida. Entre outro valor: ");
					scanf("%d", &duration);
				}
				G->array[v]->duration = duration;
				break;
			case 5:
				printf("> Inicio minimo atual: %d\n", G->array[v]->min_start);
				printf("> Entre com o novo inicio minimo: ");
				scanf("%d", &min_start);
				while (min_start < 0){
					printf("> Inicio invalido. Entre outro valor: ");
					scanf("%d", &min_start);
				}
				G->array[v]->min_start = min_start;
				break;
			case 6:
				printf("> Numero de pre-requisitos atual: %d\n", G->array[v]->reqs);
				printf("> Entre com o novo numero de pre-requisitos: ");
				scanf("%d", &reqs);
				while (reqs < 0 || reqs >= G->array[v]->reqs){
					printf("> Numero invalido. Entre outro valor: ");
					scanf("%d", &reqs);
				}
				G->array[v]->reqs = reqs;
				break;
			case 7:
				if (G->array[v]->reqs > 0) {
					printf("> Dependencias atuais:");
					for (i = 0; i < G->array[v]->reqs; i++)
						printf(" %d", G->array[v]->reqs_id[i]);
					printf("\n");
					printf("> Qual dependencia deseja alterar?");
					scanf("%d", &reqs_id);
					i = 0;
					while (G->array[v]->reqs_id[i] != reqs_id){
						i++;
						if(i == G->array[v]->reqs){
							printf("> ID nao encontrada. Escolha uma opcao dentre ");
							scanf("%d", &reqs_id);
							i = 0;
						}
					}
					G->array[v]->reqs_id[i] = reqs_id;
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
		while (sair != 's' && sair != 'S' && sair != 'n' && sair != 'N'){
			printf("Opcao invalida, entre s/n: ");
			scanf(" %c", &sair);
		}
		switch (sair){
			case 'S':
			case 's':
				printf("> Qual campo deseja alterar?\n");
				printf("\tID:\t\t\t\t[1]\n\tNome:\t\t\t\t[2]\n\tTarefa Executada:\t\t[3]\n\tDuracao:\t\t\t[4]\n\tInicio Minimo:\t\t\t[5]\n\tNumero de Pre-Requisitos:\t[6]\n\tDependencias:\t\t\t[7]\n");
				scanf("%d", &option);
				break;
			case 'N':
			case 'n':
				option = -1;
				break;
		}
	}
}