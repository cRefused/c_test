#include <ncurses.h>

// генерация карты
void gen_map(int line, int col, char map[line][col])
{
  int x ,y;

  for(y = 0; y <= line; y++)
  {
    for(x = 0; x <= col; x++)
    {
      if(y == 0 || y == line || x == 0 || x == col)
      {
        map[y][x] = '#';
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

// положение персонажа
void movie_pers(char *action, int line, int col, char map[line][col], int * x_pers, int * y_pers)
{
  if(*x_pers >= col)
  {
    *x_pers = col - 1;
  }
  else if(*x_pers < 1)
  {
    *x_pers = 1;
  }

  if(*y_pers >= line)
  {
    *y_pers = line - 1;
  }
  else if(*y_pers < 1)
  {
    *y_pers = 1;
  }

  if(*action == 'w' && map[*y_pers-1][*x_pers] != '#')
  {
    *y_pers -= 1;
  }
  else if(*action == 's' && map[*y_pers+1][*x_pers] != '#')
  {
    *y_pers += 1;
  }
  else if(*action == 'a' && map[*y_pers][*x_pers-1] != '#')
  {
    *x_pers -= 1;
  }
  else if(*action == 'd' && map[*y_pers][*x_pers+1] != '#')
  {
    *x_pers += 1;
  }
}


