#ifndef LOCAL_FN
#define LOCAL_FN

void draw_ball(char *action, int line,
              int col, char map[line][col],
              int *x_ball, int *y_ball,
              int *x_direct, int *y_direct);

void gen_map(int line, int col, char map[line][col]);
void draw_map(int line, int col, char map[line][col]);
#endif
