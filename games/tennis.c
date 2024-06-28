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
#define MAP_COL (24)

char map[MAP_LINE][MAP_COL];

struct
{
  int x, y, dx, dy, run;
} s_platform, s_balls;

struct
{
  int free_space,
  border,
  platform,
  ball,
  brick;
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

// генерация мяча + коллизия
void* fn_draw_balls(void* argc)
{
  s_balls.run = 1;
  s_platform.run = 1;
  char *action = (char*)argc;

  // место появления
  s_balls.x = MAP_COL/2, s_balls.y = MAP_COL/2;
  s_balls.dx = 1, s_balls.dy = -1;

  while(*action != 'q')
  {
    msleep(150); // задержка

    // направление
    if(s_balls.x + s_balls.dx >= MAP_COL - 1) // пр. стена
    {
      s_balls.dx = -1;
    }
    else if(s_balls.x + s_balls.dx <= 1) // лев. стена
    {
      s_balls.dx = 1;
    }

    if(s_balls.y + s_balls.dy >= MAP_LINE - 1) // пол (gameover)
    {
      s_balls.run = 0;
      s_platform.run = 0;
      break;
    }
    else if(s_balls.y + s_balls.dy < 1) // потолок
    {
      s_balls.dy = 1;
    }
    else if(map[s_balls.y + s_balls.dy][s_balls.x + s_balls.dx] == s_id_elems.brick) //  кирпич
    {
      map[s_balls.y + s_balls.dy][s_balls.x + s_balls.dx] = s_id_elems.free_space;
      s_balls.dy = 1;
    }
    else if(map[s_balls.y + s_balls.dy][s_balls.x] == s_id_elems.platform) // платформа
    {
      s_balls.dy = -1;
      s_balls.dx += s_platform.dx;
    }

    map[s_balls.y][s_balls.x] = s_id_elems.free_space;
    s_balls.x += s_balls.dx;
    s_balls.y += s_balls.dy;
    map[s_balls.y][s_balls.x] = s_id_elems.ball;
  }

  return 0;
}

// генерация кирпичей
int init_brick(void)
{
  int line_brick = 3;
  for(int i = 1; i <= line_brick; i++)
  {
    for(int j = 1; j < MAP_COL - 1; j++)
    {
      map[i][j] = s_id_elems.brick;
    }
  }
}

// генерация платформы
int draw_platform()
{
  int platform_size = 7;

  if(s_platform.run == 0) return 0;

  // если уперлись в стену, стоим
  if(s_platform.x + s_platform.dx < 1) s_platform.dx = 0;
  else if(s_platform.x + s_platform.dx + platform_size >= MAP_COL) s_platform.dx = 0;

  // чистка слева от платформы
  map[MAP_LINE - 2][s_platform.x] = s_id_elems.free_space;

  // задаем расположение платформы
  s_platform.x += s_platform.dx;
  for(int i = s_platform.x; i < s_platform.x + platform_size; i++)
  {
    map[MAP_LINE - 2][i] = s_id_elems.platform;
  }

  // чистка справа от платформы
  if((s_platform.x + platform_size) < MAP_COL - 1)
  {
    map[MAP_LINE - 2][s_platform.x + platform_size] = s_id_elems.free_space;
  }
  return 0;
}

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

// направление платформы в зависимости
// от нажатой кнопки
int get_direct_p(char *action)
{
  if(*action == 'a')
  {
    s_platform.dx = -1;
  }
  else if(*action == 'd')
  {
    s_platform.dx = 1;
  }
  /*
  else
  {
    s_platform.dx = 0;
  }
  */
  return 0;
}

// рисуем игровое поле
// сопоставляя id с символами
int draw_map(void)
{
  // набор символов для игровых элементов
  char draw_list[] = {' ', '#', '%', '*', '@', '?'};

  for(int i = 0; i < MAP_LINE; i++)
  {
    for(int j = 0; j < MAP_COL; j++)
    {
      mvaddch(i, j, draw_list[map[i][j]]);
    }
  }
  mvprintw(MAP_LINE, MAP_COL/2 - 11, "%s", "Press 'a', 'd' to move");
  mvprintw(MAP_LINE + 1, MAP_COL/2 - 9, "%s", "Press 'q' to exit");

  if(s_balls.run == 0)
  {
    mvprintw(MAP_LINE/2, MAP_COL/2 - 5, "%s", "Game Over");
  }

  return 0;
}

int main(void)
{
  pthread_t thread_draw_balls;

  char action;
  // id игровых элементов
  s_id_elems.free_space = 0;
  s_id_elems.border = 1;
  s_id_elems.platform = 2;
  s_id_elems.ball = 3;
  s_id_elems.brick = 4;

  // начальные координаты платформы
  s_platform.dx = 0;
  s_platform.x = 1;

  init_map(); // генерация пустого игрового поля
  init_brick(); // заполняем кирпичами

  // мяч в отдельном потоке
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
    msleep(75); // задержка
    get_direct_p(&action); // направление платформы
    draw_platform(); // генерация платформы
    draw_map(); // рисуем игровое поле
  }
  pthread_join(thread_draw_balls, NULL);
  nodelay(stdscr, 0);
  endwin();

  pthread_exit(NULL);

  return 0;
}
