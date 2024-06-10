/*
* Есть строка со словами через пробел
* Задача: разбить строку по пробелам, 
* слова добавить в массив
* а еще (в качестве бонуса, почему бы и нет) 
* уберем всякие запятые с точками.
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

// удаление символа по индексу
void delChar(char *str, int i)
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
// т.к. смещается содержимое строки и на проверяемой позиции
// оказывается символ, на который уже проверялось
void garbageRemoval(char *str, char *s, char *o)
{
  int i, j, l = strlen(str);

  i = 0;
  while(i < l - 1)
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
void strTrim(char *str, char *s)
{
  int i, j, l = strlen(str);
  
  // двойные пробелы
  i = 0;
  while(i < l - 1)
  {
    while(str[i] == s[0] && str[i + 1] == s[0])
    {
      delChar(str, i + 1);
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

void main(void)
{
  // приходится задавать через массив символов, чтобы использовать 
  // в strtok, хотя нам нужен только один разделитель
  char spr[] = " ";
  // эти символы будем убирать
  char other_spr[] = ".,!@#$^&*/\\\%{}[]()-";
  // сюда будем складывать слова
  char **arr, *tmp;
  // вспомогательные переменные для счетчиков
  int len = 0, c = 0, i = 0, j = 0;

  // фраза, которую надо разбить на слова
  char myString[] = "Функция#  (strtok),  [выделяет],,,  **очередную.- {часть} /- /строки, на которую указывает  аргумент str";
/*
  const int size = 1024;
  char myString[size];
  fgets(myString, size, stdin);
*/

  // удаляем ненужные символы
  garbageRemoval(myString, spr, other_spr);

  // удаляем лишние пробелы
  strTrim(myString, spr);
  
  // кол-во строк в массиве, где будут храниться слова
  for(i = 0; myString[i] != '\0'; i++)
  {
    if(myString[i] == spr[0])
    {    
      len++;
    }
  }
  
  // выделяем память под строки
  arr = (char**)malloc(len * sizeof(char*));
  
  // передаем в tmp область до первого разделителя
  tmp = strtok(myString, spr);
  
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
}
