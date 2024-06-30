/*
 * Пробую поворачивать фигуры
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ROW_BRICK 4
#define COL_BRICK 4
#define NUM_FIGURES 5

// фигуры
struct
{
  char arr[ROW_BRICK][COL_BRICK];
} figures[NUM_FIGURES];

// заполняем фигуры шаблоном
int init_figures(void)
{
  char tmp[NUM_FIGURES][ROW_BRICK][COL_BRICK] =
  {
    {
      {' ',' ',' ',' '},
      {' ','#',' ',' '},
      {' ','#','#','#'},
      {' ',' ',' ',' '},
    },
    {
      {' ',' ',' ',' '},
      {' ',' ','#',' '},
      {' ','#','#','#'},
      {' ',' ',' ',' '},
    },
    {
      {' ','#',' ',' '},
      {' ','#',' ',' '},
      {' ','#',' ',' '},
      {' ','#',' ',' '},
    },
    {
      {' ','#',' ',' '},
      {' ','#',' ',' '},
      {' ',' ','#',' '},
      {' ',' ','#',' '},
    },
    {
      {' ',' ','#',' '},
      {' ',' ','#',' '},
      {' ','#',' ',' '},
      {' ','#',' ',' '},
    },
  };

  for(int i = 0; i < NUM_FIGURES; i++)
  {
    memcpy(figures[i].arr, tmp[i], sizeof(figures[i].arr));
  }
}

// выводим поворачивание фигур
int rotate()
{
  char rotate_f[ROW_BRICK][COL_BRICK];
  srand(time(NULL));
  int f = rand() % (NUM_FIGURES - 1);

  for(int i  = 0; i < ROW_BRICK; i++)
  {
    for(int j = 0; j < COL_BRICK; j++)
    {
      int ii = ROW_BRICK - j - 1;
      int jj = i;
      rotate_f[i][j] = figures[f].arr[ii][jj];
      printf("%c", rotate_f[i][j]);
    }
    printf("\n");
  }
  memcpy(figures[f].arr, rotate_f, sizeof(figures[f].arr));
  return 0;
}

int main(void)
{
  init_figures();
  for(int i = 0; i <= 4; i++)
  {
    rotate();
  }
  return 0;
}
