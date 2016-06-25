#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../include/Digraph.h"
#include "../include/manager.h"
#include "../include/fileReader.h"
#include "../include/GUI.h"

#define WORLD_WIDTH 100  //definir altura e largura da "janela"
#define WORLD_HEIGHT 40

WINDOW* execution_environment;

const char *choices[] = 
{
	"1 - Adicionar tarefa.", 
	"2 - Editar tarefa.", 
	"3 - Remover tarefa.", 
	"4 - Escolher ciclo.", 
	"5 - Executar.",
	"6 - Sair."
};
int n_choices = sizeof(choices)/sizeof(char*);

const char *fields[] = 
{
	"[ID]",
	"[Nome]",
	"[Exec]",
	"[Duracao]",
	"[Inicio]",
	"[Pre-requisitos]",
	"[Dependências]"
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

void print_instructions(Digraph G) 
{
	Vertex i;
	int k, j;

	for (k = 2; k < 39; k++)
	{
		for (j = 2; j < 99; j++)
		{
			mvwaddch(execution_environment, k, j, ' ');
		}
	}

	for (i = 0; i < G->V; i++)
	{
		if (G->array[i]->exec == true) 
		{
			init_pair(1, COLOR_CYAN, COLOR_BLACK);
			wattron(execution_environment, COLOR_PAIR(1));
		} /*if*/
			mvwprintw(execution_environment, i + 2, 2, "%d", G->array[i]->id);
			mvwprintw(execution_environment, i + 2, 13, "%s", G->array[i]->name);
		  	mvwprintw(execution_environment, i + 2, 25, "%d", G->array[i]->exec);
		  	mvwprintw(execution_environment, i + 2, 38, "%d", G->array[i]->duration);
		  	mvwprintw(execution_environment, i + 2, 53, "%d", G->array[i]->min_start);
		  	mvwprintw(execution_environment, i + 2, 68, "%d", G->array[i]->reqs);
		  	if (G->array[i]->reqs > 0) 
		  	{
		  		for (j = 0; j < G->array[i]->reqs; j++)
		  		{
		  			wprintw(execution_environment, " %d", G->array[i]->reqs_id[j]);
		  		}
		  	} else
		  	{
		  		mvwprintw(execution_environment, i + 2, 68, "-");
			} /*if*/
		if (G->array[i]->exec == true) 
		{
		  	wattroff(execution_environment, COLOR_PAIR(1));
		} /*if*/

		if (G->array[i]->exec) 
		{
			mvwprintw(execution_environment, i + G->V + 5, 2, "Tempo %s:\t%d", G->array[i]->name, G->array[i]->time);
		} else 
		{
			mvwprintw(execution_environment, i + G->V + 5, 2, "Tempo %s:\t-", G->array[i]->name);
		}

	} /*for*/

  	wrefresh(execution_environment);
}

void print_modification(WINDOW* win, int highlight) 
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

void userInsertion(WINDOW *win, Digraph G)
{
	bool (*nameCheck)(Digraph, char*, int);
	nameCheck = NAMEcheck;

	bool (*inputCheck)(int);
	inputCheck = INPUTcheck;

	int length, 
		int_reqs,
		error_check;
	char id[5],
		exec,
		duration[3],
		min_start[3],
		reqs[3];
	char name[100];

	do
	{
		char* str = (char*)malloc(1000 * sizeof(char));

		mvwprintw(win, 3, 1, "[ID]: ");
		wscanw(win, "%5s", id);
		mvwprintw(win, 4, 1, "[Nome]: ");
		wscanw(win, "%100[^\n]s", name);
		//getchar();
		mvwprintw(win, 5, 1, "[Exec]: ");
		wscanw(win, " %c", &exec);
		mvwprintw(win, 6, 1, "[Duração]: ");
		wscanw(win, "%3s", duration);
		mvwprintw(win, 7, 1, "[Início]: ");
		wscanw(win, "%3s", min_start);
		mvwprintw(win, 8, 1, "[Pré-requisitos]: ");
		wscanw(win, "%3s", reqs);

		strcpy(str, id);
		length = strlen(str);
		str[length++] = ' ';
		str[length++] = '\'';
		str[length] = '\0';
		
		strcat(str, name);
		length = strlen(str);
		str[length++] = '\'';
		str[length++] = ' ';
		
		str[length++] = exec;
		str[length++] = ' ';
		str[length] = '\0';
		
		strcat(str, duration);
		length = strlen(str);
		str[length++] = ' ';
		str[length] = '\0';
		
		strcat(str, min_start);
		length = strlen(str);
		str[length++] = ' ';
		str[length] = '\0';
		
		strcat(str, reqs);
		//length = strlen(str);
		//str[length++] = ' ';
		//str[length] = '\0';

		int_reqs = atoi(reqs);

		if (int_reqs > 0) {
			mvwprintw(win, 9, 1, "[Dependências]: ");
			char deps[5];
			int i, line = 0;
			for (i = 0; i < int_reqs; i++)
			{
				if (line + 10 == 18)
				{
					line = 0;
				} else {
					line = i;
				}
				mvwprintw(win, 10 + line, 1, "[Dep %d]: ", i + 1);
				wscanw(win, "%5s", deps);
				length = strlen(str);
				str[length++] = ' ';
				str[length] = '\0';
				strcat(str, deps);
			}
		}

		mvwprintw(win, 18, 1, "str: %s", str);

		//getchar();

		VertexArray array = cnvInputStrToVertex(str);
		error_check = DIGRAPHinsertV(G, array, inputCheck, nameCheck);
		if (error_check < 0)
		{
			mvwprintw(win, 18, 1, "Erro: algum valor foi inválido. Repetir.");
			int i, j;
			for (i = 3; i < 17; i++)
			{
				for (j = 1; j < 49; j++)
				{
					mvwprintw(win, i, j, " ");
				}
			}
		}
	} while (error_check < 0);
}

void insertion_window(Digraph G) 
{

	WINDOW *win = newwin(20, 50, 3, 101);
	keypad(win, TRUE);
	box(win, 0, 0);
	touchwin(win);
	wrefresh(win);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	mvwprintw(win, 1, 1, "Inserindo nova tarefa:");
	wattroff(win, COLOR_PAIR(1));

	userInsertion(win, G);

	wrefresh(win);

	//getchar();
	destroy_win(win, 20, 50);
}

void deletion_window(Digraph G) 
{
	int id,
		error_check;

	WINDOW *win = newwin(7, 25, 3, 101);
	keypad(win, TRUE);
	box(win, 0, 0);
	touchwin(win);
	wrefresh(win);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	mvwprintw(win, 1, 1, "Deletando uma tarefa:");
	wattroff(win, COLOR_PAIR(1));

	do {
		mvwprintw(win, 3, 1, "[ID]: ");
		wscanw(win, "%d", &id);

		error_check = DIGRAPHremoveV(G, id);
		if (error_check < 0)
		{
			mvwprintw(win, 5, 1, "ID inválido.");
			mvwprintw(win, 3, 1, "                       ");
		}
	}
	while (error_check < 0);

	wrefresh(win);

	destroy_win(win, 7, 25);
}

void modification_window(Digraph G) 
{
	WINDOW *win = newwin(30, 50, 3, 101);
	keypad(win, TRUE);
	box(win, 0, 0);
	touchwin(win);
	wrefresh(win);
	int 	highlight 	= 1,
 			c 			= 0,
 			choice 		= 0;

 	int id;
 	Vertex v;

 	mvwprintw(win, 1, 1, "Insira ID a ser modificada: ");
 	wscanw(win, "%d", &id);
 	v = VERTEXreturn(G, id);
	while (v == -1) 
	{
		mvwprintw(win, 28, 1, "> ID nao existente. Entre com outro valor: ");
		wscanw(win, "%d", &id);
		v = VERTEXreturn(G, id);
	} /*while*/
	mvwprintw(win, 28, 1, "                                                ");

 	//mvwprintw(win, 10, 1, "Erro, insira novamente: ");
 	print_modification(win, highlight);
 	while (1)
 	{  
    	wrefresh(win);
    	c = wgetch(win);
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
			    if(highlight == n_fields) {
			    	highlight = 1;
			    }
			    else {
			    	++highlight;
			    }
			    break;
    		case 10:
			    choice = highlight;
			    break;
			default:
			    break;
    	} /*switch*/
	    print_modification(win, highlight);
	    if(choice != 0) /* User did a choice come out of the infinite loop */
	    {
	    	wrefresh(win);
	    	break;
	    } /*if*/
	} /*while*/
	modify(win, G, id, choice);
	wgetch(win);
	destroy_win(win, 30, 50);
}

void setupGUI() {
	int i,
		highlight = 1;

	initscr();
	start_color();
	cbreak();
	//noecho();
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
	mvwprintw(execution_environment, 1, 3, "ID");
  	mvwprintw(execution_environment, 1, 12, "Nome");
  	mvwprintw(execution_environment, 1, 25, "Executada?");
  	mvwprintw(execution_environment, 1, 38, "Duração");
  	mvwprintw(execution_environment, 1, 53, "Início");
  	mvwprintw(execution_environment, 1, 68, "Pré-Requisitos");
	touchwin(execution_environment);
  	wrefresh(execution_environment);

  	mvprintw(43, 80, "Ciclo 0");
  	mvprintw(43, 1, "Menu:");
 	print_menu(highlight);
}

void DIGRAPHrestart(Digraph G, int* prev_config)
{
	int i;
	for (i = 0; i < G->V; i++)
	{
		G->array[i]->exec = prev_config[i];
	}

}

void GUI(Digraph G) {
	int 	highlight 	= 1,
 			c 			= 0,
 			choice 		= 0,
 			i;

 	int* prev_config;

	setupGUI();

  	print_instructions(G);

  	int cycle;

 	while (choice != 6)	/*Opção 6 é "sair".*/
 	{	
 		choice = 0;
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
				    if (highlight == n_choices)
				    {
				    	highlight = 1;
				    } else { 
				    	++highlight;
				    }
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
		    	mvprintw(43, 80, "Ciclo 0   ");
		    	mvprintw(50, 1, "\n");
		    	mvprintw(51, 1, "\n");
		    	print_instructions(G);
		    	break;
		    } /*if*/
		} /*while*/

		if (choice == 1) {
			insertion_window(G);
			print_instructions(G);
		} else if (choice == 2) 
		{
			modification_window(G);
			print_instructions(G);
		} else if (choice == 3) 
		{
			deletion_window(G);
			print_instructions(G);
		} else if (choice == 4) 
		{
			do
			{
				mvprintw(47, 70, "Entre com o ciclo de execução: ");
				scanw("%d", &cycle);
				if (cycle < 0) 
				{
					mvprintw(48, 70, "Erro, ciclo inválido.");
				}
			} while (cycle < 0);
			mvprintw(47, 70, "\n");
			mvprintw(48, 70, "\n");
			//DIGRAPHsave(G);
			prev_config = (int*)malloc(G->V * sizeof(int));
			for (i = 0; i < G->V; i++)
			{
				prev_config[i] = G->array[i]->exec;
			}
			execution(G, cycle);
			//DIGRAPHdestroy(G);
			DIGRAPHrestart(G, prev_config);
			free(prev_config);
		} 
		else if (choice == 5) 
		{
			//DIGRAPHsave(G);
			prev_config = (int*)malloc(G->V * sizeof(int));
			for (i = 0; i < G->V; i++)
			{
				prev_config[i] = G->array[i]->exec;
			}
			execution(G, -1);
			//DIGRAPHdestroy(G);
			//G = DIGRAPHrestart();
			DIGRAPHrestart(G, prev_config);
			free(prev_config);
		}
		print_menu(highlight);
	}

	delwin(execution_environment);
	endwin();
}