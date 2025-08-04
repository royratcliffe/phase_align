#include <assert.h>

#include <phase_align.h>

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
int test_le32(int argc, char *argv[]) {
    const uint32_t store[] = {
        0x12345678U, 0x9abcdef0U, 0x11223344U, 0x55667788U
    };
    /*
     * Not 0x78563412U.
     */
    assert(phase_align_le32(0, (const uint8_t *)store) == 0x12345678U);
    assert(phase_align_le32(8, (const uint8_t *)store) == 0xf0123456U);
    return 0;
}
