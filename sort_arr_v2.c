/*
Сортировка массива
---
 Оказывается, можно перебирать двумерный массив как одномерный,
 что упрощает задачу
*/

#include <stdio.h>

void main(void)
{
  int arr2d[5][5] = {
  {24, 23, 25, 22, 21},
  {14, 13, 15, 12, 11},
  {20, 19, 18, 17, 16},
  {2, 4, 3, 5, 1},
  {10, 6, 8, 7, 9},
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
