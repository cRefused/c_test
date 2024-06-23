/*
 * Знакомлюсь с потоками
 * https://metanit.com/c/tutorial/11.1.php?ysclid=lxqb7xcqh6523596929
 * Пробую написать игрушку наподобии тенниса в тетрисе
*/

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <errno.h>
#include <pthread.h>

// размер игрового поля
#define MAP_LINE (20)
#define MAP_COL (25)

char map[MAP_LINE][MAP_COL];

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

// рисование мяча
void* fn_draw_balls(void* arg)
{
  int x = MAP_COL/2, y = MAP_COL/2;
  int dx = 1, dy = -1;
  int *ball_id = (int*)arg;

  while(1)
  {
    msleep(150);
    if(x + dx >= MAP_COL - 2) dx = -1;
    else if(x + dx <= 1) dx = 1;

    if((y + dy >= MAP_LINE - 1) || (map[y + dy][x + dx] == 2)) dy = -1;
    else if(y + dy < 1) dy = 1;

    map[y][x] = 0;
    x += dx;
    y += dy;
    map[y][x] = *ball_id;
  }
  return 0;
}

// движение платформы
void draw_platform(int *free_space_id, int *platform_id, int *dx, int *platform_x)
{
  int i;
  int platform_size = 7;

  if(*platform_x + *dx < 1) *dx = 0;
  else if(*platform_x + *dx + platform_size >= MAP_COL - 1) *dx = 0;

  // задаем расположение платформы
  map[MAP_LINE - 2][*platform_x] = *free_space_id;
  *platform_x += *dx;
  for(i = *platform_x; i < *platform_x + platform_size; i++)
  {
    map[MAP_LINE - 2][i] = *platform_id;
  }
  map[MAP_LINE - 2][*platform_x + platform_size] = *free_space_id;
}

int main(void)
{
  int i, j, dx = 0, platform_x = 1;

  char action;
  // набор символов для отображения игровых элементов
  char draw_list[] = {' ', '#', '%', '*'};

  // id игровых элементов
  int free_space_id = 0;
  int border_id = 1;
  int platform_id = 2;
  int ball_id = 3;

  pthread_t thread_draw_balls;

  // генерация пустого игрового поля
  for(i = 0; i < MAP_LINE; i++)
  {
    for(j = 0; j < MAP_COL; j++)
    {
      if((i == 0 || i == MAP_LINE - 1) || (j == 0 || j == MAP_COL - 1))
      {
        map[i][j] = border_id;
      }
      else
      {
        map[i][j] = free_space_id;
      }
    }
  }

  // рисование мяча в отдельном потоке
  pthread_create(&thread_draw_balls, NULL, fn_draw_balls, (void *)&ball_id );

  // ncurses init
  initscr();
  noecho();
  curs_set(0);
//  keypad(stdscr, 1);
  nodelay(stdscr, 1);

  while(action != 'q')
  {
    action = getch();

    // задержка
    msleep(75);

    // нажатие кнопок
    if(action == 'a')
    {
      dx = -1;
    }
    else if(action == 'd')
    {
      dx = 1;
    }
//    else
//    {
//      dx = 0;
//    }
    mvaddch(20, 20, action);

    // платформа
    draw_platform(&free_space_id, &platform_id, &dx, &platform_x);

    // рисуем игровое поле
    for(i = 0; i < MAP_LINE; i++)
    {
      for(j = 0; j < MAP_COL; j++)
      {
        mvaddch(i, j, draw_list[map[i][j]]);
      }
    }
  }

  pthread_cancel(thread_draw_balls);

  nodelay(stdscr, 0);
  endwin();

  pthread_exit(NULL);

  return 0;
}
