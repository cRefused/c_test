// https://otvet.mail.ru/question/93279910?ysclid=lxm2td1z8b993956784

#include <time.h> // for srand()
#include <stdlib.h> // random
#include <ncurses.h> // draw game
#include <unistd.h>

#include "fn.h"

#define CPS CLOCKS_PER_SEC
#define DELAY 0.1

int main(void)
{
  clock_t mt;
  mt = clock();

  int x_ball, y_ball; // координаты персонажа
  int x_direct = 1, y_direct = 1;
  const int delay = 0.2;
  int line = 24, col = 20; // размер поля
  char map[line][col]; // массив поля
  char action; // действие


  // генерация карты
  gen_map(line, col, map);

  // задаем начальные координаты персонажа
  /*
  srand(time(NULL));
  x_ball = (rand() % col) + 1;
  y_ball = (rand() % line) + 1;
  */
  x_ball = col/2;
  y_ball = line/2;

  // ncurses
  initscr();
  noecho();
  curs_set(0);
//  keypad(stdscr, 1);
  nodelay(stdscr, 1);

  // рисуем карту
  draw_map(line, col, map);

  while(action != 'q')
  {
    if(clock() - mt >= CPS * DELAY)
    {
      // рисуем персонажа
      draw_ball(&action, line, col, map, &x_ball, &y_ball, &x_direct, &y_direct);

      mvprintw(0, 55, "x_ball: %2d, y_ball: %2d, action: %c",
              x_ball, y_ball, action);

      mt += (CPS * DELAY);
    }
    //action = getch(); // нажая кнопка
  }
  endwin();

  return 0;
}
