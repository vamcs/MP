#ifndef MANAGER_H
#define MANAGER_H
#endif

#include <ncurses.h>
/*Cabeçalho de Definições do Gerenciador de Tarefas*/

/*!	\Brief Calcula o tempo de execução de uma tarefa
*
*	Detailed Funcão TIME que calcula o tempo necessário para executar uma tarefa em "v".
*	Esta função começa o cálculo a partir de tarefas que não possuem dependências,
*	pois o cálculo é trivial: o ciclo mínimo de início + duração da tarefa. Nos
*	outros casos, considera-se que todas as dependências estejam acima da tarefa
*	atual, o que significa que seus tempos já foram calculados. Dessa forma, existem
*	dois casos: o ciclo de início < tempo mais longo das dependências e o
*	caso em que o ciclo de início é maior. Na primeira situação, o tempo da tarefa atual
*	será o tempo de executar a dependência mais demorada + tempo de duração da
*	tarefa atual, já que o ciclo de início da tarefa já terá passado. Já no se-
*	gundo caso, como o tempo de início da tarefa > tempo das dependências, pode-
*	-se ignorar o tempo de execução das dependências e somar o ciclo de início
*	da tarefa à sua duração.
*
*	Assertivas de Entrada:
*	Digraph G - é o digrafo que contém todas as tarefas cujos tempos de execução serão calculados.
*	Vertex v - é o índice do vértice do digrafo que	contém a tarefa cujo tempo de execução será calculado.
*	
*	Assertivas de Saída:
*	reqs == 0:	(tarefa sem dependências)
*		time = min_start + duration
*
*	reqs > 0:	(tarefa com pelo menos uma dependência)
*		min_start < longest_time (tempo mais longo de todas dependências)
*			time = longest_time + duration
*		min_start >= longest_time
*			time = min_start + duration
*
*	Os módulos clientes também são servidores e estes são DIGRAPHinit() e DIGRAPHinsertV().
*	Como não há input humano e as funções são confiáveis, as entradas e saída já estão verificadas.
*
*	\param digrafo G 
*	\param vértice da tarefa atual v
*	\return
*/
void TIME(Digraph, Vertex);
void modify(WINDOW*, Digraph, int, int);
void execution(Digraph, int);