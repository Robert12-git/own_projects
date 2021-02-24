#ifndef utils
#define utils

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmp_header.h"
#include "cJSON.h"

#define STRING_T123 "123"
#define STRING_T4 "4"
#define STRING_BONUS "bonus"
#define MAX_LEN_STR_OUT 700000
#define len_out 100
#define MAX_9x9 9
#define BIT_LINE_SQ 15
#define ENOUGH_PIXELS 5
#define NEXT_NUMBER 18
#define ALL_IN_ONE 81
#define FIRST_TO_PASS 7
#define JUMP_IMPORTANT_ROW 3
#define NEXT_FIRST_PASS 8
#define WHITE_LINE_SQ 5
#define NEXT_NUMBER_SQ 8
#define PIXELS_1 10
#define PIXELs_235 34
#define PIXELS_4 22
#define PIXELS_69 36
#define PIXELS_7 18
#define PIXELS_0 0
#define PINCK 175
#define WHITE 255
#define DIF_NUM 24
#define NEXT_ROW 9
#define JUMP_NEXT_PX 3
#define MODIFY_PX_1 1
#define MODIFY_PX_5 13

typedef struct{

    unsigned char *array;
    bmp_fileheader file;
    bmp_infoheader info;
    
} img;

int Task1(char *argv, int *index, img *image);
void Task2(char *argv, int *index, img *image);
int Task3(char *argv, img *image);
int Task4(char *argv, img  *image);

#endif