#include <assert.h>

#include <phase_align.h>

int test_phase_align_right_shift(int argc, char *argv[])
{
    const uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0};
    struct phase_align pa;
    /*
     * Note that the destination x is 4, and the source x_store is 0.
     * This means we are shifting the source data right by 4 bits.
     * Bit 0 is the most significant bit of the first byte,
     * and bit 7 is the least significant bit of the last byte.
     *
     * The bit ordering mirrors the way we would expect
     * the bits to be aligned in a display device memory.
     */
    phase_align_start(&pa, 4, 0, data);
    assert(pa.shift == 4);
    assert((phase_align_fetch(&pa) & 0x0fU) == 0x01U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x23U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x45U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x67U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x89U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xabU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xcdU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xefU);

    phase_align_start(&pa, 0, 36, data);
    assert(pa.shift == 4);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xabU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xcdU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xefU);

    /*
     * Check that we can read the bits back in the same order.
     */
    for (int i = 0; i < 8; i++) {
        uint8_t expected = 0x00U;
        for (int x = 0; x < 8; x++) {
            /*
             * Position 7 is the least significant bit of the first byte,
             * and position 0 is the most significant bit of the last byte.
             * Bit position run from 0 to 7 as in a plane of pixels.
             */
            phase_align_start(&pa, 7, (i << 3) | x, data);
            expected |= (phase_align_fetch(&pa) & 0x01U) << (7 - x);
        }
        assert(expected == data[i]);
    }
    return 0;
}
