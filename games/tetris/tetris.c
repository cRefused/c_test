/*
 * Пилю тетрис
*/

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>

// параметры фигур
#define ROW_BRICK 4
#define COL_BRICK 4
#define NUM_FIGURES 5

// параметры поля
#define MAP_LINE (24)
#define MAP_COL (20)

char map[MAP_LINE][MAP_COL];

// id игровых элементов
struct
{
  int free_space,
  border,
  brick;
} s_id_elems;

// фигуры
struct
{
  int x, y;
  int arr[ROW_BRICK][COL_BRICK];
} figures[NUM_FIGURES];

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


/* --------------------------------
 * вынести в отдельный файл
 * --------------------------------
*/
// заполняем фигуры шаблоном
int init_figures(void)
{
  int tmp[NUM_FIGURES][ROW_BRICK][COL_BRICK] =
  {
    {
      {0,0,0,0},
      {0,1,0,0},
      {0,1,1,1},
      {0,0,0,0},
    },
    {
      {0,0,0,0},
      {0,0,1,0},
      {0,1,1,1},
      {0,0,0,0},
    },
    {
      {0,1,0,0},
      {0,1,0,0},
      {0,1,0,0},
      {0,1,0,0},
    },
    {
      {0,1,0,0},
      {0,1,0,0},
      {0,0,1,0},
      {0,0,1,0},
    },
    {
      {0,0,1,0},
      {0,0,1,0},
      {0,1,0,0},
      {0,1,0,0},
    },
  };

  for(int i = 0; i < NUM_FIGURES; i++)
  {
    memcpy(figures[i].arr, tmp[i], sizeof(figures[i].arr));
  }
}

// поворачивание фигур
int rotate(int num_brick)
{
  int rotate_f[ROW_BRICK][COL_BRICK] = {0};

  for(int i  = 0; i < ROW_BRICK; i++)
  {
    for(int j = 0; j < COL_BRICK; j++)
    {
      int ii = ROW_BRICK - j - 1;
      int jj = i;
      rotate_f[i][j] = figures[num_brick].arr[ii][jj];
    }
  }

  memcpy(figures[num_brick].arr, rotate_f, sizeof(figures[num_brick].arr));
  return 0;
}

// наносим фигуру на поле
int brick_to_map(int num_brick)
{
  figures[num_brick].x = 5;
  figures[num_brick].y = 5;

  for(int i = 0; i < MAP_LINE; i++)
  {
    for(int j = 0; j < MAP_COL; j++)
    {
      if(i == figures[num_brick].y && j == figures[num_brick].x )
      {
        for(int bi = 0; bi < ROW_BRICK; bi++)
        {
          for(int bj = 0; bj < COL_BRICK; bj++)
          {
            map[i+bi][j+bj] = figures[num_brick].arr[bi][bj];
          }
        }
      }
    }
  }
  return 0;
}
//--------------------------------

// генерация пустого игрового поля
int init_map(void)
{
  for(int i = 0; i < MAP_LINE; i++)
  {
    for(int j = 0; j < MAP_COL; j++)
    {
      if((i == 0 || i == MAP_LINE - 1) || (j == 0 || j == MAP_COL - 1))
      {
        map[i][j] = s_id_elems.border;
      }
      else
      {
        map[i][j] = s_id_elems.free_space;
      }
    }
  }
  return 0;
}

// рисуем игровое поле
// сопоставляя id с символами
int draw_map(void)
{
  // набор символов для игровых элементов
  char draw_list[] = {' ', '#', '?'};

  for(int i = 0; i < MAP_LINE; i++)
  {
    for(int j = 0; j < MAP_COL; j++)
    {
      mvaddch(i, j, draw_list[map[i][j]]);
    }
  }
  mvprintw(MAP_LINE, MAP_COL/2 - 11, "%s", "Press 'a', 'd' to move");
  mvprintw(MAP_LINE + 1, MAP_COL/2 - 9, "%s", "Press 'q' to exit");

  return 0;
}

int main(void)
{
  srand(time(NULL));
  int f;
  f = rand() % (NUM_FIGURES - 1);

  char action;
  // вид игровых элементов
  s_id_elems.free_space = 0;
  s_id_elems.border = 1;
  s_id_elems.brick = 1;

  init_map(); // генерация пустого игрового поля
  init_figures(); // заполняем фигуры

  // ncurses init
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, 1);
  nodelay(stdscr, 1);

  while(action != 'q')
  {
    //action = getch();

    // слушаем нажатие клавиш
    switch(getch())
    {
      case KEY_UP: rotate(f); break;
      case KEY_DOWN: rotate(f); break;
      case KEY_LEFT: rotate(f); break;
      case KEY_RIGHT: rotate(f); break;
      case 'q': action = 'q';
    }

    msleep(100); // задержка
    init_map();
    brick_to_map(f);
    draw_map(); // рисуем игровое поле
  }
  nodelay(stdscr, 0);
  endwin();

  return 0;
}
