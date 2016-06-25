#ifndef GUI_H
#define GUI_H
#endif
/*Cabeçalho de Definições da Interface Gráfica.*/

/*!	\Brief Interface gráfica do ambiente de execução de tarefas.
*
*	Detailed Esta função é responsável pela definição de uma tela e um ambiente de execução em NCurses,
*	emulando um gerenciador de tarefas. A interface gráfica é responsável pela impressão das tarefas contidas
*	no digrafo G, por inserir, deletar e modificar tarefas, e executar as tarefas até um determinado ciclo ou
*	até que todas sejam executadas.
*
*	Assertivas de Entrada:
*	Digraph G 	- é o digrafo que contém todas as tarefas.
*	
*	Assertivas de Saída:
*	Ambiente de execução e tela em NCurses.
*	São impressos na tela:
*		- Tela principal.
*		- Janela do ambiente de execução.
*		- Janela de inserção de nova tarefa.
*		- Janela de deleção de uma tarefa.
*		- Janela de modificação de uma tarefa.
*		- Opções de execução de tarefas.
*		- Informação sobre o ciclo atual e caminhos de execução.
*
*	Todo input é verificado antes de ser atualizado.
*
*	\param Digraph G 
*	\return
*/
void GUI(Digraph);

/*!	\Brief Função de Impressão das Tarefas no Ambiente de Execução.
*
*	Detailed Esta função imprime todos os campos da tarefas no ambiente de execução, assim como o tempo de execução
*	de cada tarefa.
*
*	Função servidora: modify() e GUI().
*	Função cliente: GUI().
*
*	Assertivas de Entrada:
*	Digraph G 	- é o digrafo que contém todas as tarefas.
*	
*	Assertivas de Saída:
*	Imprime na janela execution_environment todos os campos do digrafo, assim como o tempo de execução. Caso uma tarefa
*	já tenha sido executada, imprime a tarefa em azul.
*
*	\param Digraph G 
*	\return
*/
void print_instructions(Digraph G);