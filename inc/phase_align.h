/*
 * SPDX-FileCopyrightText: 1996, 1998, 1999, 2002, Roy Ratcliffe, Northumberland, United Kingdom
 * SPDX-License-Identifier: MIT
 */

#ifndef PHASE_ALIGN_H
#define PHASE_ALIGN_H

#include <stdint.h>

/*!
 * \file phase_align.h
 * \brief Phase alignment for 8-bit bytes.
 * This header file defines the `phase_align` structure and functions for
 * managing phase alignment of 8-bit bytes. It allows for fetching bytes with a
 * specified shift, which is useful in scenarios where data needs to be aligned
 * or shifted for processing, such as in graphics or display memory management.
 */

/*!
 * \brief Phase alignment structure.
 * This structure is used to manage phase alignment for 8-bit bytes. It contains
 * a function pointer for fetching bytes, a pointer to the store, a carry value,
 * and a shift count.
 *
 * The `fetch` function pointer is used to retrieve the next byte in the
 * phase-aligned stream. The `store` pointer points to the data being processed.
 * The `shift` value indicates how many bits to shift the data. The `carry`
 * value is used to hold any overflow from the previous byte fetch.
 */
struct phase_align
{
    uint8_t (*fetch)(struct phase_align *phase_align); /*!< Fetch the next byte */
    const uint8_t *store;                              /*!< Pointer to the data being processed */
    int shift;                                         /*!< Number of bits to shift the data */
    uint8_t carry;                                     /*!< Overflow from the previous byte fetch */
};

/*!
 * \brief Initialises the phase alignment structure.
 * This function sets up the phase alignment structure with the given parameters.
 * \param pa Pointer to the phase alignment structure.
 * \param x The destination bit position.
 * \param x_store The source bit position.
 * \param store Pointer to the data buffer.
 */
void phase_align_start(struct phase_align *pa, int x, int x_store, const uint8_t *store);

/*!
 * \brief Fetches the next byte from the phase alignment structure.
 * \param pa Pointer to the phase alignment structure.
 * \return The fetched byte.
 */
uint8_t phase_align_fetch(struct phase_align *pa);

static inline uint8_t phase_align_byte(int x_store, const uint8_t *store)
{
    struct phase_align pa;
    phase_align_start(&pa, 0, x_store, store);
    return phase_align_fetch(&pa);
}

static inline uint16_t phase_align_be16(int x_store, const uint8_t *store)
{
    struct phase_align pa;
    phase_align_start(&pa, 0, x_store, store);
    return (phase_align_fetch(&pa) << 8) | phase_align_fetch(&pa);
}

static inline uint16_t phase_align_le16(int x_store, const uint8_t *store)
{
    struct phase_align pa;
    phase_align_start(&pa, 0, x_store, store);
    return phase_align_fetch(&pa) | (phase_align_fetch(&pa) << 8);
}

static inline uint32_t phase_align_be32(int x_store, const uint8_t *store)
{
    struct phase_align pa;
    phase_align_start(&pa, 0, x_store, store);
    return (phase_align_fetch(&pa) << 24) | (phase_align_fetch(&pa) << 16) | (phase_align_fetch(&pa) << 8) | phase_align_fetch(&pa);
}

static inline uint32_t phase_align_le32(int x_store, const uint8_t *store)
{
    struct phase_align pa;
    phase_align_start(&pa, 0, x_store, store);
    return phase_align_fetch(&pa) | (phase_align_fetch(&pa) << 8) | (phase_align_fetch(&pa) << 16) | (phase_align_fetch(&pa) << 24);
}

#endif /* PHASE_ALIGN_H */
