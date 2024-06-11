/*
Перевод двоичного числа в десятичное
*/

#include <stdio.h>
#include <string.h>

#define MAX_BITS 65 // максимум бит, которые будем считать + 1 бит конца строки

typedef unsigned long long int INT64U;

const INT64U max_num_x64 = 9223372036854775807ull; // ограничения для 64 бит

// возведение числа в степень
INT64U fn_exp(int n, int e)
{
  INT64U result = 1;
  for(int i = 0; i < e; i++)
  {
    if(result * n >= max_num_x64)
    {
      return max_num_x64;
    }
    else
    {
      result *= n;
    }
  }
  return result;
}

// преобразование двоичного в десятичное
void fn_binary_code(char *str_bin, INT64U *res)
{
  INT64U r = 0; // результат (10-е число)
  char buf; // для чистки буфера

  printf("\nВведите двоичное число (максимум %d \bbit). "
          "Для выхода введите 'q': ", MAX_BITS - 1);
  fgets(str_bin, MAX_BITS, stdin); // передаем строку в массив

  // чистим буфер
  if(str_bin[strlen(str_bin) -1] != '\n')
  {
    while((buf = getchar() != '\n') && (buf != EOF))  ;
  }

  // фикс конца строки после fgets
  if(str_bin[strlen(str_bin) -1] == '\n')
  {
    str_bin[strlen(str_bin) -1] = '\0';
  }

  // длина строки
  int len_bits = strlen(str_bin);

  // проходимся по символам с конца
  for (int i = len_bits - 1; i >= 0; i--)
  {
    int deg = len_bits - i - 1; // в какую степень возводить

    // если '1', возводим в соотв. степень и плюсуем к результату
    if(str_bin[i] == '1')
    {
      r = (r + fn_exp(2, deg) >= max_num_x64 ? max_num_x64 : r + fn_exp(2, deg));
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

  while(1)
  {
    fn_binary_code(str_bin, res); // вызываем считалку, которая заполнит наш массив

    // Если ввели q, выходим
    if(str_bin[0] == 'q')
    {
      printf("Exit\n");
      break;
    }

    printf("\n Двоичное: [%d \bbit][%s]\n Десятичное: %lld \n", res[0], str_bin, res[1]);
  }
  return 0;
}

