#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_bomb_percent(char *level);
int get_num_bomb(int rows, int cols, int bomb_percent);
char **setup(int rows, int cols);
void init(char **gboard, int rows, int cols, int num_bomb);
void print_board(char **board, int rows, int cols);
void walker(char **pboard, char **gboard, int rows, int cols, int click_r, int click_c);
void play(char **pboard, char **gboard, int rows, int cols, int num_bomb);

int main(int argc, char *argv[]){

    if ( argc != 5 ){
        printf("Invalid command-line\n");
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    char *level = argv[3];
    int seed = atoi(argv[4]);

    if ( rows <= 0 || cols <= 0 ){
        printf("Invalid input\n");
        return 1;
    }

    if (strcmp(level, "easy") != 0 && strcmp(level, "medium") != 0 && strcmp(level, "hard") != 0){
        printf("Invalid difficulty level\n");
        return 1;
    }

    srand(seed);

    int bomb_percent = get_bomb_percent(level);
    int num_bomb = get_num_bomb(rows, cols, bomb_percent);
    char **gboard = setup(rows, cols);
    char **pboard = setup(rows, cols);

    init( gboard, rows, cols, num_bomb);

    printf("\nThis is the game board after initialization\n\n");
    print_board(gboard, rows, cols);

    printf("\nThis is the play board after setup\n\n");
    print_board(pboard, rows, cols);

    printf("\nThe game will start\n");

    play(pboard, gboard, rows, cols, num_bomb );

    for (int i = 0; i < rows; i++){
        free(gboard[i]);
        free(pboard[i]);
    }
    
    free(gboard);
    free(pboard);

    return 0;

}

int get_bomb_percent(char *level){

    int bomb_percent;
    int easy = 5;
    int medium = 20;
    int hard = 50;

    if (  (strcmp( level, "easy") == 0)){
        bomb_percent = easy;
    }
    else if (  (strcmp( level, "medium") == 0)){
        bomb_percent = medium;
    }
    else if (  (strcmp( level, "hard") == 0)){
        bomb_percent = hard;
    }
    else{
        printf("input invalid\n");
    }
    return bomb_percent;
}

int get_num_bomb(int rows, int cols, int bomb_percent){

    return (rows * cols * bomb_percent) / 100;

}

char **setup(int rows, int cols){

    char **board = malloc(rows * sizeof(char *));
    if (board == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(char));
        if (board[i] == NULL) {
            printf("Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            exit(1);
        }
        for (int k = 0; k < cols; k++) {
            board[i][k] = '.';
        }
    }
    return board;
}

void init(char **gboard, int rows, int cols, int num_bomb){

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            gboard[i][j] = '0';
        }
    }

    int num_bomb_placed = 0;
    while (num_bomb_placed < num_bomb) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (gboard[row][col] != '*') {
            gboard[row][col] = '*';
            num_bomb_placed++;
        }
    }

    for (int ro = 0; ro < rows; ro++) {
        for (int co = 0; co < cols; co++) {
            if (gboard[ro][co] == '*') {
                continue;
            }
            int count = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int r = ro + i;
                    int c = co + j;
                    if (r >= 0 && r < rows && c >= 0 && c < cols && gboard[r][c] == '*'){
                        count++;
                    }
                }
            }
            gboard[ro][co] = '0' + count;
        }
    }
}

void print_board(char **board, int rows, int cols){

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", board[i][j]);
            if (j < cols - 1){
                printf(" ");
            }
        }
        printf("\n");
    }
}

void walker(char **pboard, char **gboard, int rows, int cols, int click_r, int click_c){
    
    if (click_r < 0 || click_r >= rows || click_c < 0 || click_c >= cols){
        return;
    }

    if (pboard[click_r][click_c] != '.'){
        return;
    }

    pboard[click_r][click_c] = gboard[click_r][click_c];

    if (gboard[click_r][click_c] != '0'){
        return;
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0){
                continue;
            }
            int nr = click_r + i;
            int nc = click_c + j;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (pboard[nr][nc] == '.') {
                    walker(pboard, gboard, rows, cols, click_r + i, click_c + j);
                }
            }
        }
    }
}

void play(char **pboard, char **gboard, int rows, int cols, int num_bomb){
    int total_tiles = rows * cols;
    int hidden_tiles = total_tiles - num_bomb;
    

    while (1) {

        int click_r = rand() % rows;
        int click_c = rand() % cols;

        if (pboard[click_r][click_c] != '.'){
            continue;
        }

        printf("\nClicking on (%d, %d)\n\n", click_r, click_c);

        if (gboard[click_r][click_c] == '*') {
            printf("Bomb Exploded! Game Over!\n");
            printf("This is the play board after explosion\n\n");
            print_board(gboard, rows, cols);
            return;
        } 

        walker(pboard, gboard, rows, cols, click_r, click_c);

        print_board(pboard, rows, cols);

        int revealed_count = 0;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (pboard[i][j] != '.') {
                revealed_count++;
                }
            }
        }   

        if (revealed_count < hidden_tiles) {
            continue;
        } else {
            printf("\n Game Completed! \n\n");
            print_board(pboard, rows, cols);
            return; 
        }
    }
}
