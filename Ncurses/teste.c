#include <ncurses.h>

#define WORLD_WIDTH 100  //definir altura e largura da "janela"
#define WORLD_HEIGHT 40

WINDOW* execution_environment;

const char *choices[] = 
{
	"1 - Adicionar tarefa.", 
	"2 - Editar tarefa.", 
	"3 - Remover tarefa.", 
	"4 - Escolher ciclo.", 
	"5 - Executar."
};
int n_choices = sizeof(choices)/sizeof(char*);

const char *fields[] = 
{
	"[ID]",
	"[Nome]",
	"[Exec]",
	"[Duracao]",
	"[Inicio]",
	"[Pre-requisitos]"
};
int n_fields = sizeof(fields)/sizeof(char*);

void destroy_win(WINDOW *local_win, int Y, int X)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	int i, j;
	for (j = 1; j < Y - 1; j++) {
		for (i = 1; i < X - 1; i++) {
			mvwaddch(local_win, j, i, ' ');
		}
	}
	wrefresh(local_win);
	delwin(local_win);
}

void print_menu(int highlight) 
{
	int x, y, i;  

	x = 1;
	y = 44;
	for(i = 0; i < n_choices; ++i)
	{  
		if(highlight == i + 1){ /* High light the present choice */  
			attron(A_REVERSE); 
			mvprintw(y, x, "%s", choices[i]);
			attroff(A_REVERSE);
		} else
			mvprintw(y, x, "%s", choices[i]);
			++y;
	}
	refresh();
}

void print_instruction(WINDOW *win, int ID, int exec) 
{
	if (exec == 1) {
		init_pair(1, COLOR_CYAN, COLOR_BLACK);
		wattron(win, COLOR_PAIR(1));
	}
		mvwprintw(win, 2, 5, "%d", ID);
		mvwprintw(win, 2, 15, "TR0");
	  	mvwprintw(win, 2, 27, "%d", exec);
	  	mvwprintw(win, 2, 40, "%d", 2);
	  	mvwprintw(win, 2, 55, "%d", 0);
	  	mvwprintw(win, 2, 70, "-");

	if (exec == 1) {
	  	wattroff(win, COLOR_PAIR(1));
	}

  	wrefresh(win);
}

void print_modify(WINDOW* win, int highlight) 
{
	int x, y, i;  

	x = 1;
	y = 2;
	for(i = 0; i < n_fields; ++i)
	{  
		if(highlight == i + 1){ /* High light the present choice */  
			wattron(win, A_REVERSE); 
			mvwprintw(win, y, x, "%s", fields[i]);
			wattroff(win, A_REVERSE);
		} else
			mvwprintw(win, y, x, "%s", fields[i]);
			++y;
	}
	wrefresh(win);
}

void insertion_window() 
{
	WINDOW *win = newwin(20, 40, 3, 101);
	keypad(win, TRUE);
	box(win, 0, 0);
	touchwin(win);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	mvwprintw(win, 1, 1, "Inserindo nova tarefa:");
	wattroff(win, COLOR_PAIR(1));
	mvwprintw(win, 3, 1, "[ID]: TR0");
	mvwprintw(win, 4, 1, "[Nome]: ");
	mvwprintw(win, 5, 1, "[Exec]: ");
	mvwprintw(win, 6, 1, "[Duracao]: ");
	mvwprintw(win, 7, 1, "[Inicio]: ");
	mvwprintw(win, 8, 1, "[Pre-requisitos]: ");
	mvwprintw(win, 12, 1, "Erro, entre novamente: TR0");
	mvwprintw(win, 13, 1, "Erro, entre novamente: TR0");
	mvwprintw(win, 14, 1, "Erro, entre novamente: TR0");
	mvwprintw(win, 15, 1, "Erro, entre novamente: TR0");
	mvwprintw(win, 16, 1, "Erro, entre novamente: TR0");
	mvwprintw(win, 17, 1, "Erro, entre novamente: TR0");
	mvwprintw(win, 18, 1, "Erro, entre novamente: TR0");
	wrefresh(win);
	getchar();
	destroy_win(win, 20, 50);
}

void modify_window() 
{
	WINDOW *win = newwin(20, 40, 3, 101);
	keypad(win, TRUE);
	box(win, 0, 0);
	touchwin(win);
	static int 	highlight 	= 1,
 				c 			= 0,
 				choice 		= 0;

 	mvwprintw(win, 1, 1, "Insira ID a ser modificada: ");
 	mvwprintw(win, 10, 1, "Erro, insira novamente: ");
 	print_modify(win, highlight);
 	while (1)
 	{  
    	c = getch();
    	switch(c)
    	{  
    		case KEY_UP:
    			if(highlight == 1)
    			{
    				highlight = n_fields;
    			} else
    			{
    				--highlight;
    			}
    			break;
    		case KEY_DOWN:
			    if(highlight == n_fields)
			    highlight = 1;
			    else 
			    ++highlight;
			    break;
    		case 10:
			    choice = highlight;
			    break;
			default:
			    break;
    	} /*switch*/
	    print_modify(win, highlight);
	    if(choice != 0) /* User did a choice come out of the infinite loop */
	    {
	    	break;
	    } /*if*/
	} /*while*/
	getchar();
	destroy_win(win, 20, 50);
}

void setupGUI() {
	int i,
		highlight = 1;

	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	for (i = 0; i < 100; i++) 
	{
		mvaddch(0, i + 1, '-');
	}
	mvprintw(1, 1, "|");
  	mvprintw(1, 100, "|");
 	mvprintw(1, 40, "Projeto Final");
	for (i = 0; i < 100; i++) 
	{
		mvaddch(2, i + 1, '-');
	}
	refresh();

	execution_environment = newwin(WORLD_HEIGHT,WORLD_WIDTH,3,1);
	box(execution_environment, 0, 0);
	mvwprintw(execution_environment, 1, 5, "ID");
  	mvwprintw(execution_environment, 1, 15, "NOME");
  	mvwprintw(execution_environment, 1, 27, "EXECUTADA");
  	mvwprintw(execution_environment, 1, 40, "DURACAO");
  	mvwprintw(execution_environment, 1, 55, "INICIO");
  	mvwprintw(execution_environment, 1, 70, "PRE-REQUISITOS");
	touchwin(execution_environment);
  	wrefresh(execution_environment);

  	mvprintw(43, 80, "Ciclo 0");
  	mvprintw(43, 1, "Menu:");
 	print_menu(highlight);
}

int main() {
	static int 	highlight 	= 1,
 				c 			= 0,
 				choice 		= 0;

	setupGUI();
 	refresh();

  	print_instruction(execution_environment, 100, 0);
 	wrefresh(execution_environment);

 	while (1)
 	{  
    	c = getch();
    	switch(c)
    	{  
    		case KEY_UP:
    			if(highlight == 1)
    			{
    				highlight = n_choices;
    			} else
    			{
    				--highlight;
    			}
    			break;
    		case KEY_DOWN:
			    if(highlight == n_choices)
			    highlight = 1;
			    else 
			    ++highlight;
			    break;
    		case 10:
			    choice = highlight;
			    break;
			default:
			    break;
    	} /*switch*/
	    print_menu(highlight);
	    if(choice != 0) /* User did a choice come out of the infinite loop */
	    {
	    	break;
	    } /*if*/
	} /*while*/

	if (choice == 1) {
		insertion_window();
	} else if (choice == 2) 
	{
		modify_window();
	}
	else if (choice == 5) 
	{
		mvprintw(43, 80, "Ciclo 2");
		print_instruction(execution_environment, 100, 1);
	}

 	refresh();

	getchar();
	delwin(execution_environment);
	endwin();
}