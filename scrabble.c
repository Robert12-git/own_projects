#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./util/print_board.h"
#include "./util/scrabble.h"

#define TASK0 '0'
#define TASK1 '1'
#define TASK2 '2'
#define TASK3 '3'
#define TASK4 '4'
#define BOARD_SIZE	15
#define WORD_SIZE 16
#define VEC_SIZE 100
#define READ_SIZE 51
#define HORIZONTAL 0
#define VERTICAL 1
#define A 65
#define TRUE 1
#define NUM_WORDS 100
#define FALSE 0

//Player's word and its coordinates
typedef struct citire{

    char word[WORD_SIZE];
    int Y, X, D;

} Citire;

void task00();
void task01();
void task02();
void task03();
void task04();

int main(){

    char task;

    //Read number of task
    task = getchar();
    getchar();

    //Decide which task to solve
    if(task == TASK0){

        task00();

    } else if(task == TASK1){

        task01();

    } else if(task == TASK2){

        task02();

    } else if(task == TASK3){

        task03();

    } else if(task == TASK4){

        task04();

    }

    return 0;

}

void create_board(char game_board[BOARD_SIZE][BOARD_SIZE]){

    int i,j;
    
    for(i = 0; i < BOARD_SIZE; i++){

        for(j = 0; j < BOARD_SIZE; j++){

            //Each element of the board initially contains '.'
            game_board[i][j] = '.';

        }

    }

}

void task00(){

    char game_board[BOARD_SIZE][BOARD_SIZE];
    create_board(game_board);
    print_board(game_board);

}

//Read player's word and its coordinates
void read(int *y, int *x, int *d, char word[WORD_SIZE]){

    char string[READ_SIZE], *pointer;

    fgets(string, READ_SIZE, stdin);

    pointer = strtok(string, " ");
    *y = atoi(pointer);

    pointer = strtok(NULL, " ");
    *x = atoi(pointer);

    pointer = strtok(NULL, " ");
    *d = atoi(pointer);

    pointer = strtok(NULL, "\n");
    strcpy(word, pointer);
    
}

void task01(){

    char game_board[BOARD_SIZE][BOARD_SIZE];
    create_board(game_board);
    char N;
    Citire citire[VEC_SIZE];
    int n, i, j, limit_word, poz_word;

    //Read number of words
    fgets(&N, READ_SIZE, stdin);
    n = atoi(&N);

    for(i = 0; i < n; i++){

        //Read each word and its characteristics
        read(&citire[i].Y, &citire[i].X, &citire[i].D, citire[i].word);

        if(citire[i].D == VERTICAL){

            limit_word = citire[i].Y + strlen(citire[i].word);
            poz_word = 0;

            //Write word on board starting from the coordinates vertically
            for(j = citire[i].Y; j < limit_word; j++){

                game_board[j][citire[i].X] = citire[i].word[poz_word];
                poz_word++;

            }

        } else if(citire[i].D == HORIZONTAL){

            limit_word = citire[i].X + strlen(citire[i].word);
            poz_word = 0;;

            //Write word on board starting from the coordinates horizontally
            for(j = citire[i].X; j < limit_word; j++){

                game_board[citire[i].Y][j] = citire[i].word[poz_word];
                poz_word++;

            }

        }

    }

    print_board(game_board);

}

void task02(){

    //Value of each letter
    int alfabet[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5,
                     1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    char N;
    Citire citire[VEC_SIZE];
    int n, i, j, player1 = 0, player2 = 0, point, len;

    //Each main function starts the same way
    fgets(&N, READ_SIZE, stdin);
    n = atoi(&N);

    for(i = 0; i < n; i++){

        read(&citire[i].Y, &citire[i].X, &citire[i].D, citire[i].word);
        len = strlen(citire[i].word);

        //Points for player1
        if(i % 2 == 0){

            for(j = 0; j < len; j++){

                //Points without bonus
                point = alfabet[(int)citire[i].word[j] - A];
                player1 = player1 + point;

            }

        //Points for player2
        } else if(i % 2 == 1){

            for(j = 0; j < len; j++){

                //Points without bonus
                point = alfabet[(int)citire[i].word[j] - A];
                player2 = player2 + point;

            }

        }

    }

    printf("Player 1: %d Points\n", player1);
    printf("Player 2: %d Points", player2);

}

//Points without bonus
int initial_points(char word[WORD_SIZE], int len){

    int alfabet[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5,
                    1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int j, point, punctaj_cuvant = 0;

    for(j = 0; j < len; j++){

                point = alfabet[(int)word[j] - A];
                punctaj_cuvant = punctaj_cuvant + point;

            }

    return punctaj_cuvant;

}

//Points with bonux
int bonus_D(int X, int Y, int flag, int punctaj_cuvant, int len, int D){

    int word_limit0 = X + len, j;
    int word_limit1 = Y + len;

    if(D == HORIZONTAL){

        for(j = X; j < word_limit0; j++){

            if(bonus_board[Y][j] == 1 && flag == 0){

                punctaj_cuvant *= 2;

            } else if(bonus_board[Y][j] == 2 && flag == 1){

                punctaj_cuvant *= 3;

            }   
        }

    } else{

        for(j = Y; j < word_limit1; j++){

            if(bonus_board[j][X] == 1 && flag == 0){

            punctaj_cuvant *= 2;

        } else if(bonus_board[j][X] == 2 && flag == 1){

            punctaj_cuvant *= 3;

            }

        }

    }

    return punctaj_cuvant;

}

void task03(){

    char N, XX[READ_SIZE], YY[READ_SIZE];
    Citire citire[VEC_SIZE];
    int n, i, player1 = 0, player2 = 0;
    int len, punctaj_cuvant = 0, flag;

    //Pattern for bonus1
    fgets(XX, READ_SIZE, stdin);
    XX[strlen(XX) - 1] = '\0';

    //Pattern for bonus2
    fgets(YY, READ_SIZE, stdin);
    YY[strlen(YY) - 1] = '\0';

    fgets(&N, READ_SIZE, stdin);
    n = atoi(&N);

    for(i = 0; i < n; i++){

        read(&citire[i].Y, &citire[i].X, &citire[i].D, citire[i].word);
        len = strlen(citire[i].word);
        punctaj_cuvant = initial_points(citire[i].word, len);

        //Verify first pattern
        if(strstr(citire[i].word, XX) != NULL){

            //Condition for first pattern
            flag = 0;
            punctaj_cuvant = bonus_D(citire[i].X, citire[i].Y, flag,
                                     punctaj_cuvant, len, citire[i].D);

        } 

        //Verify second pattern
        if(citire[i].word[len - 2] == YY[0] && citire[i].word[len - 1] == YY[1]){

            //Flag for second pattern
            flag = 1;
            punctaj_cuvant = bonus_D(citire[i].X, citire[i].Y, flag,
                                     punctaj_cuvant, len, citire[i].D);

        }

        if(i % 2 == 0){

            player1 = player1 + punctaj_cuvant;

        } else {

            player2 = player2 + punctaj_cuvant;

        }

    }

    printf("Player 1: %d Points\n", player1);
    printf("Player 2: %d Points\n", player2);

}

//Direction and coordinates of word to be placed on board
void locate(char game_board[BOARD_SIZE][BOARD_SIZE],
            char word[WORD_SIZE], int *x, int *y, int *flag_o, int *flag_v){

    int i, j, or, ver, punct;

    for(i = 0; i < BOARD_SIZE; i++){

        for(j = 0; j < BOARD_SIZE; j++){

            //Find appearance of first letter of word
            if(game_board[i][j] == word[0]){
 
                //Verify if the word has enough space horizontally or vertically
                punct = 0;
                or = j + 1;
                while(or < BOARD_SIZE){
                    
                    //Verify space step by step
                    if(game_board[i][or] != '.'){
                        
                        break;

                    } else{

                        punct++;

                    }

                    //When there is enough space
                    if(punct == (int)(strlen(word) - 1)){
                        
                        //Direction
                        *flag_o = TRUE;

                        //Coordinates
                        *y = i;
                        *x = j;
                        break;

                    }

                    or++;

                }

                if(*flag_o == TRUE){

                    break;

                }
            
                punct = 0;
                ver = i + 1;
                while(ver < BOARD_SIZE){
                    
                    if(game_board[ver][j] != '.'){

                        break;

                    } else{

                        punct++;

                    }

                    if(punct == (int)(strlen(word) - 1)){

                        *flag_v = TRUE;
                        *y = i;
                        *x = j;
                        break;

                    }

                    ver++;

                }

                if(*flag_v == TRUE){

                    break;

                }

            }

        }

        //It is enough to find space in one direction to write the word
        if(*flag_o == TRUE || *flag_v == TRUE){

            break;

        }

    }

}

//Find first unused word which fits a gap on board
void player2_word(int cont, char used_word[VEC_SIZE][WORD_SIZE],
                char game_board[BOARD_SIZE][BOARD_SIZE], int *flag_o,
                 int *flag_v, int *x, int *y, char the_word[WORD_SIZE]){

    int i, j, unused;

    for(i = 0; i < WORD_SIZE; i++){

        unused = TRUE;

        for(j = 0; j < cont; j++){

            if(strcmp(words[i], used_word[j]) == 0){

                unused = FALSE;
                break;

            }

        }

        if(unused == TRUE){
            
            strcpy(the_word, words[i]);
            locate(game_board, words[i], x, y, flag_o, flag_v);

        }

        if(*flag_o == TRUE || *flag_v == TRUE){

            break;

        }

    }

}

void task04(){

    char XX[READ_SIZE], YY[READ_SIZE];
    char used_word[VEC_SIZE][WORD_SIZE], the_word[WORD_SIZE];
    char game_board[BOARD_SIZE][BOARD_SIZE];
    create_board(game_board);
    char N;
    Citire citire[VEC_SIZE];
    int n, i, j, limit_word, poz_word;
    int flag_o = FALSE, flag_v = FALSE, cont = 0, x, y;

    //Important function, starts the same way as the others
    fgets(XX, READ_SIZE, stdin);
    XX[strlen(XX) - 1] = '\0';

    fgets(YY, READ_SIZE, stdin);
    YY[strlen(YY) - 1] = '\0';

    fgets(&N, READ_SIZE, stdin);
    n = atoi(&N);

    for(i = 0; i < n; i++){

        read(&citire[i].Y, &citire[i].X, &citire[i].D, citire[i].word);
        strcpy(used_word[cont], citire[i].word);
        cont++;

        //Write each word in board
        if(citire[i].D == VERTICAL){

            limit_word = citire[i].Y + strlen(citire[i].word);
            poz_word = 0;

            for(j = citire[i].Y; j < limit_word; j++){

                game_board[j][citire[i].X] = citire[i].word[poz_word];
                poz_word++;

            }

        } else if(citire[i].D == HORIZONTAL){

            limit_word = citire[i].X + strlen(citire[i].word);
            poz_word = 0;;

            for(j = citire[i].X; j < limit_word; j++){

                game_board[citire[i].Y][j] = citire[i].word[poz_word];
                poz_word++;

            }

        }

    }

    //Find player's 2 next word and write on board
    player2_word(cont, used_word, game_board, &flag_o, &flag_v, &x, &y, the_word);
    if(flag_o == TRUE){

        limit_word = x + strlen(the_word);
        poz_word = 0;
        for(j = x; j < limit_word; j++){

            game_board[y][j] = the_word[poz_word];
            poz_word++;

        }

    }

    if(flag_v == TRUE){

        limit_word = y + strlen(the_word);
        poz_word = 0;
        for(j = y; j < limit_word; j++){

            game_board[j][x] = the_word[poz_word];
            poz_word++;

        }

    }

    print_board(game_board);

}