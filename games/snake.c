/*
 * Увидел здесь: https://otvet.mail.ru/answer/468274019
 * переделал под свои хотелки:
 ** теперь видны границы
 ** змейка ходит через стены (опционально, блок с врезанием оставил)
 ** линейный прирост очков
 ** еда появляется сразу после съедания
 ** заменил таймер, т.к. исходный жрал процессор
 ** и еще всякие мелочи
 **
*/

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <errno.h>

struct snake
{
  int x;
  int y;
} sc[32];

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

// рисование рамки
void draw_frame(int line, int col)
{
  char g_frame = '#', v_frame = '#';
  int i;

  for(i = 0; i <= line; i++ )
  {
      mvaddch(i, 0, v_frame);
      mvaddch(i, col, v_frame);
  }
  for(i = 0; i <= col; i++ )
  {
      mvaddch(0, i, g_frame);
      mvaddch(line, i, g_frame);
  }
}

// генерация еды
void gen_eat(int *eat_x, int *eat_y, int snake_len, int line, int col, char eat)
{
  srand(time(NULL));
  int c, cf = 1;

  while (cf)
  {
    *eat_x = rand() % col;
    *eat_y = rand() % line;
    cf = 0;
    for (c = 0; c < snake_len; c++)
    {
      if ((*eat_x == sc[c].x && *eat_y == sc[c].y)
          || *eat_x == col
          || *eat_x == 0
          || *eat_y == line
          || *eat_y == 0)
      {
        cf = 1;
      }
    }
  }
  mvaddch(*eat_y, *eat_x, eat);
}

int main() {
  int x, y, i = 0, snake_len = 3, dx = 1, dy = 0, eat_x = -1, eat_y = -1, score = 0, c, run = 1;
  int col = 26, line = 25;
  char eat = '@'; // вид еды
  char snake_body = '#'; // вид змейки

  // ncurses init
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, 1);
  nodelay(stdscr, 1);

  // где появляемся
  sc[0].x = col/3; sc[0].y = line/2;

  draw_frame(line, col); // рамка
  gen_eat(&eat_x, &eat_y, snake_len, line, col, eat); // еда
  mvprintw(line + 1, col/2 - 9, "%s", "press 'q' to quit");

  while(run)
  {
    // задержка
    msleep(100);
    // слушаем нажатие клавиш
    switch(getch())
    {
      case KEY_UP: dx = 0; dy = -1; break;
      case KEY_DOWN: dx = 0; dy = 1; break;
      case KEY_LEFT: dx = -1; dy = 0; break;
      case KEY_RIGHT: dx = 1; dy = 0; break;
      case 'q': run = 0; break;
    }

    x = sc[0].x + dx, y = sc[0].y + dy;

    // если съели еду, увеличиваемся, и генерируем новую
    if(x == eat_x && y == eat_y)
    {
      snake_len++;
      score += 10;
      gen_eat(&eat_x, &eat_y, snake_len, line, col, eat);
    }

    // проходим через стены
    if (x >= col) x = 1;
    else if (x <= 0) x = col - 1;
    else if (y >= line) y = 1;
    else if (y <= 0) y = line - 1;

    mvaddch(y, x, snake_body);
    if(sc[snake_len-1].y != 0) // фикс затирания угла рамки
    {
      mvaddch(sc[snake_len-1].y, sc[snake_len-1].x, ' ');
    }
    memmove(sc + 1, sc, snake_len * sizeof(sc[0]));
    sc[0].x = x;
    sc[0].y = y;

/*
    // если врезались в стену - геймовер
    if (x >= col || x < 1 || y >= line || y < 1)
    {
      run = 0;
    }
*/

    for (c = 1; c < snake_len; c++)
    {
      // если врезались в себя - геймовер
      if (x == sc[c].x && y == sc[c].y)
      {
        run = 0;
      }
      // если еда под змейкой, генерируем заново
      else if (eat_x == sc[c].x && eat_y == sc[c].y)
      {
        gen_eat(&eat_x, &eat_y, snake_len, line, col, eat);
      }
    }
    mvprintw(1, col + 2, "score: %d", score);
  }

  //clear();
  nodelay(stdscr, 0);

  while(i != 10)
  {
    mvprintw(line/3 + i, col/2 - 7, "%s", "              "); i++;
    mvprintw(line/3 + i, col/2 - 7, "%s", "**************"); i++;
    mvprintw(line/3 + i, col/2 - 7, "%s", "* GAME OVER! *"); i++;
    mvprintw(line/3 + i, col/2 - 7, "%s", "**************"); i++;
    mvprintw(line/3 + i, col/2 - 7, "%s", "              "); i++;
    i = 10;
  }

  char quit;
  while(quit != 'q')
  {
    quit = getch();
  }
  endwin();
}
