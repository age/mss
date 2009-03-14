#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <iface.h>
#include <htc.h>

const char welc[] = "HT217 interface";
extern int temp[];
WINDOW *msgs;

/*print str in statustext box*/
void sText(const char *str){
  int y, ycmp, x;
  getyx(msgs, y, x);
  
  if(strlen(str) + x > COLS - 2)
    wmove(msgs, ++y, 2);
  if(y >= 9){
    scroll(msgs);
    wmove(msgs, --y, 2);
  }
  
  wprintw(msgs, str);
  
  getyx(msgs, ycmp, x);
  if(y < ycmp)
    wmove(msgs, ycmp, 2);
  box(msgs, 0, 0);
  getyx(msgs, y, x);
  mvwprintw(msgs, 0, 3, "Status");
  wmove(msgs, y, x);
  wrefresh(msgs);
}

void init(void){
  initscr();
  //cbreak();
  //noecho();
  keypad(stdscr, TRUE);
  mvprintw(0, COLS/2-strlen(welc), welc);
  //refresh();
  
  msgs = newwin(10, COLS, LINES - 10, 0);
  keypad(msgs, TRUE);
  scrollok(msgs, TRUE);
  idlok(msgs, TRUE);
  clearok(msgs, TRUE);
  wsetscrreg(msgs, 1, 8);

  box(msgs, 0, 0);
  wmove(msgs, 1, 2);
  refresh();
  noecho();
  cbreak();
  wrefresh(msgs);

  /*welcome message*/
  sText("This is HT217 interface version " VERSION ". Press F1 for help.\n");
}


int
main(int argc, char *argv[]){
  int btn, n = 0;
  char *str = (char *)malloc(32);

	init();

	while(1){
	  refresh();
	  btn = getch();
	if(btn == 'q')
	  break;
	if(btn == KEY_F(1)){
	  sprintf(str, "WHEE %d", n++);
	  sText(str);
	}
	}

	endwin();
	return 0;
}
