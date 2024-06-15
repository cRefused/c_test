#ifndef LOCAL_STRPROC_H
#define LOCAL_STRPROC_H

void fn_clean_buf(char *str);
void fn_del_char(char *str, int i);
void fn_garbage_removal(char *str, char *s, char *o);
void fn_str_trim(char *str, char *s);
int fn_count_len(char *str, char *sep);
char **fn_split_string(char *str, char *sep, int len);

#endif
