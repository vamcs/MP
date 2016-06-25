/*Cabeçalho de Definições da Interface Gráfica.*/

/*!	\Brief Interface gráfica do ambiente de execução de tarefas.
*
*	Detailed Esta função é responsável pela definição de uma tela e um ambiente de execução em NCurses,
*	emulando um gerenciador de tarefas. A interface gráfica é responsável pela impressão das tarefas contidas
*	no digrafo G, por inserir, deletar e modificar tarefas, e executar as tarefas até um determinado ciclo ou
*	até que todas sejam executadas.
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
void GUI(Digraph);

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
void print_instructions(Digraph G);