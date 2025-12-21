#include <assert.h>

#include <phase_align.h>

static inline uint32_t bswap_32(uint32_t xxxx)
{
    return ((xxxx >> 24) & 0x000000ffU) |
           ((xxxx >> 8) & 0x0000ff00U) |
           ((xxxx << 8) & 0x00ff0000U) |
           ((xxxx << 24) & 0xff000000U);
}

/*!
 * \brief Determines if the system is big-endian.
 * This is a common technique to check endianness, as it relies on the fact that
 * in a big-endian system, the most significant byte is stored at the lowest
 * address.
 *
 * Testing all but the first byte of a 32-bit integer to determine endianness
 * might seem redundant, but it ensures that the system's byte order is indeed
 * big-endian; and not something else entirely, not even little-endian.
 * \return 1 if big-endian, 0 if little-endian.
 */
static inline int is_big_endian(void)
{
    union
    {
        uint32_t xxxx;
        uint8_t x[4];
    } endian = {0x11223344U};
    return endian.x[0] == 0x11U &&
           endian.x[1] == 0x22U &&
           endian.x[2] == 0x33U &&
           endian.x[3] == 0x44U;
}

/*!
 * \brief Converts a 32-bit integer to big-endian byte order.
 * \param xxxx The 32-bit integer to convert.
 * \return The value in big-endian byte order.
 */
static inline uint32_t to_big_endian_32(uint32_t xxxx)
{
    return is_big_endian() ? xxxx : bswap_32(xxxx);
}

/*!
 * \brief Tests the phase alignment for little-endian 32-bit integers.
 * \details This function tests the phase alignment functionality for 32-bit
 * integers in little-endian byte order.
 *
 * It makes important assumptions about the byte order:
 *
 * - The least significant byte is stored first.
 * - The most significant byte is stored last.
 *
 * The compiler must ensure that the data is aligned correctly in memory. Hence,
 * it depends on the compiler's handling of endianness. It assumes a
 * little-endian architecture where the least significant byte is at the lowest
 * address.
 */
int test_le32(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    const uint32_t store[] = {
        0x12345678U, 0x9abcdef0U, 0x11223344U, 0x55667788U};
    /*
     * Not 0x78563412U.
     */
    assert(phase_align_le32(0, (const uint8_t *)store) == 0x12345678U);
    assert(phase_align_le32(8, (const uint8_t *)store) == 0xf0123456U);

    /*
     * Test phase alignment for each bit position. This tests the phase
     * alignment for each bit position from 0 to 7, and ensures that the correct
     * bits are fetched for each position. This is done by shifting the bits of
     * the store value and checking that the expected value matches the actual
     * value fetched.
     */
    for (int x = 0; x < 8; x++)
    {
        for (int x_store = 0; x_store < 32; x_store++)
        {
            const uint32_t store[2] = {to_big_endian_32(1UL << (31 - x_store)), 0x0UL};
            struct phase_align pa;
            phase_align_start(&pa, x, x_store, (const uint8_t *)store);
            const uint8_t expected = 1U << (7 - x);
            const uint8_t actual = phase_align_fetch(&pa);
            if (expected != actual)
            {
                assert(0 && "phase_align_fetch failed");
            }
        }
    }

    return 0;
}
