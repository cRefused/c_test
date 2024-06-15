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
  char sep[] = " ";
  // эти символы будем убирать
  char other_sep[] = ".,!@#$^&*/\\\%{}[]()-";
  // сюда будем складывать слова
  char **arr;
  // количество строк в массиве со словами
  int len;

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
  fn_garbage_removal(my_string, sep, other_sep);

  // удаляем лишние пробелы
  fn_str_trim(my_string, sep);

  // кол-во строк в массиве, где будут храниться слова
  len = fn_count_len(my_string, sep);

/*
  printf("\n");
  printf("\nline: %-4d | %-20s | **arr указывает на адрес [%p] \n",
    __LINE__, "main()", arr);
*/

  // разбиваем фразу по словам
  arr = fn_split_string(my_string, sep, len);

/*
  printf("\nline: %-4d | %-20s | **arr указывает на адрес [%p] \n",
    __LINE__, "main()", arr);
*/

  // выводим результат
  for(int i = 0; i <= len; i++)
  {
    printf("[%-2d][%s]\n", i, arr[i]);

    // очищаем память
    if(arr[i] != NULL)
    {
      free(arr[i]);
    }
  }
  printf("\n");
  return 0;
}
