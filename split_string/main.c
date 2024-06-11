/*
* Есть строка со словами через пробел
* Задача: разбить строку по пробелам,
* слова добавить в массив
* а еще надо убрать всяки запятые с точками.
*
** подсмотрел вот здесь, понравилось как сделано, сделал по аналогии:
** https://www.cyberforum.ru/post5407849.html
*
** использование ф-ции strtok:
** http://all-ht.ru/inf/prog/c/func/strtok.html?ysclid=lx8nkcu3cr94513654
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// вынес грязную работу отдельно
#include "strproc.h"

int main(void)
{
  // пробел в качестве разделителя
  char spr[] = " ";
  // эти символы будем убирать
  char other_spr[] = ".,!@#$^&*/\\\%{}[]()-";
  // сюда будем складывать слова
  char **arr, *tmp;
  // вспомогательные переменные для счетчиков
  int len = 0, c = 0, i = 0;

  // фраза, которую надо разбить на слова
  char my_string[] = "Функция#  (strtok),  [выделяет],,,  **очередную.- {часть} /- /строки, на которую указывает  аргумент str!!!";
/*
  const int size = 257; // макс. размер строки + 1 бит
  char my_string[size];

  // вводим строку
  fgets(my_string, size, stdin);

  // чистим буфер
  fn_clean_buf(my_string);
*/
  // удаляем ненужные символы
  fn_garbage_removal(my_string, spr, other_spr);

  // удаляем лишние пробелы
  fn_str_trim(my_string, spr);

  // кол-во строк в массиве, где будут храниться слова
  for(i = 0; my_string[i] != '\0'; i++)
  {
    if(my_string[i] == spr[0])
    {
      len++;
    }
  }

  // выделяем память под строки
  arr = (char**)malloc(len * sizeof(char*));

  // передаем в tmp область до первого разделителя
  tmp = strtok(my_string, spr);

  // проходимся в цикле
  while(tmp != NULL && c <= len)
  {
    // выделяем память под слово
    arr[c] = (char*)malloc(strlen(tmp) + 1);
    // заносим слово в массив
    strcpy(arr[c], tmp);
    c++;
    // продолжить выделять область до следующего разделителя
    tmp = strtok(NULL, spr);
  }

  // выводим результат
  for(i = 0; i <= len; i++)
  {
    printf("[%-2d][%s]\n", i, arr[i]);
    /*
    for(j = 0; j < strlen(arr[i]); j++)
    {
      printf("[%c]", arr[i][j]);
    }
    printf("\n");
    */
  }
  printf("\n");
  return 0;
}
