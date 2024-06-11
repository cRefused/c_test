// фикс конца строки после fgets
// плюс чистка буфера
void fn_clean_buf(char *str);

// удаление символа по индексу
void fn_del_char(char *str, int i);

// удаление ненужных символов
void fn_garbage_removal(char *str, char *s, char *o);

// удаление лишних пробелов
void fn_str_trim(char *str, char *s);

// кол-во строк в массиве, где будут храниться слова
int fn_count_len(char *str, char *sep);

// разбиваем по словам
char **fn_split_string(char *str, char *sep, int len);
