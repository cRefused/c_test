// фикс конца строки после fgets
// плюс чистка буфера
void fn_clean_buf(char *str);

// удаление символа по индексу
void fn_del_char(char *str, int i);

// удаление ненужных символов
void fn_garbage_removal(char *str, char *s, char *o);

// удаление лишних пробелов
void fn_str_trim(char *str, char *s);
