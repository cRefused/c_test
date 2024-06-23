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

struct s_platform
{
  int x, dx;
};

struct s_id_elems
{
  int free_space,
  border,
  platform,
  ball;
};

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
  int *ball = (int*)arg;

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
    map[y][x] = *ball;
  }
  return 0;
}

// движение платформы
void draw_platform(struct s_id_elems *id_elems, struct s_platform *platform)
{
  int i;
  int platform_size = 7;

  if(platform->x + platform->dx < 1) platform->dx = 0;
  else if(platform->x + platform->dx + platform_size >= MAP_COL - 1) platform->dx = 0;

  // задаем расположение платформы
  map[MAP_LINE - 2][platform->x] = id_elems->free_space;
  platform->x = platform->x + platform->dx;
  for(i = platform->x; i < platform->x + platform_size; i++)
  {
    map[MAP_LINE - 2][i] = id_elems->platform;
  }
  map[MAP_LINE - 2][platform->x + platform_size] = id_elems->free_space;
}

int main(void)
{
  struct s_id_elems id_elems;
  struct s_platform platform;

  int i, j;
  char action;
  // набор символов для отображения игровых элементов
  char draw_list[] = {' ', '#', '%', '*'};

  platform.dx = 0;
  platform.x = 1;

  // id игровых элементов
  id_elems.free_space = 0;
  id_elems.border = 1;
  id_elems.platform = 2;
  id_elems.ball = 3;

  pthread_t thread_draw_balls;

  // генерация пустого игрового поля
  for(i = 0; i < MAP_LINE; i++)
  {
    for(j = 0; j < MAP_COL; j++)
    {
      if((i == 0 || i == MAP_LINE - 1) || (j == 0 || j == MAP_COL - 1))
      {
        map[i][j] = id_elems.border;
      }
      else
      {
        map[i][j] = id_elems.free_space;
      }
    }
  }

  // рисование мяча в отдельном потоке
  pthread_create(&thread_draw_balls, NULL, fn_draw_balls, (void *)&id_elems.ball );

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
      platform.dx = -1;
    }
    else if(action == 'd')
    {
      platform.dx = 1;
    }
//    else
//    {
//      platform.dx = 0;
//    }
    mvaddch(20, 20, action);

    // платформа
    draw_platform(&id_elems, &platform);

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
