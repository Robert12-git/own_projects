#include "hunt.h"
#include <stdio.h>

/* Task 1 - Axii */

uint16_t find_spell(uint64_t memory)
{
    // Variable meant to count bits of 1
    uint8_t k = 0;

    uint16_t spell = 0;
    uint16_t mask16, one16 = 1;
    uint64_t mask = 0, one = 1;

    for(uint8_t i = 0; i <= 63 ; i++){

        mask = one << i;

        if(memory & mask){

            k++;

        } else {

            k = 0;

        }

        // Identifying if there are 5 consecutive bits of 1
        if(k == 5){

            for(uint16_t j = 0; j <= 15; j++){

                // Increment to find positions of the spell's bits
                i++;

                // Reimplementation of the mask for parameter memory
                mask = one << i;

                mask16 = one16 << j;

                if(memory & mask){

                    spell = spell | mask16;

                }

            }

            // Spell is done, stop looping
            break;

        }

    }

    /*
     * The spell is 16 bits and they are in the memory exactly after 5
     * consecutive bits of 1:
     *
     *            high                                             low
     *                [][][x]...[x][x][x][1][1][1][1][1][]...[][][]
     *                    |----16bits---|
     *                          Spell
     *
     * There is only 1 group of 5 consecutive bits of 1 and the other groups
     * have less than consecutive bits of 1
     * Steps:
     * - find 5 consecutive bits of 1
     * - the NEXT 16 bits == spell
     */

    uint16_t res = -1;

    res = spell;

    /* TODO */

    return res;
}


uint16_t find_key(uint64_t memory)
{
    // Variable meant to count bits of 1
    uint8_t k = 0;

    uint16_t key = 0;
    uint16_t mask16, one16 = 1;
    uint64_t mask = 0, one = 1;

    for(int i = 63; i >= 0 ; i--){

        mask = one << i;

        if(memory & mask){

            k++;

        } else {

            k = 0;

        }
        // Identifying if there are 3 consecutive bits of 1
        if(k == 3){

            for(int j = 15; j >= 0; j--){

                // Decrement to find positions of the key's bits
                i--;

                // Reimplementation of the mask for parameter memory
                mask = one << i;

                mask16 = one16 << j;

                if(memory & mask){

                    key = key | mask16;

                }

            }

            // Key is done, stop looping
            break;

        }

    }
    /*
     * The key is 16 bits and they are in the memory exactly before 3
     * consecutive bits of 1:
     *
     *                high                                           low
     *                     [][][][1][1][1][x][x]...[x][x][]...[][][]
     *                                    |----16bits---|
     *                                           Key
     * Steps:
     * - find 3 consecutive bits of 1
     * - the PREVIOUS 16 bits == key
     */

    uint16_t res = -1;

    res = key;

    /* TODO */

    return res;
}


uint16_t decrypt_spell(uint16_t spell, uint16_t key)
{
    /*
     * Find the spell knowing that
     * spell_encrypted = spell_plaintext ^ key
     */

    uint16_t res = -1;

    res = spell ^ key;

    /* TODO */

    return res;
}


/* Task 2 - Sword Play */

uint32_t choose_sword(uint16_t enemy)
{
    /*
     * Help Geralt construct a sword that will give him an advantage in battle.
     * The steps we will follow:
     * - choose the corect sword
     * - brand the sword with the correct runes such that the equation
     * for that specific enemy type is true
     *
     * How does the sword look (Sword template):
     *  [][][][] [0][0][0]...[0][0][0] [][][][][][][][][][][][][][][][]
     *  -------- --------------------- --------------------------------
     *     Type    Does not matter      The "Value" such that the equation
     *                               from 2 holds depending on the enemy type
     *    4 bits      12 bits                     16 bits
     *
     * 1. Type of the sword:
     * - 1001 - Silver Sword - for monsters
     * - 0110 - Steel Sword - for humans
     *
     *  If the number of active bits from variable "enemy" is even == Monster
     *  else == Human
     *
     * 2.
     * Monster Equation:
     *    Value ^ (Enemy & (1 - Enemy)) = 0
     *
     * Human Equation:
     *    Value + Enemy = 0
     */

    /* Implementation:
     * 
     * -determination of the number of the bits from parameter enemy
     * needs to be done in order to specify the type of enemy
     * 
     * -to determine value from monster equation, it seems that a xor equation
     * is 0 only if its terms are equivalent, which means that
     * value = (enemy & (1 - enemy))
     * 
     * -to determine value from human equation, it is well known that
     * the sum of 2 values is 0  only when they have opposite values, which 
     * means that value = -enemy
    */

    uint32_t res = -1, type_enemy = 0;
    uint16_t value;
    
    res = 0;

    for(int i = 0; i <= 15; i++){

        if(enemy & (1 << i)){

            type_enemy++;

        }

    }

    if(type_enemy % 2){

        value = -enemy;

        // Sword for humans has bits 30 and 29 setted
        res = res | (1 << 30);
        res = res | (1 << 29);

        res = res | value;

    } else{

        value = enemy & (1 - enemy);

        // Sword for monsters has bits 31 and 28 setted
        res = res | (1 << 31);
        res = res | (1 << 28);

        res = res | value;

    }

    /* TODO */

    return res;
}


/* Task 3 - The Witcher Trials */

uint32_t trial_of_the_grasses(uint16_t cocktail)
{
    /*
     * To become a witcher one must survive a cocktail of different
     * herbs. The body should generate the correct antibodies to
     * neutralize the given cocktail.
     *
     *
     * The antibodies need to respect the following properties:
     *   (antibodies_high & cocktail) ^ (antibodies_low | cocktail) = 0
     *   antibodies_low & antibodies_high = 0
     *   antibodies_low | antibodies_high = cocktail
     *
     * Where:
     *  [][][]...[][][] | [][][]...[][][]
     *  ---------------   ---------------
     *  antibodies_high    antibodies_low
     *      16 bits           16 bits
     *      -------------------------
     *              antibodies
     */


    /* Implementation:
     * 
     * -after making the Truth Table it seems that there are 2 conditions
     * needed to verify the equations:
     * 
     * i) antibodies_low has only unsetted bits
     * 
     * ii) antibodies_high has the same bits setted as the parameter cocktail
     * 
     * -after identifying what should represent antibodies_high and
     * antibodies_low, they should be merged in the variable meant
     * to be returned
     * 
     * -antibodies_low represents the first 16 bits from res, and
     * antibodies_high represents the last 16 bits from res
    */

    uint32_t res = -1;

    res = 0;

    for(int i = 0; i <= 15; i++){

        if(cocktail & (1 << i)){

            // (i + 16) needed to set the last 16 bits from res
            res = res | (1 << (i + 16));

        }

    }

    /* TODO */

    return res;
}


uint8_t trial_of_forrest_eyes(uint64_t map)
{
    /*
     * For the next trail, the candidate is tied up and blindfolded.
     * They are also taken to a forest and they should return until
     * the next morning.
     *
     * The candidate knows that there are 4 possibilities for the forest:
     * Brokilon, Hindar, Wolven Glade and Caed Dhu.
     *
     * The candidate also knows those forests by heart, but first you
     * need to identify the forest.
     *
     * The characteristics for 3 of the forests are as follows:
     * - Brokilon - has ONLY groups of 4 trees (or a forest without any tree)
     *   Ex: a group of 4 trees: "...00111100..."
     * - Hindar - has ONLY 2 trees in the MIDDLE
     * - Wolven Glade - FULL of trees
     *
     * The "map" variable encodes the following information:
     * - 1 tree
     * - 0 patch of grass
     *
     * You should put in the "res" variable the index for the identified
     * forrest.
     * - Brokilon - index 0
     * - Hindar - index 1
     * - Wolven Glade - index 2
     * - Caed Dhu - index 3
     */

    uint8_t res = -1, bit_set = 0, group_set = 0;

    uint64_t mid1_set;
    mid1_set = map & ((uint64_t)1 << (uint64_t)31);

    uint64_t mid2_set;
    mid2_set = map & ((uint64_t)1 << (uint64_t)32);

    // Brokilon case
    res = 0;

    // Analyze of map to see if it has groups of 4 bits setted
    for(int i = 0; i <= 63; i++){

        if(map & ((uint64_t)1 << i)){

            // Count each setted bit until loop stops
            bit_set++;

            // Count as many setted bits as possible to verify groups of bits
            group_set++;

        // After finding an unsetted bit
        } else{

            // To unplease the Broklin forest, group_set < 4 || group_set > 4
            if((group_set != 4) && (group_set != 0)){

                // Caed Dhu case
                res = 3;

                // Found exception, stop running loop
                break;
            }

            // It gets here only when it was found group of 4 or no bit setted
            group_set = 0;

        }

    }

    if(bit_set == 64){

        // Wolven Glade case
        res = 2;

    } else if((bit_set == 2) && (mid1_set != 0) && (mid2_set != 0)){

        // Hindar case
        res = 1;

    }
    
    /* TODO */

    return res;
}


uint8_t trial_of_the_dreams(uint32_t map)
{
    /*
     * For this final trial, the candidates go into a trance and when
     * they woke up they would be a fully fledged witcher.
     *
     * For this task, you need to help the witcher find the number
     * of steps that would take the candidate from the dream realm to
     * the real world.
     *
     * The "map" variable would have only 2 bits of 1:
     * - candidate location
     * - portal location
     *
     * You should put in the "res" variable the distance from the
     * candidate position to the portal.
     *
     *  Ex:
     *  [0][0][0][0][0][0]....[1][0][0][0][0][1][0]....[0][0][0]
     *
     *  res = 5
     */

    uint8_t res = -1 , witcher_position, portal, cont = 0;

    for(int i = 0; i <= 31; i++){

        // Identify first setted bit which represents witcher's position
        if(((map & (1 << i)) != 0) && (cont == 0)){

            witcher_position = i;
            cont = 1;

          // Identify second setted bit which represents portal's position
        } else if(map & (1 << i)){

            portal = i;

            // Both positions identified, get out of the loop
            break;

        }

    }

    // Distance from positions
    res = portal - witcher_position;


    /* TODO */

    return res;
}
