#include <ncurses.h>

// генерация карты
void gen_map(int line, int col, char map[line][col])
{
  int x ,y;
  int cnt = 0;

  for(y = 0; y <= line; y++)
  {
    for(x = 0; x <= col; x++)
    {
      if(y == 0 || y == line || x == 0 || x == col)
      {
        map[y][x] = '#';
      }
      else if(y <= 5)
      {
        map[y][x] = '@';
      }
      else if((y == line - 1) && cnt < 4)
      {
        map[y][x] = '-';
        cnt++;
      }
      else
      {
        map[y][x] = ' ';
      }
    }
  }
}

// рисование карты
void draw_map(int line, int col, char map[line][col])
{
  int x ,y;

  for(y = 0; y <= line; y++)
  {
    for(x = 0; x <= col; x++)
    {
        mvaddch(y, x, map[y][x]);
    }
  }
}

// рисуем персонажа
void draw_ball(char *action, int line,
              int col, char map[line][col],
              int *x_ball, int *y_ball,
              int *x_direct, int *y_direct)
{
  char ball = '*'; // внешний вид персонажа
  int x_ball_cur = *x_ball, y_ball_cur = *y_ball;

  if(*x_ball + *x_direct >= col)
  {
    *x_direct = *x_direct * -1;
    //*x_ball = col - 1;
  }
  else if(*x_ball + *x_direct < 1)
  {
    *x_direct = *x_direct * -1;
    //*x_ball = 1;
  }

  if(*y_ball + *y_direct >= line)
  {
    *y_direct = *y_direct * -1;
    if((*x_ball += *x_direct >= col) || (*x_ball += *x_direct <= 1))
    {
      *x_ball -= *x_direct;
    }
    else
    {
      *x_ball += *x_direct;
    }
  }
  else if(*y_ball + *y_direct < 1)
  {
    *y_direct = *y_direct * -1;
    //*y_ball = 1;
  }
  else if(map[*y_ball + *y_direct][*x_ball + *x_direct] == '@')
  {
    mvaddch((*y_ball + *y_direct), (*x_ball + *x_direct), ' ');
    map[*y_ball + *y_direct][*x_ball + *x_direct] = ' ';
    *y_direct = *y_direct * -1;
  }

  *x_ball = *x_ball + *x_direct;
  *y_ball = *y_ball + *y_direct;
  switch(getch())
  {
    case KEY_UP: *y_ball -= 1; break;
    case KEY_DOWN: *y_ball += 1; break;
    case KEY_LEFT: *x_ball -= 1; break;
    case KEY_RIGHT: *x_ball += 1; break;
    case 'q': *action = 'q';
  }
/*
  if(*action == 'w' && map[*y_ball-1][*x_ball] != '#')
  {
    *y_ball -= 1;
  }
  else if(*action == 's' && map[*y_ball+1][*x_ball] != '#')
  {
    *y_ball += 1;
  }
  else if(*action == 'a' && map[*y_ball][*x_ball-1] != '#')
  {
    *x_ball -= 1;
  }
  else if(*action == 'd' && map[*y_ball][*x_ball+1] != '#')
  {
    *x_ball += 1;
  }

*/
  mvaddch(*y_ball, *x_ball, ball);
  mvaddch(y_ball_cur, x_ball_cur, ' ');
}


