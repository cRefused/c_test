/*
Перевод двоичного числа в десятичное
*/

#include <stdio.h>
#include <string.h>

#define MAX_BITS 65 // максимум бит, которые будем считать + 1 бит конца строки

typedef unsigned long long int INT64U;

// чистка буфера
void fn_clean_buf(char *str)
{
  char b;
  int i = (strlen(str) - 1 < 0 ? 0 : strlen(str) - 1);

  if(str[i] != '\n')
  {
    while((b = getchar() != '\n') && (b != EOF))  ;
  }

  // фикс конца строки после fgets
  if(str[i] == '\n')
  {
    str[i] = '\0';
  }
}

// возведение числа в степень
INT64U fn_exp(int n, int e)
{
  INT64U result = 1;
  for(int i = 0; i < e; i++)
  {
    result *= n;
  }
  return result;
}

// преобразование двоичного в десятичное
void fn_binary_code(char *str_bin, INT64U *res)
{
  INT64U r = 0; // результат (10-е число)

  printf("\nВведите двоичное число (максимум %d \bbit). "
          "Для выхода введите 'q': ", MAX_BITS - 1);
  fgets(str_bin, MAX_BITS, stdin); // передаем строку в массив

  // чистим буфер
  fn_clean_buf(str_bin);

  // длина строки
  int len_bits = strlen(str_bin);

  // проходимся по символам с конца
  for (int i = len_bits - 1; i >= 0; i--)
  {
    int deg = len_bits - i - 1; // в какую степень возводить

    // если '1', возводим в соотв. степень и плюсуем к результату
    if(str_bin[i] == '1')
    {
      r = r + fn_exp(2, deg);
    }
    // меняем другие символы на '0', если ввели абракадабру (кроме буквы выхода)
    else if(str_bin[0] != 'q')
    {
      str_bin[i] = '0';
    }
  }

  // результат
  res[0] = len_bits; // сколько бит
  res[1] = r; // 10-е число
}

int main(void)
{
  char str_bin[MAX_BITS]; // тут будет введенная строка
  INT64U res[2]; // массив значений (сколько бит, 10-е число)
  INT64U signed_d, unsigned_d;

  while(1)
  {
    fn_binary_code(str_bin, res); // вызываем считалку, которая заполнит наш массив

    // Если ввели q, выходим
    if(str_bin[0] == 'q')
    {
      printf("Exit\n");
      break;
    }

    unsigned_d = res[1];
    signed_d = res[1] / 2;
    printf("\n"
          "Двоичное: [%d \bbit][%s]\n"
          "Десятичное:\n"
          "unsigned: %llu \n"
          "signed: %lld \n",
          res[0], str_bin,
          unsigned_d,
          signed_d);
  }
  return 0;
}

