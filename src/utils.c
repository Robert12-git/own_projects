#include "utils.h"

//Access all info from file and convert to usable data
int read(cJSON *try, img *image, int *i){

    cJSON *file =cJSON_GetObjectItem(try, "file_header");
    cJSON *offset = cJSON_GetObjectItem(file, "offset");
    cJSON *signature = cJSON_GetObjectItem(file, "signature");
    cJSON *reserved = cJSON_GetObjectItem(file, "reserved");
    cJSON *size_file = cJSON_GetObjectItem(file, "file_size");
    
    char *string = cJSON_GetStringValue(signature);
    image->file.fileMarker1 = string[0];
    image->file.fileMarker2 = string[1];
    image->file.bfSize = (unsigned int)cJSON_GetNumberValue(size_file);
    image->file.unused1 = (unsigned short)cJSON_GetNumberValue(reserved);
    image->file.unused2 = (unsigned short)cJSON_GetNumberValue(reserved);
    image->file.imageDataOffset = (unsigned int)cJSON_GetNumberValue(offset);

    cJSON *info = cJSON_GetObjectItem(try, "info_header");
    cJSON *size = cJSON_GetObjectItem(info, "size");
    cJSON *width = cJSON_GetObjectItem(info, "width");
    cJSON *height = cJSON_GetObjectItem(info, "height");
    cJSON *planes = cJSON_GetObjectItem(info, "planes");
    cJSON *bit_count = cJSON_GetObjectItem(info, "bit_count");
    cJSON *compression = cJSON_GetObjectItem(info, "compression");
    cJSON *image_size = cJSON_GetObjectItem(info, "image_size");
    cJSON *biX = cJSON_GetObjectItem(info, "x_pixels_per_meter");
    cJSON *biY = cJSON_GetObjectItem(info, "y_pixels_per_meter");
    cJSON *colors_used = cJSON_GetObjectItem(info, "colors_used");
    cJSON *colors_imp = cJSON_GetObjectItem(info, "colors_important");

    image->info.biSize = (unsigned int)cJSON_GetNumberValue(size);
    image->info.width = (unsigned int)cJSON_GetNumberValue(width);
    image->info.height = (unsigned int)cJSON_GetNumberValue(height);
    image->info.planes = (unsigned short)cJSON_GetNumberValue(planes);
    image->info.bitPix = (unsigned short)cJSON_GetNumberValue(bit_count);
    image->info.biCompression = (unsigned int)cJSON_GetNumberValue(compression);
    image->info.biSizeImage = (unsigned int)cJSON_GetNumberValue(image_size);
    image->info.biXPelsPerMeter = (int)cJSON_GetNumberValue(biX);
    image->info.biYPelsPerMeter = (int)cJSON_GetNumberValue(biY);
    image->info.biClrUsed = (unsigned int)cJSON_GetNumberValue(colors_used);
    image->info.biClrImportant = (unsigned int)cJSON_GetNumberValue(colors_imp);

    cJSON *bitmap = cJSON_GetObjectItem(try, "bitmap");
    cJSON *vector;

    image->array = calloc(image->info.biSizeImage, sizeof(unsigned char));
    if(image->array == NULL){

        free(image);
        cJSON_Delete(try);

        printf("No bitmap\n");
        return 12;

    }

    int k = 0;
    cJSON_ArrayForEach(vector, bitmap){

        image->array[k++] = cJSON_GetNumberValue(vector);

    }

    *i = k;

    return 0;

}

//Write info in bmp file element by element
void write1(img *image, int *i, FILE *outputFile){

    int k, index, size_array;

    fwrite(&(image->file), 1, sizeof(image->file), outputFile);
    fwrite(&(image->info), 1, sizeof(image->info), outputFile);

    k = *i;
    size_array = image->info.biSizeImage / image->info.height;
    for(int j = 1; j <= image->info.height; j++){

        index = (k / image->info.width) * (image->info.height - j);
        fwrite(&(image->array[index]), 1, size_array, outputFile);

    }

}

void invert(unsigned char *x, unsigned char *y){

    unsigned char aux;
    
    aux = *x;
    *x = *y;
    *y = aux;

}

void invert_array(img *image){

    int flag = 0, num_per_line;

    //Number of elements per line without padding
    num_per_line = image->info.biSizeImage / image->info.width - 1;
    for(int i = 2; i < image->info.height - 2; i++){

        //Invert elements according to certain pattern
        for(int j = 6 + i * num_per_line; j < (i + 1) * num_per_line; j++){

            //If inverted enough elements go to next number
            if(flag == ENOUGH_PIXELS){

                j += NEXT_NUMBER;
                flag = 0;

            } else if(flag < 3){

                invert(&image->array[j], &image->array[j + 12]);
                flag++;

            } else{

                invert(&image->array[j], &image->array[j + 6]);
                flag++;

            }

        }

    }

}

//Same as write1, but also inverts certain elements from bitmap
void write2(img *image, int *i, FILE *outputFile){

    int k, index, size_array;
    fwrite(&(image->file), 1, sizeof(image->file), outputFile);
    fwrite(&(image->info), 1, sizeof(image->info), outputFile);
    //Invert certain bitmap elements to mirror image
    invert_array(image);

    k = *i;
    size_array = image->info.biSizeImage / image->info.height;
    for(int j = 1; j <= image->info.height; j++){

        index = (k / image->info.width) * (image->info.height - j);
        fwrite(&(image->array[index]), 1, size_array, outputFile);

    }

}

//Check if square holds number or not
int white(img *image, unsigned char *array){

    //Ver is first line to be passed
    //Flag says if square holds number or not
    int ver = FIRST_TO_PASS, flag = 1, index, num_per_line;

    num_per_line = image->info.biSizeImage / image->info.width - 1;
    for(int i = 2; i < image->info.height - 2; i++){

        if(i == ver){

            i += JUMP_IMPORTANT_ROW;
            ver += NEXT_FIRST_PASS;

        }

        for(int j = 6 + i * num_per_line; j < (i + 1) * num_per_line; j++){

            index = 0;

            //Save elements from certain line of number
            for(int k = 0; k < BIT_LINE_SQ; k++){

                array[k] = image->array[j];
                j++;

            }

            //Verify if square 5x5 has full white line
            for(int k = 0; k < BIT_LINE_SQ; k += JUMP_IMPORTANT_ROW){
                
                if((array[k] == WHITE) &&
                                    (array[k + 1] == WHITE) &&
                                                (array[k + 2] == WHITE)){
                                                    
                    index++;

                }

                if(index == WHITE_LINE_SQ){

                    flag = 0;

                }

            }

            j += NEXT_NUMBER_SQ;

        }

    }

    return flag;

}

int write_loss_task3(char *argv){

    char *output, *buffer;
    FILE *outputFile;
    int len = 0;

    output = calloc(len_out, sizeof(char));
    if(output == NULL){

        printf("Error creating output name\n");
        return 4;

    }

    buffer = calloc(len_out, sizeof(char));

        if(buffer == NULL){

            free(output);

            printf("No buffer\n");
            return 20;

        }

    sprintf(output, "./output_task3_board%c%c.json", argv[19], argv[20]);

    outputFile = fopen(output, "wt");
    if(outputFile == NULL){

        free(output);

        printf("Error opening output file\n");
        return 5;

    }

    sprintf(output, "board%c%c.json", argv[19], argv[20]);
    len += sprintf(buffer + len, "{\n\t\"input_file\":\t");
    len += sprintf(buffer + len, "\"%s\"", output);
    len += sprintf(buffer + len, ",\n\t\"game_state\":\t\"Loss :(\"\n}\n");
    fprintf(outputFile, "%s", buffer);

    free(buffer);
    free(output);
    fclose(outputFile);

    return 0;

}

int write_win_task3(char *argv){

    char *output, *buffer;
    FILE *outputFile;
    int len = 0;

    output = calloc(len_out, sizeof(char));
    if(output == NULL){

        printf("Error creating output name\n");
        return 4;

    }

    buffer = calloc(len_out, sizeof(char));

        if(buffer == NULL){

            free(output);

            printf("No buffer\n");
            return 20;

        }

    sprintf(output, "./output_task3_board%c%c.json", argv[19], argv[20]);

    outputFile = fopen(output, "wt");
    if(outputFile == NULL){

        free(output);

        printf("Error opening output file\n");
        return 5;

    }

    sprintf(output, "board%c%c.json", argv[19], argv[20]);
    len += sprintf(buffer + len, "{\n\t\"input_file\":\t");
    len += sprintf(buffer + len, "\"%s\"", output);
    len += sprintf(buffer + len, ",\n\t\"game_state\":\t\"Win!\"\n}\n");
    fprintf(outputFile, "%s", buffer);

    free(buffer);
    free(output);
    fclose(outputFile);

    return 0;

}

//Create the mask of the board
void write_in_matrix(int *matrix, int *p, int var, img *image, int j){

    int ho, no_pad_nums;
    
    ho = *p;
    no_pad_nums = image->info.biSizeImage / image->info.height - 1;

    if(var == PIXELS_1){

        matrix[ho] = 1;
        ho++;

    } else if(var == PIXELs_235){

        if(image->array[j - 11 - 3 * no_pad_nums] == PINCK){

            if(image->array[j - 11 - no_pad_nums] == PINCK){

                matrix[ho] = 3;
                ho++;
            } else{

                matrix[ho] = 2;
                ho++;

            }
        } else{

            matrix[ho] = 5;
            ho++;

        }

    } else if(var == PIXELS_4){

        matrix[ho] = 4;
        ho++;

    } else if(var == PIXELS_69){

        if(image->array[j - 11 - 3 * no_pad_nums] == 175){

            matrix[ho] = 9;
            ho++;

        } else{

            matrix[ho] = 6;
            ho++;

        }

    } else if(var == PIXELS_7){

        matrix[ho] = 7;
        ho++;

    } else if(var == PIXELS_0){

        matrix[ho] = 0;
        ho++;

    } else{

        matrix[ho] = 8;
        ho++;

    }

    *p = ho;

}

//Write in mask column by column 
void create_column(img *image, unsigned char *array, int *matrix, int m, int *p){

    int ver = FIRST_TO_PASS, var = 0, ho, no_pad, index;
    int j;
    
    ho = *p;
    no_pad = image->info.biSizeImage / image->info.width - 1;

    for(int i = 2; i < image->info.height - 2; i++){

        if(i == ver){
                        
           write_in_matrix(matrix, &ho, var, image, j);

            i += JUMP_IMPORTANT_ROW;
            ver += NEXT_FIRST_PASS;
            var = 0;

        }

        index = 6 + m * DIF_NUM + i * no_pad;
        for(j = index; j < index + BIT_LINE_SQ; j++){

            //Verify if square has number or white space
            for(int k = 0; k < BIT_LINE_SQ; k++){
                
                array[k] = image->array[j];
                if(array[k] == PINCK){

                    var++;

                }

                j++;

            }

            j += NEXT_NUMBER_SQ;

        }

    }

    write_in_matrix(matrix, &ho, var, image, j);

    *p = ho;

}

//Check mask to see if line of board respects first rule
int verify_column(int *matrix, int *flag_other_hand, char *argv){

    int contor = 0, flag = 0, flag_task3;
    while(contor < ALL_IN_ONE){

        if(flag == 1){

            break;

        }

        for(int i = contor; i < contor + 8; i++){

            if(flag == 1){

                break;

            }

            for(int j = i + 1; j < contor + 9; j++){

                if(matrix[i] == matrix[j]){

                    flag_task3 = write_loss_task3(argv);
                    if(flag_task3 != 0){

                        return -1;

                    }

                    flag = 1;
                    break;

                }

            }

        }

        contor += 9;

    }

    *flag_other_hand = flag;
    return 0;

}

//Check mask to see if column of board respects second rule
int verify_row(int *matrix, int *flag, char *argv){

    int flag_task3, index;

    index = ALL_IN_ONE - MAX_9x9;

    *flag = 0;
    for(int i = 0; i < MAX_9x9; i++){

        if(*flag == 1){

            break;

        }

        for(int j = i; j < index + i; j += NEXT_ROW){

            if(*flag == 1){

                break;

            }

            for(int q = j + 9; q <= 72 + i; q += NEXT_ROW){

                if(matrix[j] == matrix[q]){

                    flag_task3 = write_loss_task3(argv);
                    if(flag_task3 != 0){

                        return -1;

                    }

                    *flag = 1;
                    break;

                }

            }

        }

    }

    return 0;

}

//Check mask to see if 9x9 square of board respects third rule
int verify_square(int *matrix, int *flag, char *argv){

    int flag_task3;

    *flag = 0;
    for(int contor = 0; contor <= 60; contor += 3){

        if((contor / 9 == 0) && (contor != 0)){

            contor += 18;

        }

        for(int i = 0; i < 3; i++){

            if(*flag == 1){

                break;

            }

            for(int j = 0; j < 2; j++){

                if(*flag == 1){

                    break;

                }

                for(int l = j + 1; l < 3; l++){

                    if(matrix[i * 9 + contor + j] == matrix[i * 9 + contor + l]){

                        flag_task3 = write_loss_task3(argv);
                        if(flag_task3 != 0){

                            return -1;

                        }
                        
                        *flag = 1;
                        break;

                    }

                }

            }

        }
        
    }

    return 0;

}

//Free parameters for Task3
void free_parameters(int *matrix, unsigned char *array, img *image){

    if(matrix != NULL){

        free(matrix);

    }

    if(array != NULL){

        free(array);
    
    }

    if(image->array != NULL){

        free(image->array);

    }

    if(image != NULL){

        free(image);

    }

}

//Free parameters for Task4
void free_parameters_4(char *buffer, int *array_numbers, int **good_matrix,
                        int *matrix, unsigned char *array, img *image,
                         FILE *inputFile, FILE *outputFile){

    if(outputFile != NULL){

        fclose(outputFile);

    }

    if(buffer != NULL){

        free(buffer);

    }

    if(array_numbers != NULL){

        free(array_numbers);

    }

    for(int i = 0; i < MAX_9x9; i++){

        if(good_matrix[i] != NULL){

            free(good_matrix[i]);

        }

    }

    if(good_matrix != NULL){

        free(good_matrix);

    }

    if(matrix != NULL){

        free(matrix);

    }

    if(array != NULL){

        free(array);

    }

    if(image->array != NULL){

        free(image->array);

    }

    if(inputFile != NULL){

        fclose(inputFile);

    }

}


//fill_with_1-9 writes in white squares numbers which fit
void fill_with_1(img *image, int mama){

    image->array[mama + 13] = 0;

}

void fill_with_2(img *image, int mama, int row,
                int col, int **matrix, int *array_numbers){

    int index;

    index = matrix[row][col] - 1;
    if(array_numbers[index] == 0){

            for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

                image->array[mama + i + MODIFY_PX_1] = 0;

            }

            array_numbers[index]++;

        } else if(array_numbers[index] == 1){

            image->array[mama + MODIFY_PX_5] = 0;
            array_numbers[index]++;

        } else if(array_numbers[index] == 2){

            for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

                image->array[mama + i + MODIFY_PX_1] = 0;

            }

            array_numbers[index]++;

        } else if(array_numbers[index] == 3){

            image->array[mama + MODIFY_PX_1] = 0;
            array_numbers[index]++;

        } else if(array_numbers[index] == 4){

            for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

                image->array[mama + i + MODIFY_PX_1] = 0;

            }

            array_numbers[index]++;

        }

        if(array_numbers[index] == 5){

            array_numbers[index] = 0;

        }

}

void fill_with_3(img *image, int mama,
                int row, int col, int **matrix, int *array_numbers){

    int index;

    index = matrix[row][col] - 1;
    if(array_numbers[index] == 0){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 1){

        image->array[mama + MODIFY_PX_5] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 2){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 3){

        image->array[mama + MODIFY_PX_5] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 4){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    }

    if(array_numbers[index] == 5){

        array_numbers[index] = 0;

    }

}

void fill_with_4(img *image, int mama,
                int row, int col, int **matrix, int *array_numbers){

    int index;

    index = matrix[row][col] - 1;
    if(array_numbers[index] == 0){

        image->array[mama + MODIFY_PX_5] = 0;
        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 1){

        image->array[mama + MODIFY_PX_5] = 0;
        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 2){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

                image->array[mama + i + MODIFY_PX_1] = 0;

            }

        array_numbers[index]++;

    } else if(array_numbers[index] == 3){

        image->array[mama + MODIFY_PX_5] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 4){
        
        image->array[mama + MODIFY_PX_5] = 0;
        array_numbers[index]++;

    }

    if(array_numbers[index] == 5){

        array_numbers[index] = 0;

    }

}

void fill_with_5(img *image, int mama,
                int row, int col, int **matrix, int *array_numbers){

    int index;

    index = matrix[row][col] - 1;
    if(array_numbers[index] == 0){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 1){

        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 2){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 3){

        image->array[mama + MODIFY_PX_5] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 4){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    }

    if(array_numbers[index] == 5){

        array_numbers[index] = 0;

    }

}

void fill_with_6(img *image, int mama,
                int row, int col, int **matrix, int *array_numbers){

    int index;

    index = matrix[row][col] - 1;
    if(array_numbers[index] == 0){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 1){

        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 2){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 3){

        image->array[mama + MODIFY_PX_5] = 0;
        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 4){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    }

    if(array_numbers[index] == 5){

        array_numbers[index] = 0;

    }

}

void fill_with_7(img *image, int mama,
                int row, int col, int **matrix, int *array_numbers){

    int var_cons, index;
    
    index = matrix[row][col] - 1;
    var_cons = array_numbers[index];
    if(array_numbers[index] == 0){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        var_cons = 1;
        array_numbers[index]++;

    } else if(array_numbers[index] == var_cons){

        image->array[mama + MODIFY_PX_5] = 0;
        array_numbers[index]++;
        var_cons++;

    }

    if(var_cons == 5){

        array_numbers[index] = 0;

    }

}

void fill_with_8(img *image, int mama,
                int row, int col, int **matrix, int *array_numbers){

    int index;

    index = matrix[row][col] - 1;
    if(array_numbers[index] == 0){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 1){

        image->array[mama + MODIFY_PX_5] = 0;
        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 2){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 3){

        image->array[mama + MODIFY_PX_5] = 0;
        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 4){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    }

    if(array_numbers[index] == 5){

        array_numbers[index] = 0;

    }

}

void fill_with_9(img *image, int mama,
                int row, int col, int **matrix, int *array_numbers){

    int index;

    index = matrix[row][col] - 1;
    if(array_numbers[index] == 0){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 1){

        image->array[mama + MODIFY_PX_5] = 0;
        image->array[mama + MODIFY_PX_1] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 2){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    } else if(array_numbers[index] == 3){

        image->array[mama + MODIFY_PX_5] = 0;
        array_numbers[index]++;

    } else if(array_numbers[index] == 4){

        for(int i = 0; i < BIT_LINE_SQ; i += JUMP_NEXT_PX){

            image->array[mama + i + MODIFY_PX_1] = 0;

        }

        array_numbers[index]++;

    }

    if(array_numbers[index] == 5){

        array_numbers[index] = 0;

    }

}

//Pattern for each number
void modify_pixel(int row, int col, img *image,
                int mama, int **matrix, int *array_numbers){

    if(matrix[row][col] == 1){

        fill_with_1(image, mama);

    } else if(matrix[row][col] == 2){

        fill_with_2(image, mama, row, col, matrix, array_numbers);

    } else if(matrix[row][col] == 3){

        fill_with_3(image, mama, row, col, matrix, array_numbers);

    } else if(matrix[row][col] == 4){

        fill_with_4(image, mama, row, col, matrix, array_numbers);

    } else if(matrix[row][col] == 5){

        fill_with_5(image, mama, row, col, matrix, array_numbers);

    } else if(matrix[row][col] == 6){

        fill_with_6(image, mama, row, col, matrix, array_numbers);

    } else if(matrix[row][col] == 7){

        fill_with_7(image, mama, row, col, matrix, array_numbers);

    } else if(matrix[row][col] == 8){

        fill_with_8(image, mama, row, col, matrix, array_numbers);

    } else if(matrix[row][col] == 9){

        fill_with_9(image, mama, row, col, matrix, array_numbers);

    }

}

//Change certain values of bitmap to write magenta numbers
void modify_bitmap(img *image, int **good_matrix,
                    unsigned char *array, int *array_numbers){

    int ver = FIRST_TO_PASS, increase, col = 0, row = 0, num_pad, mama;

    num_pad = image->info.biSizeImage / image->info.width - 1;
    for(int i = 2; i < image->info.height - 2; i++){

        if(i == ver){

            i += JUMP_IMPORTANT_ROW;
            ver += NEXT_FIRST_PASS;

        }

        row = (i - 2) / 8;

        for(unsigned int j = 6 + i * num_pad; j < (i + 1) * num_pad; j++){

            if(col == NEXT_ROW){

                col = 0;

            }

            increase = 0;
            mama = j;

            for(int k = 0; k < BIT_LINE_SQ; k++){

                array[k] = image->array[j];
                j++;

            }

            for(int k = 0; k < BIT_LINE_SQ; k += JUMP_IMPORTANT_ROW){
                
                //If line is white
                if((array[k] == WHITE) &&
                            (array[k + 1] == WHITE) &&
                                        (array[k + 2] == WHITE)){

                    increase++;

                }

                if(increase == WHITE_LINE_SQ){
                    
                    //Complet white square with number which fits
                    modify_pixel(row, col, image, mama, good_matrix, array_numbers);

                }

            }

            j += NEXT_NUMBER_SQ;
            col++;

        }

    }

}

//Create matrix mask from matrix array so that matrix is not inverted any longer
void create_good_matrix(int **matrix, int *array){

    int index = 0;

    for(int i = 0; i < MAX_9x9; i++){

        for(int j = 0; j < MAX_9x9; j++){

            matrix[j][i] = array[index];
            index++;

        }

    }

}

//Fill matrix elements with numbers that fit
void fill_white(int **good_matrix){

    int flag;

    //To be able to write any number, they must respect all three rules
    for(int i = 0; i < MAX_9x9; i++){

        for(int  j = 0; j < MAX_9x9; j++){

            flag = 0;

            if(good_matrix[i][j] == 0){

                //Verify row rule
                for(int q = 1; q <= 9; q++){

                    flag = 1;

                    good_matrix[i][j] = q;

                    for(int l = 0; l < 9; l++){

                        if(l == j){

                        } else if(good_matrix[i][l] == good_matrix[i][j]){

                            good_matrix[i][j] = 0;
                            flag = 0;
                            break;

                        }

                    }

                    if(flag == 1){

                        //Verify column rule
                        for(int l = 0; l < 9; l++){

                            if(l == i){

                            } else if(good_matrix[l][j] == good_matrix[i][j]){

                                good_matrix[i][j] = 0;
                                flag = 0;
                                break;

                            }

                        }

                    }

                    if(flag == 1){

                        //Verify square rule
                        for(int l = i - i % 3; l < i - i % 3 + 3; l++){

                            for(int s = j - j % 3; s < j - j % 3 + 3; s++){

                                if((l == i) && (s == j)){

                                } else if(good_matrix[l][s] == good_matrix[i][j]){

                                    good_matrix[i][j] = 0;
                                    flag = 0;
                                    break;

                                }

                            }

                        }

                    }

                    if(flag == 1){

                        break;

                    }

                }
                
            }

        }

    }

}

int file_size(FILE *inputFile){

    int size_buffer;

    fseek(inputFile, 0L, SEEK_END);
    size_buffer = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);

    return size_buffer;

}

int Task1(char *argv, int *index, img *image){

    FILE *inputFile, *outputFile;
    char *buffer, *output;
    int flag_task1, size_buffer;
    cJSON *try;

    inputFile = fopen(argv, "rt");
    if(inputFile == NULL){

        free(image);

        printf("Error opening file\n");
        exit(1);

    }

    size_buffer = file_size(inputFile);

    buffer = calloc(size_buffer + 1, sizeof(char));
    if(buffer == NULL){

        fclose(inputFile);
        free(image);

        printf("Error with buffer\n");
        exit(2);

    }

    fscanf(inputFile, "%s", buffer);

    //Get all info from file JSON fromat
    try = cJSON_Parse(buffer);
    if (try == NULL){

        free(buffer);
        fclose(inputFile);
        free(image);

        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(3);
    }

    output = calloc(len_out, sizeof(char));
    if(output == NULL){

        cJSON_Delete(try);
        free(buffer);
        fclose(inputFile);
        free(image);

        printf("Error creating output name\n");
        exit(4);

    }

    //Name of file meant to be written
    sprintf(output, "./output_task1_board%c%c.bmp", argv[19], argv[20]);
    outputFile = fopen(output, "wb");
    if(outputFile == NULL){

        free(output);
        cJSON_Delete(try);
        free(buffer);
        fclose(inputFile);
        free(image);

        printf("Error opening output file\n");
        exit(5);

    }

    //Read all info from file
    flag_task1 = read(try, image, index);
    if(flag_task1 != 0){

        free(output);
        cJSON_Delete(try);
        free(buffer);
        fclose(inputFile);
        free(image);

        return -1;

    }

    //Create BMP image
    write1(image, index, outputFile);
    
    free(output);
    cJSON_Delete(try);
    free(buffer);
    fclose(inputFile);
    fclose(outputFile);

    return 0;

}

void Task2(char *argv, int *index, img *image){

    char *output, k;
    FILE *outputFile;

    //Help point to the part of the bitmap meant to be written
    k = *index;
    output = calloc(len_out, sizeof(char));
    if(output == NULL){

        free(image);

        printf("Error creating output name\n");
        exit(4);

    }

    sprintf(output, "./output_task2_board%c%c.bmp", argv[19], argv[20]);

    outputFile = fopen(output, "wb");
    if(outputFile == NULL){

        free(output);
        free(image);

        printf("Error opening output file\n");
        exit(5);

    }

    //Create mirrored BMP
    write2(image, index, outputFile);

    free(output);
    fclose(outputFile);

}

int Task3(char *argv, img *image){

    //ho is index of matrix which is actually an array
    //Matrix is mask of board
    unsigned char *array;
    int *matrix, flag, ho = 0, flag_other_hand, flag_task3;

    array = calloc(BIT_LINE_SQ, sizeof(unsigned char));
    if(array == NULL){

        free_parameters(matrix, array, image);

        printf("Array for matrix broken\n");
        exit(6);

    }

    matrix = calloc(ALL_IN_ONE, sizeof(int));
    if(matrix == NULL){

        free_parameters(matrix, array, image);

        printf("No memory for matrix\n");
        exit(7);

    }

    //If board has white spaces -> loss
    //Else continue checking rules
    flag = white(image, array);
    if(flag == 0){

        flag_task3 = write_loss_task3(argv);
        if(flag_task3 != 0){

            free_parameters(matrix, array, image);

            return -1;

        }

    } else{

        for(int i = 0; i < MAX_9x9; i++){
            
            create_column(image, array, matrix, i, &ho);

        }

        flag_task3 = verify_column(matrix, &flag_other_hand, argv);
        if(flag_task3 != 0){

            free_parameters(matrix, array, image);

            return -1;

        }

        if(flag_other_hand == 0){

            flag_task3 = verify_row(matrix, &flag_other_hand, argv);
            if(flag_task3 != 0){

                free_parameters(matrix, array, image);

                return -1;

            }

        }
        
        if(flag_other_hand == 0){

            flag_task3 = verify_square(matrix, &flag_other_hand, argv);
            if(flag_task3 != 0){

                free_parameters(matrix, array, image);

                return -1;

            }

        }
        
        if(flag_other_hand == 0){

            flag_task3 = write_win_task3(argv);
            if(flag_task3 != 0){

                free_parameters(matrix, array, image);

                return -1;

            }

        }

        free(matrix);
        free(array);
        free(image->array);
    }

    return 0;

}

//Info about Task4 in README and functions
int Task4(char *argv, img *image){

    int k, *matrix, ho = 0, **good_matrix, *array_numbers, mama, size_buffer;
    FILE *inputFile, *outputFile;
    unsigned char *array;
    char *buffer;

    inputFile = fopen(argv, "rb");
    if(inputFile == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 8;

    }

    size_buffer = file_size(inputFile);
    fread(&(image->file), sizeof(image->file), 1, inputFile);
    fread(&(image->info), sizeof(image->info), 1, inputFile);
    k = image->info.biSizeImage - image->info.height;
    image->array = calloc(image->info.biSizeImage, sizeof(unsigned char));
    if(image->array == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 10;

    }

    for(int j = 1; j <= image->info.height; j++){
        fread(&(image->array[(k / image->info.width) * (image->info.height - j)]), 1, image->info.biSizeImage / image->info.height, inputFile);

    }

    array = calloc(BIT_LINE_SQ, sizeof(unsigned char));
    if(array == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 6;

    }

    matrix = calloc(ALL_IN_ONE, sizeof(int));
    if(matrix == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 7;

    }

    for(int i = 0; i < 9; i++){     
        create_column(image, array, matrix, i, &ho);

    }

    good_matrix = calloc(MAX_9x9, sizeof(int*));
    if(good_matrix == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 13;

    }

    for(int i = 0; i < MAX_9x9; i++){
        good_matrix[i] = calloc(MAX_9x9, sizeof(int));
        if(good_matrix[i] == NULL){
            free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
            return 14;

        }

    }

    create_good_matrix(good_matrix, matrix);
    fill_white(good_matrix);
    array_numbers = calloc(MAX_9x9, sizeof(int));
    if(array_numbers == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 11;

    }
    
    modify_bitmap(image, good_matrix, array, array_numbers);
    buffer = calloc(size_buffer + 1, sizeof(char));
    if(buffer == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 16;

    }

    sprintf(buffer, "./output_task4_board%c%c.bmp", argv[19], argv[20]);
    outputFile = fopen(buffer, "wb");
    if(outputFile == NULL){
        free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
        return 15;

    }

    write1(image, &k, outputFile);
    free_parameters_4(buffer, array_numbers, good_matrix,
                        matrix, array, image, inputFile, outputFile);
    return 0;

}