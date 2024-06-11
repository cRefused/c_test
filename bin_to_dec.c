/*
Перевод двоичного числа в десятичное
*/

#include <stdio.h>
#include <string.h>

#define MAX_BITS 64 // максимум бит, которые будем считать

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

  printf("Введите двоичное число (максимум %d \bbit): ", MAX_BITS);
  fgets(str_bin, MAX_BITS + 1, stdin); // передаем строку в массив

  int len_bits = strlen(str_bin); // длина строки
  int count_bits = (len_bits < MAX_BITS ? len_bits : MAX_BITS); // сколько бит считать (если строка короче MAX_BITS)
  int end_bit = (len_bits < MAX_BITS ? 0 : len_bits - MAX_BITS); // до какого бита идти (если строка длиннее MAX_BITS)

  // проходимся по символам с конца
  for (int i = len_bits - 1; i >= end_bit; i--)
  {
    int deg = len_bits - i - 1; // в какую степень возводить

    // если '1', возводим в соотв. степень и плюсуем к результату
    if(str_bin[i] == '1')
    {
      r = (r + fn_exp(2, deg) >= max_num_x64 ? max_num_x64 : r + fn_exp(2, deg));
    }
    else if(str_bin[i] == 10)
    {
      str_bin[i] = '\0';
    }
    else
    {
      str_bin[i] = '0';
    }
  }

  // результат
  res[0] = count_bits; // сколько бит
  res[1] = r; // 10-е число
}

int main(void)
{
  char str_bin[MAX_BITS + 1]; // тут будет введенная строка
  INT64U res[2]; // массив значений (сколько бит, 10-е число)
  fn_binary_code(str_bin, res); // вызываем считалку, которая заполнит наш массив
  printf(" Двоичное: [%d \bbit][%s]\n Десятичное: %lld \n", res[0], str_bin, res[1]);
  return 0;
}

