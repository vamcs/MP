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
			w = VERTEXreturn(G, G->array[v]->reqs_id[i]);	//Encontra o vértice da dependência reqs_id[i]
			if(G->array[w]->time > longest_time)
				longest_time = G->array[w]->time;			//Procura pela dependência de maior duração
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