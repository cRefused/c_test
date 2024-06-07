/*
Перевод двоичного числа в десятичное
*/

#include <stdio.h>
#include <string.h>

typedef unsigned long long int INT64U;

const INT64U maxNumX64 = 9223372036854775807ull; // ограничения для 64 бит
 
// возведение числа в степень
INT64U fnExp(int n, int e)
{
  INT64U result = 1;
  for(int i = 0; i < e; i++)
  {
    if(result * n >= maxNumX64)
    {
      return maxNumX64;
    }
    else
    {
      result *= n;
    }
  }
  return result;
}

// преобразование двоичного в десятичное
void fnBinaryCode(INT64U * res)
{
  const unsigned int maxBits = 64; // максимум бит, которые будем считать
  INT64U r = 0; // результат (10-е число)
  char strBin[maxBits]; // тут будет введенная строка

  printf("Введите двоичное число (максимум %d \bbit): ", maxBits);
  scanf("%s", strBin); // передаем строку в массив

  int lenBits = strlen(strBin); // длина строки
  int countBits = (lenBits < maxBits ? lenBits : maxBits); // сколько бит считать (если строка короче maxBits)
  int endBit = (lenBits < maxBits ? 0 : lenBits - maxBits); // до какого бита идти (если строка длиннее maxBits)

  // проходимся по символам с конца
  for (int i = lenBits - 1; i >= endBit; i--)
  {
    int deg = lenBits - i - 1; // в какую степень возводить
    
    // если '1', возводим в соотв. степень и плюсуем к результату
    if(strBin[i] == '1')
    {
      r = (r + fnExp(2, deg) >= maxNumX64 ? maxNumX64 : r + fnExp(2, deg));
    }
  }
  
  // результат
  res[0] = countBits; // сколько бит
  res[1] = r; // 10-е число
}

int main(void)
{
  INT64U res[] = {0, 0}; // массив значений (сколько бит, 10-е число)
  fnBinaryCode(res); // вызываем считалку, которая заполнит наш массив
  printf("Двоичное (%d \bbit), десятичное: %lld \n", res[0], res[1]);
  return 0;
}

