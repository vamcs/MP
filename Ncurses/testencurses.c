#include <ncurses.h>
#include <panel.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);


#define WORLD_WIDTH 100  //definir altura e largura da "janela"
#define WORLD_HEIGHT 40
#define NLINES 10
#define NCOLS 40

const char *choices[]={"1-Adicionar tarefa.", "2-Editar tarefa.", "3-Remover tarefa.", "4-Escolher ciclo.","5-Executar tarefa."};
int n_choices = sizeof(choices)/sizeof(char*);

void frase(){
  WINDOW *win1;
   win1 = newwin(WORLD_HEIGHT,WORLD_WIDTH,3,1);

  mvwprintw(win1,3, 25, "Tarefa 5");
  waddstr (win1, "olá" );

}
void box1(){                                                  //cria a tabela 
    WINDOW *win1;

    win1 = newwin(WORLD_HEIGHT,WORLD_WIDTH,3,1); 
 
    box(win1, 0, 0);
    mvwprintw(win1, 1, 5, "ID");
    mvwprintw(win1, 1, 15, "NOME");
    mvwprintw(win1, 1, 27, "EXECUTADA");
    mvwprintw(win1, 1, 40, "DURACAO");
    mvwprintw(win1, 1, 55, "INICIO");
    mvwprintw(win1, 1, 70, "PRE-REQUISITOS");
    frase();
    mvwprintw(win1,3, 13, "Tarefa 1");
    touchwin(win1);
    wrefresh(win1);
}

void box2(){                                                //cria a caixa do ciclo atual

      WINDOW *win2;

      win2 = newwin(5,20,3,103);
      box(win2,0, 0);
      touchwin(win2);
      mvwprintw(win2, 2, 3, "CICLO ATUAL: ");
      wrefresh(win2);
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
/* Put all the windows */
void init_wins(WINDOW **wins, int n){ int x, y, i;
  char label[80];

  y = 2;
  x = 10;
  for(i = 0; i < n; ++i)
  { wins[i] = newwin(NLINES, NCOLS, y, x);
    sprintf(label, "Window Number %d", i + 1);
    win_show(wins[i], label);
    y += 3;
    x += 7;
  }
}

/* Show the window with a border and a label */
void win_show(WINDOW *win, char *label){ 

  int startx, starty, height, width;

  getbegyx(win, starty, startx);
  getmaxyx(win, height, width);

  box(win, 0, 0);
  mvwaddch(win, 2, 0, ACS_LTEE); 
  mvwhline(win, 2, 1, ACS_HLINE, width - 2); 
  mvwaddch(win, 2, width - 1, ACS_RTEE); 
  
  print_in_middle(win, 1, 0, width, label, COLOR_PAIR(3));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color){ 
  int length, x, y;
  float temp;

  if(win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if(startx != 0)
    x = startx;
  if(starty != 0)
    y = starty;
  if(width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length)/ 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}

void panel(){ 
  
  WINDOW *my_wins[3];
  PANEL  *my_panels[3];
  PANEL  *top;
  int ch;

  /* Initialize all the colors */
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);

  init_wins(my_wins, 3);
  
  /* Attach a panel to each window */   /* Order is bottom up */
  my_panels[0] = new_panel(my_wins[0]);   /* Push 0, order: stdscr-0 */

  /* Set up the user pointers to the next panel */
  set_panel_userptr(my_panels[0], my_panels[1]);

  /* Update the stacking order. 2nd panel will be on top */
  update_panels();

  /* Show it on the screen */
  attron(COLOR_PAIR(4));
  attroff(COLOR_PAIR(4));
  doupdate();

  top = my_panels[0];
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
  while(1){  c = wgetch(menu);
    switch(c)
    {  case KEY_UP:
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
        panel();
        break;
      default:
        break;
    }
    print_menu(menu, highlight);
    if(choice != 0)  /* User did a choice come out of the infinite loop */
    break;
  }

  wrefresh(menu);
  getchar();
}

int main(){

    int selection,row=1, col=10, arraylength=10, width=5, menulength=5;
    
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