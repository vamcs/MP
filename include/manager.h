/*Cabeçalho de Definições do Gerenciador de Tarefas*/
#ifndef MANAGER_H
#define MANAGER_H
#endif

#include <ncurses.h>

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
*	Funções clientes: modify(), DIGRAPHinsertV() e DIGRAPHremoveV().
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
*	Como não há input humano e as funções são confiáveis, as entradas e saída já estão verificadas.
*
*	\param digrafo G 
*	\param vértice da tarefa atual v
*	\return
*/
void TIME(Digraph, Vertex);

/*!	\Brief Menu que dá opções para alterar os campos da tarefa.
*
*	Detailed A função modify recebe uma ID tratada e entrada pelo usuário e oferece opções de modificação
*	de cada um dos campos da tarefa. Sendo estes ID, nome, exec, duração, início mínimo, número de pré-re-
*	quisitos e os pré-requisitos em si, caso existam. Após maior parte das modificações, tratamentos da
*	estrutura do digrafo devem ser feitos como alterar, adicionar ou remover arestas ou realocar o array 
*	de pré-requisitos. Além disso, caso seja necessário, o tempo de execução das tarefas será recalculado
*	caso for necessário. A função imprime diretamente na tela "modification_window" da GUI.
*
*	Função servidora: TIME() e dependências em manager.cpp.
*	Função cliente: GUI().
*
*	Assertivas de Entrada:
*	WINDOW* win - ponteiro que contém uma janela ncurses y = 30, x = 50.
*	Digraph G 	- é o digrafo que contém todas as tarefas.
*	int id 		- id da tarefa a ser modificada.
*	int choice 	- escolha de qual modificação será executada. A função modification_window é responsável por este valor.
*	
*	Assertivas de Saída:
*	Imprime na janela win informações e os valores alterados, além de efetuar todas as modificações no digrafo descritas e
*	detalhadas na descrição de cada uma das funções em manager.cpp.
*
*	Todo input é verificado antes de ser atualizado.
*
*	\param WINDOW* win
*	\param Digraph G 
*	\param int id
*	\param int choice
*	\return
*/
void modify(WINDOW*, Digraph, int, int);

/*!	\Brief Função de execução das tarefas e computação dos caminhos mínimos de execução.
*
*	Detailed A função execution verifica quais tarefas podem ser executadas em um determinado ciclo e salva o caminho de
*	execução das tarefas, assim como as alternativas que possam surgir. Caso duas tarefas forem executadas durante o mesmo
*	ciclo e possuam as mesmas tarefas que dependem delas em sua lista de adjacência, elas entraram em um array de caminho
*	alternativo, para que se possa imprimir todas as opções ao final. É também possível decidir a até qual ciclo a execução irá
*	parar, independentemente de todas as tarefas terem sido executadas ou não. As saídas da função são impressas em uma tela
*	definida pela ncurses na GUI().
*
*	Funções servidoras: VERTEXreturn(), print_instructions().
*	Função cliente: GUI().
*
*	Assertivas de Entrada:
*	Digraph G 	- é o digrafo que contém todas as tarefas.
*	int end 	- valor inteiro que define o ciclo de parada. Caso seja -1 executa até que todas as tarefas tenham sido executadas.
*	
*	Assertivas de Saída:
*	Imprime na tela NCurses o ciclo de execução atual, o caminho de execução e as possibilidades alternativas, assim como o tempo de
*	execução mínima até o presente ciclo.
*
*	As opções são, por exemplo:
*		Caso não haja caminho alternativo:
*			Caminho de Execução: 100 101 103 102 103
*		Caso haja caminhos alternativos:
*			Caminhos de Execução: 100 (102 ou 103 ou 101) 104 (105 ou 106) 107
*
*	Todo input é verificado antes de ser atualizado.
*
*	\param Digraph G 
*	\param int end
*	\return
*/
void execution(Digraph, int);