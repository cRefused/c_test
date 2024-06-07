/*
Сортировка массива
---
 Начальный вариант, когда не знал, что можно перебрать все элементы двумерного массива 
 как одномерного и проходился по строкам
---
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
  int lln, tmp, tmp2;
  for(int i = 0; i < lenArr; i++) // всего итераций по lenArr
  {
    for(int ll = 0; ll < lenLine; ll++) // проходимся по строкам
    {
      for(int lc = 0; lc < lenCol; lc++) // проходимся по столбцам в строке
      {
        for(int j = lc + 1; j < lenCol; j++) // сравниваем со следующими значениями в строке
        {
          if(arr2d[ll][lc] > arr2d[ll][j]) // если текущее больше следующего, меняем местами
          {
            tmp = arr2d[ll][lc];
            arr2d[ll][lc] = arr2d[ll][j];
            arr2d[ll][j] = tmp;
          }
          // если это последняя строка, переходим к новой итерации, 
          // иначе сравниваем со значением следующей строки
          if(ll == lenLine - 1)
          {
            continue;
          }
          else
          {
            lln = ll + 1;
          }
          if(arr2d[ll][j] > arr2d[lln][lc]) // если текущее значение больше чем в следующей строке, меняем
          {
            tmp2 = arr2d[ll][j];
            arr2d[ll][j] = arr2d[lln][lc];
            arr2d[lln][lc] = tmp2;
          }
        }
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
