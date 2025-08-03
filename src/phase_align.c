/*
 * SPDX-FileCopyrightText: 1996, 1998, 1999, 2002, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */

 #include <phase_align.h>

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

void phase_align_start(struct phase_align *pa, int x, int x_store, const uint8_t *store)
{
    pa->store = store + (x_store >> 3);
    int shift = (x & 0x7U) - (x_store & 0x7U);
    if (shift < 0)
    {
        pa->fetch = &fetch_left_shift;
        pa->shift = -shift;
        pa->carry = *pa->store;
    }
    else if (shift == 0)
    {
        pa->fetch = &fetch;
    }
    else
    {
        /*
         * Carry starts off as undefined for right shifts. The first byte
         * fetched will be the most significant byte of the first byte in the
         * store buffer, and subsequent calls will use the carry from the
         * previous fetch.
         */
        pa->fetch = &fetch_right_shift;
        pa->shift = shift;
    }
}

uint8_t phase_align_fetch(struct phase_align *pa)
{
    return (*pa->fetch)(pa);
}
