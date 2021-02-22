#include "communication.h"
#include "util_comm.h"
#include <stdio.h>
#include <string.h>


/* Task 1 - The Beginning */

void send_byte_message () {

    // Send ASCII code of the characters: R, I, C, K

    send_squanch ('R' - 64);
    send_squanch ('I' - 64);
    send_squanch ('C' - 64);
    send_squanch ('K' - 64);

}


void recv_byte_message (void) {

    /* Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     * To decode a character must add 64 to its value
     * to convert from ASCII code to value
     */

    uint8_t mesaj;

    // 5 steps to receive each character encoded
    for (uint8_t i = 0; i <= 4; i++) {

        mesaj = recv_squanch () + 64;
        fprintf(stdout, "%c", mesaj);

    }

}


void comm_byte () {

    /*
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */

    uint8_t mesaj;

    // 10 steps to receive and send 10 characters
    for (uint16_t i = 0; i <= 9; i++) {

        mesaj = recv_squanch() + 64;

        // Send each character encoded, twice
        send_squanch (mesaj - 64);
        send_squanch (mesaj - 64);

    }

}


/* Task 2 - Waiting for the Message */

void send_message () {

    /*
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */

    char c[11] = "HELLOTHERE";

    send_squanch (strlen (c) << 2);

    for (int i = 0; i <= strlen(c); i++) {
        
        send_squanch(c[i] - 64);

    }

}


void recv_message () {

    /*
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     */

    uint8_t lungime, mesaj;

    lungime = recv_squanch();

    // Shift the bits to decode the length
    lungime = lungime << 2;
    lungime = lungime >> 4;
    
    // Print the length decoded
    fprintf(stdout,"%d", lungime);

    for (uint8_t i = 0; i < lungime; i++) {

        mesaj = recv_squanch();
        fprintf(stdout,"%c", mesaj + 64);

    }

}


void comm_message () {

    /*
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */

    uint8_t mesaj,lungime;

    lungime = recv_squanch();

    // Shift the bits to decode the length
    lungime = lungime << 2;
    lungime = lungime >> 4;

    while (lungime) {

        mesaj = recv_squanch();
        lungime--;

    }

    // Now, mesaj holds the last character encoded received

    // Verify if the last character received represents the ASCII code of 'P'
    if ((char)(mesaj + 64) == 'P') {

        char send1[11] = "PICKLERICK";

        // Initialization of the length encoded
        uint8_t a = strlen ("PICKLERICK") << 2;

        send_squanch (a);

        for (uint8_t i = 0; i <= strlen ("PICKLERICK"); i++)
            send_squanch (send1[i] - 64);

    } else {

        char send2[12] = "VINDICATORS";

        // Initialization of the length encoded
        uint8_t b = strlen("VINDICATORS") << 2;

        send_squanch (b);

        for (uint8_t i = 0; i <= strlen("VINDICATORS"); i++)
            send_squanch (send2[i] - 64);

    }

}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2) {
    /* 
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     * 
     * Implememntation:
     * -odd bits from res are meant to hold bits from c1
     * -even bits from res are meant to hold bits from c2
    */

    uint8_t mesaj = 0;
    
    // Initialization of variables used to test bits from c1 and c2
    uint8_t bit1 = 0, bit2 = 0;

    // Initialization of the mask and the conditions meant to be verified
    uint8_t mask, ver1, ver2;

    for (uint8_t i = 0; i < 8; i ++) {
        
        mask = 1 << i;
        ver2 = c2 & (1 << bit2);
        ver1 = c1 & (1 << bit1);

        if (i % 2 == 0) {

            // Verify if bit from c2 is 1
            if (ver2)
                mesaj = mesaj | mask;

            // Change position in c2
            bit2++;

        } else if (i % 2 == 1) {

            // Verify if bit from c1 is 1
            if (ver1)
                mesaj = mesaj | mask; 

            // Change position in c1
            bit1++;

        }

    }

    // Send c1 and c2 merged
    send_squanch (mesaj);

}


uint8_t decode_squanch2 (uint8_t c) {
    
    /*
     * Decode the given byte:
     * - split the two characters as in the image from resources
     */

    /* Implementation:
     * 
     * -even bits from c are meant to be placed in first half of byte
     * 
     * -odd bits from c are meant to be placed in second half of byte
    */

    // Initialization of the decoded message and variables to be used in masks
    uint8_t for_decode = 0, bit2 =0, bit1 = 4;

    // Initialization of the masks and the condition meant to be verified
    uint8_t mask1, ver, mask2, mask;

    for(int i = 0; i < 8; i ++){

        mask = 1 << i;

        // Mask needed to set bit identified from first message
        mask1 = 1 << bit1;

        // Mask needed to set bit identified from second message
        mask2 = 1 << bit2;

        ver = c & mask;

        if(i % 2 == 0){

            if(ver){

                // Set bit from fisrt half of byte
                for_decode = for_decode | mask2; 

            }

            // Change position in first half of byte
            bit2++;

        } else if(i % 2 == 1){

            if(ver){

                // Set bit from second half of byte
                for_decode = for_decode | mask1;  

            }
            // Change position in second half of byte
            bit1++;

        }

    }

    uint8_t res = -1;
    res = for_decode;

    return res;
    
}
