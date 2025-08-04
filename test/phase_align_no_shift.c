#include <assert.h>

#include <phase_align.h>

int test_phase_align_no_shift(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    uint8_t data[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    struct phase_align pa;
    phase_align_start(&pa, 0, 0, data);
    assert(phase_align_fetch(&pa) == 0x12);
    assert(phase_align_fetch(&pa) == 0x34);
    assert(phase_align_fetch(&pa) == 0x56);
    assert(phase_align_fetch(&pa) == 0x78);
    assert(phase_align_fetch(&pa) == 0x9A);
    assert(phase_align_fetch(&pa) == 0xBC);
    assert(phase_align_fetch(&pa) == 0xDE);
    assert(phase_align_fetch(&pa) == 0xF0);
    return 0;
}
