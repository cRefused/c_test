/*
 * Tetris
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ncurses.h>

#define MAP_LINE 28
#define MAP_COL 26

int game_map[MAP_LINE][MAP_COL] = {0};

// фигуры
typedef struct
{
  int x;
  int y;
  int arr[4][4];
  int size;
} s_figures;

const s_figures figures[] = {
{1, 1, {{0,0,0,0},{0,3,3,0},{0,3,3,0},{0,0,0,0}}, 4},  // SQ
{1, 1, {{0,0,0,0},{0,3,3,3},{0,0,3,0},{0,0,0,0}}, 4},  // T
{1, 1, {{0,0,0,0},{0,0,3,3},{0,3,3,0},{0,0,0,0}}, 4},  // S
{1, 1, {{0,0,0,0},{0,3,3,0},{0,0,3,3},{0,0,0,0}}, 4},  // Z
{1, 1, {{0,0,0,0},{0,3,0,0},{0,3,3,3},{0,0,0,0}}, 4},  // ML
{1, 1, {{0,0,0,0},{0,0,0,3},{0,3,3,3},{0,0,0,0}}, 4},  // L
{1, 1, {{0,0,0,0},{3,3,3,3},{0,0,0,0},{0,0,0,0}}, 4},  // I
};

s_figures tmp_figure, cur_figure;

// цифра -> символ
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
  figure->x = rand() % (MAP_COL - figures[f].size - 1) + 1;
  figure->y = 0;
  figure->size = figures[f].size;

  for(int i = 0; i < figure->size; i++)
  {
    memcpy(figure->arr[i],  figures[f].arr[i], figure->size * sizeof(int));
  }

  return 0;
}

// вносим врезавшуюся фигуру в массив поля
int figure_to_map(s_figures* figure)
{
  int x = figure->x, y = figure->y; // коорд. фигуры
  int fx, fy; // индекс массива фигуры

  for(int i = (y + figure->size - 1); i >= y; i--)
  {
    fy = i - y;
    for(int j = (x + figure->size - 1); j >= x; j--)
    {
      fx = j - x;
      if(figure->arr[fy][fx] != 0)
      {
        game_map[i][j] = figure->arr[fy][fx];
      }
    }
  }
  return 0;
}

// проверка коллизии
int check_collizion(s_figures* figure, int* dx, int* dy, int* ni)
{
  int mx, my; // индекс массива поля
  int fx, fy; // индекс массива фигуры
  for(int i = figure->y; i <= (figure->y + figure->size - 1); i++)
  {
    fy = i - figure->y;

    // проверка выхода за границы массива поля по Y
    if(i >= MAP_LINE - 2)
    {
      my = MAP_LINE - 2;
    }
    else
    {
      my = i;
    }

    for(int j = figure->x; j <= (figure->x + figure->size - 1) ; j++)
    {
      // проверка выхода за границы массива поля по X
      if(j >= MAP_COL - 1)
      {
        mx = MAP_COL - 1;
        if(figure->arr[fy][j - figure->x] != 0) figure->x -= *dx;
      }
      else if(j <= 0)
      {
        mx = 0;
        if(figure->arr[fy][j - figure->x] != 0) figure->x -= *dx;
      }
      else
      {
        mx = j;
      }
      fx = j - figure->x;

      if(game_map[my][mx] + figure->arr[fy][fx] >= 3*2)
      {
        figure->x -= *dx;
      }
      else if(game_map[my+1][mx] + figure->arr[fy][fx] > 3)
      {
        *ni = 1;
      }
    }
  }
  return *ni;
}

// рисование фигуры
int draw_figure(s_figures* figure, int* dx, int* dy, int tmp)
{
  int mx, my; // индекс массива поля
  int fx, fy; // индекс массива фигуры
  int ni = 0, cnt = 0;

  if(tmp == 1) // у временной фигуры положение в предпросмотре
  {
    figure->y = 1;
    figure->x = MAP_COL + 2;

    for(int i = 0; i < 5; i++)
    {
      for(int j = 0; j < 5; j++)
      {
        mvaddch(figure->y+i, figure->x+j, arr_symbols[0]);
      }
    }
  }
  else // у текущей динамическое положение
  {
    figure->y += *dy;
    figure->x += *dx;

    // проверка коллизии
    check_collizion(figure, dx, dy, &ni);
  }

  for(int i = 0; i < figure->size; i++)
  {
    for(int j = 0; j < figure->size; j++)
    {
      if(figure->arr[i][j] == 0) continue;
      mvaddch(figure->y+i, figure->x+j, arr_symbols[figure->arr[i][j]]);
    }
  }

  *dy = (ni == 1 ? 0 : 1);
  *dx = 0;

  return ni;
}

// поворачивание фигур
int rotate(s_figures* figure, int* dx, int* dy)
{
  s_figures rotate_f;
  rotate_f.x = figure->x;
  rotate_f.y = figure->y;
  rotate_f.size = figure->size;

  const int s = figure->size;

  int ni = 0;

  for(int i  = 0; i < s; i++)
  {
    for(int j = 0; j < s; j++)
    {
      int ii = s - j - 1;
      int jj = i;
      rotate_f.arr[i][j] = figure->arr[ii][jj];
    }
  }

  if(check_collizion(&rotate_f, dx, dy, &ni) == 0)
  {
    memcpy(figure->arr, rotate_f.arr, sizeof(figure->arr));
  }
//  memcpy(figure->arr, rotate_f.arr, sizeof(figure->arr));
  return 0;
}

// рисование игрового поля
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
  int cx = 0, cy = 0;
  int check_lines = 4;

  for(int y = 1; y <= check_lines; y++)
  {
    for(int x = 0; x < MAP_COL; x++)
    {
      if(game_map[y][x] > 2)
      {
        cx = 1;
      }
    }
    if(cx == 1) cy += 1;
    cx = 0;
  }
  if(cy >= check_lines) return 1;
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
  int cnt_shift = 0; // счетчик после коллизии, чтобы успеть сместить фигуру
  num_figures = sizeof(figures) / sizeof(figures[0]);
  f = rand() % (num_figures);

  // ncurses init
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, 1);
  halfdelay(1);

  figure_new_iteration(&f, &f_tmp, &num_figures);
  draw_map();

  while(run != 0)
  {
    msleep(50);

    // слушаем нажатие клавиш
    switch(getch())
    {
      case KEY_UP:
        dy = 0;
        draw_map();
        rotate(&cur_figure, &dx, &dy);
        draw_figure(&cur_figure, &dx, &dy, 0);
        break;
      case KEY_DOWN: sf_tmp = sf; break;
      case KEY_LEFT: dx = -1, dy = 0; break;
      case KEY_RIGHT: dx = 1, dy = 0; break;
      case 'q': run = 0; break;
    }

    if(check_gameover() == 1)
    {
      mvprintw(MAP_LINE + 1, 0, "GAME OVER");
      continue;
    }

    if(sf_tmp >= sf || dx != 0)
    {
      draw_map();
      if(draw_figure(&cur_figure, &dx, &dy, 0) == 1)
      {
        if(cnt_shift >= 1)
        {
          cnt_shift = 0;
          figure_to_map(&cur_figure);
          figure_new_iteration(&f, &f_tmp, &num_figures);
        }
        cnt_shift++;
      }
      else
      {
        cnt_shift = 0;
        draw_figure(&tmp_figure, &dx, &dy, 1);
      }
      sf_tmp = 0;
    }
    sf_tmp += 1;
    flushinp();
  }

  nodelay(stdscr, 0);
  endwin();

  return 0;
}
