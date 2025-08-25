/*
 * SPDX-FileCopyrightText: 1996, 1998, 1999, 2002, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */

/*!
 * \file phase_align.c
 * \brief Phase alignment for 8-bit bytes.
 * This source file implements the functions for managing phase alignment of
 * 8-bit bytes, as defined in the `phase_align.h` header file.
 */

#include <phase_align.h>

/*
 * MISRA-C compliance for rule 8.4: static functions should be declared before
 * use. The functions `fetch_left_shift`, `fetch`, and `fetch_right_shift` are
 * declared before they are defined to ensure that they can be used within the
 * `phase_align_start` and `phase_align_fetch` functions without any issues.
 * This is necessary to avoid implicit function declarations, which can lead to
 * undefined behavior and is against the C11 standard.
 */
static uint8_t fetch_left_shift(struct phase_align *pa);
static uint8_t fetch(struct phase_align *pa);
static uint8_t fetch_right_shift(struct phase_align *pa);

/*
 * Decide how to fetch the source bits. There are three phase-align functions to
 * choose from, based on how the bits are out of phase. The destination
 * alignment is x & 7, i.e. how many bits from the left side of the byte.
 * Expression x_store & 7 gives the source alignment. The sign and magnitude of
 * the difference between the alignments determines the direction and amount of
 * shift.
 *
 * Note that only the least significant three bits of x are used since the
 * output is always a byte (8 bits). The shift value is calculated based on the
 * difference between the destination x and the source x_store. The output
 * stream appears as a sequence of bytes returned by the phase_align_fetch
 * function, where each byte is shifted according to the specified shift value.
 */
void phase_align_start(struct phase_align *pa, int x, int x_store, const uint8_t *store)
{
    pa->store = store + (x_store >> 3);
    int shift = (x & 7) - (x_store & 7);
    if (shift < 0)
    {
        pa->fetch = &fetch_left_shift;
        pa->shift = -shift;
        pa->carry = *pa->store;
    }
    else if (shift == 0)
    {
        pa->fetch = &fetch;
        /*
         * Shift and carry are not used when there is no shift.
         * Be tidy and set them to zero.
         */
        pa->shift = 0;
        pa->carry = 0x00U;
    }
    else
    {
        /*
         * Carry starts off as undefined for right shifts. Make it zero so that
         * the first byte fetched does not have any undefined overflow from the
         * previous non-byte.
         */
        pa->fetch = &fetch_right_shift;
        pa->shift = shift;
        pa->carry = 0x00U;
    }
}

uint8_t phase_align_fetch(struct phase_align *pa)
{
    return (*pa->fetch)(pa);
}

/*!
 * \brief Fetches a 8-bit byte from the phase alignment structure with a left shift.
 * \param pa Pointer to the phase alignment structure.
 * \return The fetched 8-bit byte.
 */
static uint8_t fetch_left_shift(struct phase_align *pa)
{
    const uint8_t lo = *++pa->store; // pre-increment
    const uint8_t hi = pa->carry;    // carry is the previous value
    pa->carry = lo;                  // store the current value as carry for the next call
    return (hi << pa->shift) | (lo >> (8 - pa->shift));
}

/*!
 * \brief Fetches a 8-bit byte from the phase alignment structure with no shift.
 * \param pa Pointer to the phase alignment structure.
 * \return The fetched 8-bit byte.
 */
static uint8_t fetch(struct phase_align *pa)
{
    return *pa->store++;
}

/*!
 * \brief Fetches a 8-bit byte from the phase alignment structure with a right shift.
 * \param pa Pointer to the phase alignment structure.
 * \return The fetched 8-bit byte.
 */
uint8_t fetch_right_shift(struct phase_align *pa)
{
    const uint8_t lo = *pa->store++; // post-increment
    const uint8_t hi = pa->carry;    // carry is the previous value
    pa->carry = lo;                  // store the current value as carry for the next call
    return (hi << (8 - pa->shift)) | (lo >> pa->shift);
}
