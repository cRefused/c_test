/*
 * Знакомлюсь с потоками и структурами
 * https://metanit.com/c/tutorial/11.1.php
 * https://metanit.com/c/tutorial/6.1.php
 *
 * Пробую написать игрушку наподобии тенниса в тетрисе
*/

#include <ncurses.h>
#include <errno.h>
#include <pthread.h>

// размер игрового поля
#define MAP_LINE (20)
#define MAP_COL (25)

char map[MAP_LINE][MAP_COL];

struct
{
  int x, y, dx, dy;
} s_platform, s_balls;

struct
{
  int free_space,
  border,
  platform,
  ball;
} s_id_elems;

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

// движение мяча
void* fn_draw_balls(void* argc)
{
  s_balls.x = MAP_COL/2, s_balls.y = MAP_COL/2;
  s_balls.dx = 1, s_balls.dy = -1;
  char *action = (char*)argc;

  while(*action != 'q')
  {
    msleep(150); // задержка
    if(s_balls.x + s_balls.dx >= MAP_COL - 2)
    {
      s_balls.dx = -1;
    }
    else if(s_balls.x + s_balls.dx <= 1)
    {
      s_balls.dx = 1;
    }

    if((s_balls.y + s_balls.dy >= MAP_LINE - 1)
    || (map[s_balls.y + s_balls.dy][s_balls.x + s_balls.dx] == 2))
    {
      s_balls.dy = -1;
    }
    else if(s_balls.y + s_balls.dy < 1)
    {
      s_balls.dy = 1;
    }

    map[s_balls.y][s_balls.x] = s_id_elems.free_space;
    s_balls.x += s_balls.dx;
    s_balls.y += s_balls.dy;
    map[s_balls.y][s_balls.x] = s_id_elems.ball;
  }
  return 0;
}

// движение платформы
void draw_platform()
{
  int i;
  int platform_size = 7;

  if(s_platform.x + s_platform.dx < 1) s_platform.dx = 0;
  else if(s_platform.x + s_platform.dx + platform_size >= MAP_COL - 1) s_platform.dx = 0;

  // задаем расположение платформы
  map[MAP_LINE - 2][s_platform.x] = s_id_elems.free_space;
  s_platform.x += s_platform.dx;
  for(i = s_platform.x; i < s_platform.x + platform_size; i++)
  {
    map[MAP_LINE - 2][i] = s_id_elems.platform;
  }
  map[MAP_LINE - 2][s_platform.x + platform_size] = s_id_elems.free_space;
}

int main(void)
{
  int i, j;
  char action;
  // набор символов для отображения игровых элементов
  char draw_list[] = {' ', '#', '%', '*'};

  s_platform.dx = 0;
  s_platform.x = 1;

  // id игровых элементов
  s_id_elems.free_space = 0;
  s_id_elems.border = 1;
  s_id_elems.platform = 2;
  s_id_elems.ball = 3;

  pthread_t thread_draw_balls;

  // генерация пустого игрового поля
  for(i = 0; i < MAP_LINE; i++)
  {
    for(j = 0; j < MAP_COL; j++)
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

  // рисование мяча в отдельном потоке
  pthread_create(&thread_draw_balls, NULL, fn_draw_balls, (void *)&action );

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
      s_platform.dx = -1;
    }
    else if(action == 'd')
    {
      s_platform.dx = 1;
    }
//    else
//    {
//      s_platform.dx = 0;
//    }
    mvaddch(20, 20, action);

    // платформа
    draw_platform();

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
