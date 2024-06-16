#include <time.h> // for srand()
#include <stdlib.h> // random
#include <ncurses.h> // draw game

#include "fn.h"

int main(void)
{
  int x_pers, y_pers; // координаты персонажа
  int line = 15, col = 25; // размер поля
  char map[line][col]; // массив поля
  char action; // действие

  // генерация карты
  gen_map(line, col, map);

  // задаем начальные координаты персонажа
  srand(time(NULL));
  x_pers = (rand() % col) + 1;
  y_pers = (rand() % line) + 1;

  // ncurses
  initscr();
  noecho();
  curs_set(0);

  while(action != 'q')
  {
    // рисуем карту
    draw_map(line, col, map);

    // рисуем персонажа
    draw_pers(&action, line, col, map, &x_pers, &y_pers);
    mvprintw(0, 30, "x_pers: %2d, y_pers: %2d, action: %c",
                    x_pers, y_pers, action);
    action = getch(); // нажая кнопка
  }
  endwin();

  return 0;
}
