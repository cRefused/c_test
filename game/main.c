#include <time.h> // for srand()
#include <stdlib.h> // random
#include <ncurses.h> // draw game

#include "fn.h"

int main(void)
{
  int x_pers, y_pers; // координаты персонажа
  int line = 15, col = 25; // размер поля
  char pers = '@'; // вид персонажа
  char map[line][col]; // массив поля
  char action; // действие

  // генерация карты
  gen_map(line, col, map);

  // задаем рандомные координаты персонажа
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
    // положение персонажа в зависимости от action
    movie_pers(&action, line, col, map, &x_pers, &y_pers);
    // рисуем персонажа
    mvaddch(y_pers, x_pers, pers);
    action = getch(); // нажая кнопка

  }
  endwin();

  return 0;
}
