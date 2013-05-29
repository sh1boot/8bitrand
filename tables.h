#if !defined(TABLES_H_INCLUDED)
#define TABLES_H_INCLUDED

#include <stdint.h>

typedef struct
{
    int len;
    uint16_t const *tab;
} tabptr_t;

extern const tabptr_t wide_table[32];
extern const tabptr_t narrow_table[32];
extern const tabptr_t unsafe_table[32];

#endif
