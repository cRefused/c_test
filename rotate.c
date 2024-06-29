/*
 * Пробую поворачивание фигур
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ROW 4
#define COL 4
#define NUM_FIGURES 5

// фигуры
struct
{
  char arr[ROW][COL];
} figures[NUM_FIGURES];

// выводим поворачивание фигур
int rotate()
{
  char rotate_f[ROW][COL];
  srand(time(NULL));
  int f = rand() % (NUM_FIGURES - 1);

  for(int i  = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      int ii = ROW - j - 1;
      int jj = i;
      rotate_f[i][j] = figures[f].arr[ii][jj];
      printf("%c", rotate_f[i][j]);
    }
    printf("\n");
  }
  memcpy(figures[f].arr, rotate_f, sizeof(figures[f].arr));
  return 0;
}

// заполняем фигуры шаблоном
int init_figures(void)
{
  char tmp[NUM_FIGURES][ROW][COL] =
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

int main(void)
{
  init_figures();
  for(int i = 0; i < 4; i++)
  {
    rotate();
  }
  return 0;
}
