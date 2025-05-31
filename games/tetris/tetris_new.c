/*
 https://codereview.stackexchange.com/questions/136406/tetris-in-c-in-200-lines
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define ROWS 20
#define COLS 11
#define TRUE 1
#define FALSE 0

char Table[ROWS][COLS] = {0};
int score = 0;
char game_on = TRUE;
double timer = 500000; //half second

typedef struct {
    char **array;
    int width, row, col;
} s_shape;
s_shape current;

const s_shape shapes_array[7]= {
    {(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},                           //S_shape
    {(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},                           //Z_shape
    {(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //T_shape
    {(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //L_shape
    {(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //ML_shape
    {(char *[]){(char []){1,1},(char []){1,1}}, 2},                                                   //SQ_shape
    {(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4} //R_shape
};

s_shape copy_shape(s_shape shape){
    s_shape new_shape = shape;
    char **copy_shape = shape.array;
    new_shape.array = (char**)malloc(new_shape.width*sizeof(char*));
    int i, j;
    for(i = 0; i < new_shape.width; i++){
        new_shape.array[i] = (char*)malloc(new_shape.width*sizeof(char));
        for(j=0; j < new_shape.width; j++) {
            new_shape.array[i][j] = copy_shape[i][j];
        }
    }
    return new_shape;
}

void delete_shape(s_shape shape){
    int i;
    for(i = 0; i < shape.width; i++){
        free(shape.array[i]);
    }
    free(shape.array);
}

int check_position(s_shape shape){ //Check the position of the copied shape
    char **array = shape.array;
    int i, j;
    for(i = 0; i < shape.width;i++) {
        for(j = 0; j < shape.width ;j++){
            if((shape.col+j < 0 || shape.col+j >= COLS || shape.row+i >= ROWS)){ //Out of borders
                if(array[i][j]) //but is it just a phantom?
                    return FALSE;
            }
            else if(Table[shape.row+i][shape.col+j] && array[i][j])
                return FALSE;
        }
    }
    return TRUE;
}

void get_new_shape(){ //returns random shape
    s_shape new_shape = copy_shape(shapes_array[rand()%7]);

    new_shape.col = rand()%(COLS-new_shape.width+1);
    new_shape.row = 0;
    delete_shape(current);
    current = new_shape;
    if(!check_position(current)){
        game_on = FALSE;
    }
}

void rotate_shape(s_shape shape){ //rotates clockwise
    s_shape temp = copy_shape(shape);
    int i, j, k, width;
    width = shape.width;
    for(i = 0; i < width ; i++){
        for(j = 0, k = width-1; j < width ; j++, k--){
                shape.array[i][j] = temp.array[k][i];
        }
    }
    delete_shape(temp);
}

void WriteToTable(){
    int i, j;
    for(i = 0; i < current.width ;i++){
        for(j = 0; j < current.width ; j++){
            if(current.array[i][j])
                Table[current.row+i][current.col+j] = current.array[i][j];
        }
    }
}

void full_line(){ //checks lines
    int i, j, sum, count=0;
    for(i=0;i<ROWS;i++){
        sum = 0;
        for(j=0;j< COLS;j++) {
            sum+=Table[i][j];
        }
        if(sum==COLS){
            count++;
            int l, k;
            for(k = i;k >=1;k--)
                for(l=0;l<COLS;l++)
                    Table[k][l]=Table[k-1][l];
            for(l=0;l<COLS;l++)
                Table[k][l]=0;
        }
    }
    timer-=1000; score += 100*count;
}

void print_table(){
    char Buffer[ROWS][COLS] = {0};
    int i, j;
    for(i = 0; i < current.width ;i++){
        for(j = 0; j < current.width ; j++){
            if(current.array[i][j])
                Buffer[current.row+i][current.col+j] = current.array[i][j];
        }
    }
    clear();
    for(i = 0; i < ROWS ;i++){
        for(j = 0; j < COLS ; j++){
            printw("%c ", (Table[i][j] + Buffer[i][j])? 'O': '.');
        }
        printw("\n");
    }
    printw("\nScore: %d\n", score);
}

void manipulate_current(int action){
    s_shape temp = copy_shape(current);
    switch(action){
        case 's':
            temp.row++;  //move down
            if(check_position(temp))
                current.row++;
            else {
                WriteToTable();
                full_line(); //check full lines, after putting it down
                get_new_shape();
            }
            break;
        case 'd':
            temp.col++;  //move right
            if(check_position(temp))
                current.col++;
            break;
        case 'a':
            temp.col--;  //move left
            if(check_position(temp))
                current.col--;
            break;
        case 'w':
            rotate_shape(temp);  //yes
            if(check_position(temp))
                rotate_shape(current);
            break;
    }
    delete_shape(temp);
    print_table();
}

int main() {
    srand(time(0));
    score = 0;
    int c;

    initscr();
    noecho();
    curs_set(0);
    halfdelay(1);

    struct timeval before, after;
    gettimeofday(&before, NULL);
    get_new_shape();
    print_table();
    while(game_on){
        if ((c = getch()) != ERR) {
          manipulate_current(c);
        }
        gettimeofday(&after, NULL);
        if (((double)after.tv_sec*1000000 + (double)after.tv_usec)-((double)before.tv_sec*1000000 + (double)before.tv_usec) > timer){ //time difference in microsec accuracy
            before = after;
            manipulate_current('s');
        }
    }
    printw("\nGame over\n");
    delete_shape(current);
    endwin();
    return 0;
}
