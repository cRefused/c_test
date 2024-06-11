#include <stdio.h>
#include <string.h>

// фикс конца строки после fgets
// плюс чистка буфера
void fn_clean_buf(char *str)
{
  char b;
  int i = (strlen(str) - 1 < 0 ? 0 : strlen(str) - 1);

  if(str[i] == '\n')
  {
    str[i] = '\0';
  }
  else
  {
    while((b = getchar()) != '\n' && b != EOF)
      ;
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

