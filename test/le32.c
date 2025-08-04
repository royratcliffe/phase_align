#include <assert.h>

#include <phase_align.h>

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
