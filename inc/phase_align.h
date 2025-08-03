#include <stdint.h>

/*!
 * \brief Phase alignment structure.
 * This structure is used to manage phase alignment for 8-bit bytes.
 * It contains a function pointer for fetching bytes, a pointer to the store,
 * a carry value, and a shift count.
 */
struct phase_align
{
    uint8_t (*fetch)(struct phase_align *phase_align);
    const uint8_t *store;
    int shift;
    uint8_t carry;
};

void phase_align_start(struct phase_align *pa, int x, int x_store, const uint8_t *store);

uint8_t phase_align_fetch(struct phase_align *pa);
