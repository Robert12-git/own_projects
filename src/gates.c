#include "gates.h"
#include <stdio.h>
#include <assert.h>

/* Task 1 - Bit by Bit */

// Verify if a bit is set or not (1 for set and 0 for unset)
uint8_t get_bit (uint64_t nr, uint8_t i) {

    assert (i <= 8 * sizeof nr);

    uint8_t res = -1;
    uint64_t a = 1; 

    if (nr & (a << i))
        res = 1;
    else
        res = 0;

    return res;

}


// Set or unset a given bit and return new number created
uint64_t flip_bit (uint64_t nr, uint8_t i) {

    assert (i <= 8 * sizeof nr);

    uint64_t res = -1;
    uint64_t a = 1;
    uint64_t mask = (a << i);

    if (nr & (a << i))
        nr = nr & ~mask;
    else
        nr = nr | mask;

    res = nr; 

    return res;
}

// Set given bit and return new number created
uint64_t activate_bit (uint64_t nr, uint8_t i) {

    assert (i <= 8 * sizeof nr);

    uint64_t res = 0xFF;
    uint64_t a = 1;
    uint64_t mask = a << i;

    if ((nr & mask) == 0)
        nr = nr | mask;

    res = nr;

    return res;

}

// If given bit is setted, unset it
uint64_t clear_bit (uint64_t nr, uint8_t i) {

    assert (i <= 8 * sizeof nr);

    uint64_t res = -1;
    uint64_t a = 1;
    uint64_t mask = a << i;

    if (nr & mask)
        nr = nr & ~mask;

    res = nr;

    return res;

}


/* Task 2 - One Gate to Rule Them All */
// All gates implemented using nand_gate(if necessarly)

uint8_t nand_gate (uint8_t a, uint8_t b) {

    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    return !(a & b);

}


uint8_t and_gate (uint8_t a, uint8_t b) {

    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    res = nand_gate (nand_gate (a, b), nand_gate (a, b));

    return res;

}


uint8_t not_gate (uint8_t a) {

    assert (a == 0 || a == 1);

    uint8_t res = -1;

    res = nand_gate (a, a);

    return res;

}


uint8_t or_gate (uint8_t a, uint8_t b) {

    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    res = nand_gate (nand_gate (a, a), nand_gate (b, b));

    return res;

}


uint8_t xor_gate (uint8_t a, uint8_t b) {

    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1, nand = nand_gate (a, b);

    res = nand_gate (nand_gate (nand, a), nand_gate (b, nand));

    return res;

}


/* Task 3 - Just Carry the Bit */

uint8_t full_adder (uint8_t a, uint8_t b, uint8_t c) {

    // Sum and carry will be saved on positions 1 and 0 in variable returned

    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);

    uint8_t res = -1, sumbit, carrybit;

    res = 0;
    sumbit = xor_gate(c, xor_gate(a, b));
    carrybit = or_gate(and_gate(a, b), and_gate(c, xor_gate(a, b)));

    if (sumbit)
        res = activate_bit(res, 1);

    if (carrybit)
        res = activate_bit(res, 0);

    return res;

}


uint64_t ripple_carry_adder (uint64_t a, uint64_t b) {

    uint64_t res = -1;
    
    res = 0;

    // Initialization of sumx which holds the sum and the carry
    uint8_t sumx, carrybit = 0;

    for (uint64_t i = 0; i <= 63; i++) {

        // Calculation of the number which holds the sum and carry bits
        sumx = full_adder (get_bit (a, i), get_bit (b, i), carrybit);

        // Getting the value of the carry bit
        carrybit = get_bit (sumx, 0);

        if (get_bit (sumx, 1))
            res = activate_bit(res, i);

    }

    // Stack overflow case
    if(carrybit)
        res = 0;

    return res;

}
