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
{1, 1, (int* []){(int []){2,2},(int []){2,2}}, 2},      // SQ
{1, 1, (int* []){(int []){0,2,0},(int []){2,2,2},(int []){0,0,0}}, 3},  // T
{1, 1, (int* []){(int []){0,2,2},(int []){2,2,0},(int []){0,0,0}}, 3},  // S
{1, 1, (int* []){(int []){2,2,0},(int []){0,2,2},(int []){0,0,0}}, 3},  // Z
{1, 1, (int* []){(int []){2,0,0},(int []){2,2,2},(int []){0,0,0}}, 3},  // ML
{1, 1, (int* []){(int []){0,0,2},(int []){2,2,2},(int []){0,0,0}}, 3},  // L
{1, 1, (int* []){(int []){2,2,2,2},(int []){0,0,0,0},(int []){0,0,0,0},(int []){0,0,0,0}}, 4},  // I
};

s_figures tmp_figure, cur_figure;

// index to symbol
char arr_symbols[] = {' ', '#', 'O'};

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

int get_figure(int f, s_figures* figure)
{
  // freeing memory from old figure
  for(int i = 0; i < figure->size; i++)
  {
    free(figure->arr[i]);
  }
  free(figure->arr);

  // create new figure
  figure->x = rand() % (MAP_COL - figures[f].size - 2) + 1;
  figure->y = 1;
  figure->size = figures[f].size;
  figure->arr = (int**)malloc(figure->size * sizeof(int*));

  for(int i = 0; i < figure->size; i++)
  {
    figure->arr[i] = (int*)malloc(figure->size * sizeof(int));
    memcpy(figure->arr[i],  figures[f].arr[i], figure->size * sizeof(int));
  }

  return 0;
}

// draw figure
int draw_figure(s_figures* figure, int tmp)
{
  int x, y;
  if(tmp == 1)
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
  else
  {
    y = figure->y;
    x = figure->x;
  }

  for(int i = 0; i < figure->size; i++)
  {
    for(int j = 0; j < figure->size; j++)
    {
      mvaddch(y+i, x+j, arr_symbols[figure->arr[i][j]]);
    }
  }
  return 0;
}

int draw_map()
{
  for(int y = 0; y < MAP_LINE; y++)
  {
    for(int x = 0; x < MAP_COL; x++)
    {
      if(x == 0 || x == MAP_COL -1 ||
      y == 0 || y == MAP_LINE - 1)
      {
        mvaddch(y, x, arr_symbols[1]);
      }
      else
      {
        mvaddch(y, x, arr_symbols[game_map[y][x]]);
      }
    }
  }

  return 0;
}

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
  int f, f_tmp, num_figures, dx = 0, dy = 0;
  num_figures = sizeof(figures) / sizeof(figures[0]);
  f = rand() % (num_figures);
  char action;

  // ncurses init
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, 1);
  halfdelay(1);

  figure_new_iteration(&f, &f_tmp, &num_figures);

  while(action != 'q')
  {
    action = getch();
    if(action == 'n')
    {
      figure_new_iteration(&f, &f_tmp, &num_figures);
    }
    draw_map();
    draw_figure(&cur_figure, 0);
    draw_figure(&tmp_figure, 1);
    msleep(50);
  }

  nodelay(stdscr, 0);
  endwin();

  return 0;
}
