#include <assert.h>

#include <phase_align.h>

int test_phase_align_left_shift(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    struct phase_align pa;
    phase_align_start(&pa, 0, 4, data);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x23U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x45U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x67U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0x89U);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xabU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xcdU);
    assert((phase_align_fetch(&pa) & 0xffU) == 0xefU);
    return 0;
}
