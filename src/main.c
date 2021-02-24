#include "utils.h"

int main(int argc, char *argv[]){
    
    //Bad number of arguments as input
    if(argc != 3){

        return -1;

    }

    //Index is variable needed to count bitmap's elements for tasks 123
    int index = 0, flag_task3, flag_task1, flag_task4;
    img *image;

    image = calloc(1, sizeof(img));
    if(image == NULL){

        printf("Go to sleep no image\n");
        exit(9);

    }

    if (strcmp(argv[2], STRING_T123) == 0) {
        
        flag_task1 = Task1(argv[1], &index, image);
        if(flag_task1 != 0){

            free(image);
            return -1;

        }

        Task2(argv[1], &index, image);

        flag_task3 = Task3(argv[1], image);
        if(flag_task3 != 0){

            free(image);
            return -1;

        }

    }
    else if (strcmp(argv[2], STRING_T4) == 0) {

	    flag_task4 = Task4(argv[1], image);
        if(flag_task4 != 0){

            free(image);
            return -1;

        }

    }

    free(image);

    return 0;

}