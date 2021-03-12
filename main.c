#include "utils.h"

int main (int argc, char *argv[]) {
    if (argc != 2)
        return -1;

    int k=0, fun = 0, x_init, y_init;
    grid grid_game;
    FILE *input_data;

    input_data = fopen (argv[1], "rt");
    grid_game = *read_data (input_data, &grid_game);
    grid_game = *ant_sort (&grid_game);

    FILE *output = fopen ("output.txt", "wt");
    fprintf (stdout, "%d\n", grid_game.num_ant);
    for (int i = 0; i < grid_game.num_ant; i++) {
        fprintf (stdout, "%d %d %d\n", i, grid_game.builds[i].poz.poz_x, grid_game.builds[i].poz.poz_y);
    }
    x_init = grid_game.builds[k].poz.poz_x;
    y_init = grid_game.builds[k].poz.poz_y;
    // if (grid_game.num_ant < 8)
    // for (int j = 0; j < grid_game.num_ant; j++) {
    //         grid_game.builds[k].poz.poz_x = x_init;
    //         grid_game.builds[k].poz.poz_y = y_init;
    //         switch (j % 8) {
    //             case 1:
    //                 grid_game.builds[k].poz.poz_y++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
                
    //             case 2:
    //                 grid_game.builds[k].poz.poz_y--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
                
    //             case 3:
    //                 grid_game.builds[k].poz.poz_x--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 4:
    //                 grid_game.builds[k].poz.poz_x++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 5:
    //                 grid_game.builds[k].poz.poz_x--;
    //                 grid_game.builds[k].poz.poz_y--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 6:
    //                 grid_game.builds[k].poz.poz_x++;
    //                 grid_game.builds[k].poz.poz_y++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 7:
    //                 grid_game.builds[k].poz.poz_x--;
    //                 grid_game.builds[k].poz.poz_y++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
                
    //             case 0:
    //                 grid_game.builds[k].poz.poz_x++;
    //                 grid_game.builds[k].poz.poz_y--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
    //         }
    //     } else
    // for (int i = 8; i < grid_game.num_ant; i+=8) {
    //     k++;
    //     fun = i;
    //     x_init = grid_game.builds[k].poz.poz_x;
    //     y_init = grid_game.builds[k].poz.poz_y;
    //     for (int j = fun; j < fun + ((fun + 1) % 8); j++) {
    //         grid_game.builds[k].poz.poz_x = x_init;
    //         grid_game.builds[k].poz.poz_y = y_init;
    //         switch (j % 8) {
    //             case 1:
    //                 grid_game.builds[k].poz.poz_y++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
                
    //             case 2:
    //                 grid_game.builds[k].poz.poz_y--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
                
    //             case 3:
    //                 grid_game.builds[k].poz.poz_x--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 4:
    //                 grid_game.builds[k].poz.poz_x++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 5:
    //                 grid_game.builds[k].poz.poz_x--;
    //                 grid_game.builds[k].poz.poz_y--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 6:
    //                 grid_game.builds[k].poz.poz_x++;
    //                 grid_game.builds[k].poz.poz_y++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;

    //             case 7:
    //                 grid_game.builds[k].poz.poz_x--;
    //                 grid_game.builds[k].poz.poz_y++;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
                
    //             case 0:
    //                 grid_game.builds[k].poz.poz_x++;
    //                 grid_game.builds[k].poz.poz_y--;
    //                 fprintf (stdout, "%d %d %d\n", grid_game.antennas[j].id, grid_game.builds[k].poz.poz_x, grid_game.builds[k].poz.poz_y);
    //                 break;
    //         }
    //     }
    // }

    fclose (output);
    fclose (input_data);

    return 0;
}