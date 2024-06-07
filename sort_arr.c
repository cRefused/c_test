/*
Сортировка массива
*/

#include <stdio.h>

void main(void)
{
  int arr2d[4][3] = {
  {12, 11, 10}, 
  {9, 1, 7}, 
  {6, 5, 4},
  {3, 8, 1}
  };
  
  int lenArr = sizeof(arr2d) / sizeof(arr2d[0][0]); // длинна массива
  int lenLine = sizeof(arr2d) / sizeof(arr2d[0]); // кол-во строк
  int lenCol = lenArr / lenLine; // кол-во столбцов

// выводим исходный массив
  for(int ll = 0; ll < lenLine; ll++)
  {
    for(int lc = 0; lc < lenCol; lc++)
    {
      printf("%3d", arr2d[ll][lc]);
    }
    printf("\n");
  }
  printf("---------------\n");
  
// сортируем
  for(int i = 0; i < lenArr; i++)
  {
    for(int j = i + 1; j < lenArr; j++)
    {
      if(arr2d[0][i] > arr2d[0][j])
      {
        int tmp = arr2d[0][i];
        arr2d[0][i] = arr2d[0][j];
        arr2d[0][j] = tmp;
      }
    }
  }
/*
// Изначальный вариант, когда не знал, что можно перебрать все элементы двумерного массива как одномерного
// и проходился по строкам
  for(int i = 0; i < lenArr; i++)
  {
    for(int ll = 0; ll < lenLine; ll++)
    {
      for(int lc = 0; lc < lenCol; lc++)
      {
        int lln;
        for(int j = lc + 1; j < lenCol; j++)
        {
          if(arr2d[ll][lc] > arr2d[ll][j])
          {
            int tmp = arr2d[ll][lc];
            arr2d[ll][lc] = arr2d[ll][j];
            arr2d[ll][j] = tmp;
          }
          if(ll == lenLine - 1)
          {
            continue;
          }
          else
          {
            lln = ll + 1;
          }
          if(arr2d[ll][j] > arr2d[lln][lc])
          {
            int tmp2 = arr2d[ll][j];
            arr2d[ll][j] = arr2d[lln][lc];
            arr2d[lln][lc] = tmp2;
          }
        }
      }
    }
  }
*/

// выводим отсортированный массив
  for(int ll = 0; ll < lenLine; ll++)
  {
    for(int lc = 0; lc < lenCol; lc++)
    {
      printf("%3d", arr2d[ll][lc]);
    }
    printf("\n");
  }
    printf("\n");

}
