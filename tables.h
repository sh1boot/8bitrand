#if !defined(TABLES_H_INCLUDED)
#define TABLES_H_INCLUDED

#include <stdint.h>
#include "8bitrand.h"

enum
{
    WIDE_TABLE, NARROW_TABLE, UNSAFE_TABLE, NUM_TABLES
};

typedef struct
{
    int len;
    uint16_t const *tab;
} tabptr_t;

extern const tabptr_t multiplier_table[NUM_TABLES][SEED_MAX + 1];

#endif
