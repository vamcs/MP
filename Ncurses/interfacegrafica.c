#include <ncurses.h>
#include <panel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define WORLD_WIDTH 100  //definir altura e largura da "janela"
#define WORLD_HEIGHT 40
#define NLINES 10
#define NCOLS 40

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

const char *choices[]={"1-Adicionar tarefa.", "2-Editar tarefa.", "3-Remover tarefa.", "4-Escolher ciclo.","5-Executar tarefa."};
int n_choices = sizeof(choices)/sizeof(char*);

void frase(WINDOW *win1){

  mvwprintw(win1,3, 25, "Tarefa 5");
}

void box1(){                  //cria a tabela 
  WINDOW *win1;

  win1 = newwin(WORLD_HEIGHT,WORLD_WIDTH,3,1); 
 
  box(win1, 0, 0);
  mvwprintw(win1, 1, 5, "ID");
  mvwprintw(win1, 1, 15, "NOME");
  mvwprintw(win1, 1, 27, "EXECUTADA");
  mvwprintw(win1, 1, 40, "DURACAO");
  mvwprintw(win1, 1, 55, "INICIO");
  mvwprintw(win1, 1, 70, "PRE-REQUISITOS");
  frase(win1);
  touchwin(win1);
  wrefresh(win1);

  delwin(win1);
}

void box2(){                                                //cria a caixa do ciclo atual

  WINDOW *win2;

 win2 = newwin(5,20,3,103);
 box(win2,0, 0);
 touchwin(win2);
 mvwprintw(win2, 2, 3, "CICLO ATUAL: ");
 wrefresh(win2);

 delwin(win2);
}

void print_menu(WINDOW *menu_win, int highlight){
  
  int x, y, i;  

  x = 4;
  y = 4;
  for(i = 0; i < n_choices; ++i){  
    if(highlight == i + 1){ /* High light the present choice */  
      wattron(menu_win, A_REVERSE); 
      mvwprintw(menu_win, y, x, "%s", choices[i]);
      wattroff(menu_win, A_REVERSE);
    }else
      mvwprintw(menu_win, y, x, "%s", choices[i]);
    ++y;
  }
  wrefresh(menu_win);
}

void panel ()
{ WINDOW *panel;
  //PANEL  *my_panels;

  int lines = 10, cols = 40, y = 2, x = 4;

  /* Create windows for the panels */
  panel = newwin(lines, cols, y, x);
  
  /* 
   * Create borders around the windows so that you can see the effect
   * of panels
   */
    box(panel, 0, 0);

  /* Attach a panel to each window */   /* Order is bottom up */
  //my_panels = new_panel(panel);   /* Push 0, order: stdscr-0 */

  /* Update the stacking order. 2nd panel will be on top */
  update_panels();

  /* Show it on the screen */
  doupdate();
  
  getch();
}

void box3(){
  WINDOW *menu;
  static int highlight = 1;
  int c = 0;
  int choice = 0;

  menu = newwin(20,30,10,103); 
  box(menu,0, 0);
  touchwin(menu);
  mvwprintw(menu,1 ,13, "MENU ");
  keypad(menu,TRUE);
  print_menu(menu, highlight);
  while(1){  
    c = wgetch(menu);
    switch(c){  
      case KEY_UP:
        if(highlight == 1)
          highlight = n_choices;
        else
          --highlight;
        break;
      case KEY_DOWN:
        if(highlight == n_choices)
          highlight = 1;
        else 
          ++highlight;
        break;
      case 10:
        choice = highlight;
        wprintw(menu,"olá ");
       // panel();

        break;
      default:
        break;
    }
    print_menu(menu, highlight);
    if(choice != 0)  /* User did a choice come out of the infinite loop */
    break;
  }

  wrefresh(menu);
}

int main(){

    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    box1();
    box2();
    box3();

    refresh();
    endwin();
    return 0;
}