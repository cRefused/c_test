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

// чистка буфера
void fn_clean_buf(char *str)
{
  char b;
  int i = (strlen(str) - 1 < 0 ? 0 : strlen(str) - 1);

  if(str[i] != '\n')
  {
    while((b = getchar()) != '\n' && b != EOF)  ;
  }

  // фикс конца строки после fgets
  if(str[i] == '\n')
  {
    str[i] = '\0';
  }
}

// удаление символа по индексу
void fn_del_char(char *str, int i)
{
  int len = strlen(str);
  if (i < len && i > -1)
  {
    while (i <= len)
    {
      str[i] = str[i+1];
      i++;
    }
  }
}

// заменяем символы, перечисленные в other_spr на пробел
// если сразу удалять, некоторые проскакивают
// т.к. смещается содержимое строки
// и на проверяемой позиции оказывается символ,
// на который уже проверялось
void fn_garbage_removal(char *str, char *s, char *o)
{
  int i, j, l = strlen(str);

  i = 0;
  while(i < l)
  {
    for(j = 0; j < strlen(o); j++)
    {
      if(str[i] == o[j])
      {
        str[i] = s[0];
      }
    }
    i++;
  }
}

// удаление лишних пробелов
void fn_str_trim(char *str, char *s)
{
  int i, j, l = strlen(str);

  // двойные пробелы
  i = 0;
  while(i < l - 1)
  {
    while(str[i] == s[0] && str[i + 1] == s[0])
    {
      fn_del_char(str, i + 1);
    }
    i++;
  }

  // пробелы в конце
  i = strlen(str) - 1;
  while(str[i] == s[0])
  {
    str[i] = '\0';
    i--;
  }
}

int main(void)
{
  // приходится задавать через массив символов, чтобы использовать
  // в strtok, хотя нам нужен только один разделитель
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
