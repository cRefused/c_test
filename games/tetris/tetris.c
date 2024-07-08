/*
 * Tetris
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ncurses.h>

#define MAP_LINE 28
#define MAP_COL 28

int game_map[MAP_LINE][MAP_COL] = {0};

// figures
typedef struct
{
  int x;
  int y;
  int **arr;
  int size;
} s_figures;

const s_figures figures[] = {
{1, 1, (int* []){(int []){3,3},(int []){3,3}}, 2},      // SQ
{1, 1, (int* []){(int []){0,3,0},(int []){3,3,3},(int []){0,0,0}}, 3},  // T
{1, 1, (int* []){(int []){0,3,3},(int []){3,3,0},(int []){0,0,0}}, 3},  // S
{1, 1, (int* []){(int []){3,3,0},(int []){0,3,3},(int []){0,0,0}}, 3},  // Z
{1, 1, (int* []){(int []){3,0,0},(int []){3,3,3},(int []){0,0,0}}, 3},  // ML
{1, 1, (int* []){(int []){0,0,3},(int []){3,3,3},(int []){0,0,0}}, 3},  // L
{1, 1, (int* []){(int []){3,3,3,3},(int []){0,0,0,0},(int []){0,0,0,0},(int []){0,0,0,0}}, 4},  // I
};

s_figures tmp_figure, cur_figure;

// index to symbol
char arr_symbols[] = {' ', '#', '#', 'O'};

/*
 * msleep(): Sleep for the requested number of milliseconds.
 * https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
 *
*/
int msleep(long msec)
{
    struct timespec ts;
    int res;
    if (msec < 0)
    {
      errno = EINVAL;
      return -1;
    }
    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;
    do {
      res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);
    return res;
}

// генерация новой фигуры
int get_figure(int f, s_figures* figure)
{
  // освобождение памяти от предыдущей фигуры
  for(int i = 0; i < figure->size; i++)
  {
    free(figure->arr[i]);
  }
  free(figure->arr);

  // создание новой фигуры
  figure->x = rand() % (MAP_COL - figures[f].size - 1) + 1;
  figure->y = 0;
  figure->size = figures[f].size;
  figure->arr = (int**)malloc(figure->size * sizeof(int*));

  for(int i = 0; i < figure->size; i++)
  {
    figure->arr[i] = (int*)malloc(figure->size * sizeof(int));
    memcpy(figure->arr[i],  figures[f].arr[i], figure->size * sizeof(int));
  }

  return 0;
}

// наносим врезавшуюся фигуру на массив карты
int figure_to_map(s_figures* figure)
{
  int x = figure->x, y = figure->y; // коорд. фигуры
  int mx, my; // индекс массива карты
  int fx, fy; // индекс массива фигуры
  
  for(int i = (y + figure->size - 1); i >= y; i--)
  {
    fy = i - y;
    // проверка выхода за границы массива карты по Y
    if(i > MAP_LINE - 1)
    {
      my = MAP_LINE - 1;
    }
    else
    {
      my = i;
    }

    for(int j = (x + figure->size - 1); j >= x; j--)
    {
      fx = j - x;
      // проверка выхода за границы массива карты по X
      if(j >= MAP_COL - 1)
      {
        mx = MAP_COL - 1;
      }
      else if(j < 0)
      {
        mx = 0;
      }
      else
      {
        mx = j;
      }
      
      if(figure->arr[fy][fx] != 0)
      {
        game_map[my][j] = figure->arr[fy][fx];
      }
    }
  }
  return 0;
}

// проверка коллизии
int check_collizion(s_figures* figure, int* dx, int* ni)
{
  int mx, my; // индекс массива карты
  int fx, fy; // индекс массива фигуры
  for(int i = (figure->y + figure->size - 1); i >= figure->y; i--)
  {
    fy = i - figure->y;

    // проверка выхода за границы массива карты по Y
    if(i > MAP_LINE - 1)
    {
      my = MAP_LINE - 1;
    }
    else
    {
      my = i;
    }

    for(int j = (figure->x + figure->size - 1); j >= figure->x; j--)
    {
      fx = j - figure->x;
      // проверка выхода за границы массива карты по X
      if(j > MAP_COL - 1)
      {
        mx = MAP_COL - 1;
      }
      else if(j < 0)
      {
        mx = 0;
      }
      else
      {
        mx = j;
      }

      if(game_map[my + 1][mx] + figure->arr[fy][fx] > 3 && *dx == 0)
      {
        *ni = 1;
        figure_to_map(figure);
      }
      else if(game_map[my + 1][mx] + figure->arr[fy][fx] > 3)
      {
        figure->x -= *dx;
      }
    }
  }
  return *ni;
}

// рисование фигуры
int draw_figure(s_figures* figure, int* dy, int* dx, int tmp)
{
  int x, y; // коорд. фигуры
  int mx, my; // индекс массива карты
  int fx, fy; // индекс массива фигуры
  int ni = 0;

  if(tmp == 1) // у временной фигуры положение в предпросмотре
  {
    y = 1;
    x = MAP_COL + 2;

    for(int i = 0; i < 5; i++)
    {
      for(int j = 0; j < 5; j++)
      {
        mvaddch(y+i, x+j, arr_symbols[0]);
      }      
    }
  }
  else // у текущей динамическое положение
  {
    figure->y += *dy;
    figure->x += *dx;

    // проверка коллизии
    check_collizion(figure, dx, &ni);

    y = figure->y;
    x = figure->x;
  }

  for(int i = 0; i < figure->size; i++)
  {
    for(int j = 0; j < figure->size; j++)
    {
      if(figure->arr[i][j] == 0) continue;
      mvaddch(y+i, x+j, arr_symbols[figure->arr[i][j]]);
    }
  }
  *dy = 1;
  *dx = 0;
  return ni;
}

// рисование карты
int draw_map()
{
  for(int y = 0; y < MAP_LINE; y++)
  {
    for(int x = 0; x < MAP_COL; x++)
    {
      if(x == 0 || x == MAP_COL -1 || y == 0 || y == MAP_LINE - 1)
      {
        game_map[y][x] = 1;
      }
      mvaddch(y, x, arr_symbols[game_map[y][x]]);
    }
  }

  return 0;
}

// проверка геймовера
int check_gameover()
{
  int y = 1;
  for(int x = 0; x < MAP_COL; x++)
  {
    if(game_map[y][x] > 2)
    {
      return 1;
    }

  }
  return 0;
}

// новая итерация фигуры
int figure_new_iteration(int* f, int* f_tmp, int* num_figures)
{
  *f_tmp = rand() % (*num_figures);

  get_figure(*f_tmp, &tmp_figure);
  get_figure(*f, &cur_figure);

  *f = *f_tmp;

  return 0;
}

int main(void)
{
  srand(time(NULL));
  int run = 1;
  int f, f_tmp; // индекс фигуры
  int num_figures; // кол-во фигур
  int dx = 0, dy = 1; // смещение
  int sf_tmp = 0, sf = 3; // скорость фигуры;
  num_figures = sizeof(figures) / sizeof(figures[0]);
  f = rand() % (num_figures);

  // ncurses init
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, 1);
  halfdelay(1);

  figure_new_iteration(&f, &f_tmp, &num_figures);

  while(run != 0)
  {
    msleep(100);
    
    // слушаем нажатие клавиш
    switch(getch())
    {
      case KEY_UP: dy = 0; break;
      case KEY_DOWN: dy = 1; break;
      case KEY_LEFT: dx = -1, dy = 0; break;
      case KEY_RIGHT: dx = 1, dy = 0; break;
      case 'q': run = 0; break;
    }
    flushinp();


    if(check_gameover() == 1)
    {
      mvprintw(MAP_LINE + 1, 0, "GAME OVER");
      continue;
    }
    
    if(sf_tmp == sf || dx != 0)
    {
      draw_map();
      if(draw_figure(&cur_figure, &dy, &dx, 0) == 1)
      {
        figure_new_iteration(&f, &f_tmp, &num_figures);
      }
      else
      {
        draw_figure(&tmp_figure, &dy, &dx, 1);
      }
      sf_tmp = 0;
    }
    sf_tmp += 1;
  }

  nodelay(stdscr, 0);
  endwin();

  return 0;
}
