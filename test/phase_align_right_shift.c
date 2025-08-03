#include <assert.h>

#include <phase_align.h>

int test_phase_align_right_shift(int argc, char *argv[])
{
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
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
    assert((phase_align_fetch(&pa) & 0xffU) == 0xabU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xcdU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xefU);
    return 0;
}
