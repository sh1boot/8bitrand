#include <stdint.h>

#include "examples.h"

#define BASIC_MULTIPLIER 0xdc17
#define BASIC_LAG 8
#define WIDE_MULTIPLIER 0xfd00c8ul
#define WIDE_LAG 3
#define TINY_MULTIPLIER 0xce07
#define TINY_LAG 2
#define FAST_MULTIPLIER 0x013b
#define FAST_LAG 6
#define FAST2_MULTIPLIER 0x010D0001
#define FAST2_LAG 8

#define MUL_8x8(x, y) ((uint16_t)((uint8_t)(x) * (uint8_t)(y)))

#define FAST_PERMUTE(a, x, c) do { \
        uint16_t t = MUL_8x8(a, x) + c; \
        c = t >> 8; \
        if (a & 0x100) c += x; \
        x = t & 0xff; \
    } while (0)

#define GOOD_PERMUTE(a, x, c) do { \
        uint16_t t = MUL_8x8(a, x) + c; \
        c = t >> 8; \
        c += MUL_8x8((a >> 8), x); \
        x = t & 0xff; \
    } while (0)

#define WIDE_PERMUTE(a, x, c) do { \
        uint32_t t = MUL_8x8(a, x); \
        t |= (uint32_t)MUL_8x8((a >> 16), x) << 16; \
        t += c; \
        c = t >> 8; \
        x = t & 0xff; \
    } while (0)

#define REALLYWIDE_PERMUTE(a, x, c) do { \
        uint16_t t = x + c[0]; \
        uint8_t out = t & 255; \
        t >>= 8; /* just carry */ \
        t += c[1]; \
        c[0] = t & 255; \
        t >>= 8; /* just carry */ \
        t += c[2]; \
        t += MUL_8x8(a >> 16, x); \
        c[1] = t & 255; \
        t >>= 8; /* full 16-bit */ \
        t += c[3] & 1; \
        t += x; \
        c[2] = t & 255; \
        t >>= 8; /* just 1 bit */ \
        c[3] = t & 1; \
        x = out; \
    } while (0)


static uint8_t hash(uint8_t *out, uint8_t olen, uint8_t const *in, uint8_t ilen)
{
    uint8_t i, l;
    uint8_t *end = out + olen;
    uint16_t seed = 0x5555;

    l = ilen + olen;
    for (i = 0; i < l; i++)
    {
        /* TODO: think more carefully */
        seed += i;
        if (i < ilen) seed += *in++;
        if (i > olen) seed += *out << 8;
        seed = (seed ^ (seed >> 11)) * 0xe9;
        *out++ = seed >> 8;
        if (out >= end)
            out -= olen;
    }
    return seed & 0xff;
}


/* basic generator -- 11 bytes of RAM and two multiplies per byte of output */

static struct
{
    uint8_t x[BASIC_LAG];
    uint16_t c;
    uint8_t i;
} rand_state;

void basicrand_seed(void const *s, uint8_t l)
{
    rand_state.c = 0x5500 | hash(rand_state.x, sizeof(rand_state.x), s, l);
    rand_state.i = 0;
}

uint16_t basicrand(void)
{
    uint8_t x, y;
    uint16_t c;
    uint8_t i;

    c = rand_state.c;
    i = rand_state.i;

    x = rand_state.x[i + 0];
    y = rand_state.x[i + 1];

    GOOD_PERMUTE(BASIC_MULTIPLIER, x, c);
    GOOD_PERMUTE(BASIC_MULTIPLIER, y, c);

    rand_state.x[i++] = x;
    rand_state.x[i++] = y;

    i &= 6;

    rand_state.c = c;
    rand_state.i = i;

    return (y << 8) | x;
}


/* improved basic generator -- 6 bytes of RAM and two multiplies per byte of output */

static struct
{
    uint16_t c0;
    uint8_t c1;
    uint8_t x[WIDE_LAG];
} newrand_state;

void newrand_seed(void const *s, uint8_t l)
{
    newrand_state.c0 = 0x5500 | hash(newrand_state.x, sizeof(newrand_state.x), s, l);
    newrand_state.c1 = 0;
}

uint16_t newrand(void)
{
    uint8_t x, y, z;
    uint32_t c;

    c = newrand_state.c0 | ((uint32_t)newrand_state.c1 << 16);

    x = newrand_state.x[0];
    y = newrand_state.x[1];
    z = newrand_state.x[2];

    WIDE_PERMUTE(WIDE_MULTIPLIER, x, c);
    WIDE_PERMUTE(WIDE_MULTIPLIER, y, c);

    newrand_state.c0 = (uint16_t)c;
    newrand_state.c1 = (uint8_t)(c >> 16);

    newrand_state.x[0] = z;
    newrand_state.x[1] = x;
    newrand_state.x[2] = y;

    return (y << 8) | x;
}


/* Tiny generator -- 4 bytes of RAM, but two multiplies per byte */

static struct tinyrand_state_t
{
    uint8_t x[TINY_LAG];
    uint16_t c;
} tinyrand_state;

void tinyrand_seed(void const *s, uint8_t l)
{
    tinyrand_state.c = 0x5500 | hash(tinyrand_state.x, sizeof(tinyrand_state.x), s, l);
}

uint16_t tinyrand(void)
{
    uint8_t x, y;
    uint16_t c;

    c = tinyrand_state.c;
    x = tinyrand_state.x[0];
    y = tinyrand_state.x[1];

    GOOD_PERMUTE(TINY_MULTIPLIER, x, c);
    GOOD_PERMUTE(TINY_MULTIPLIER, y, c);

    tinyrand_state.c = c;
    tinyrand_state.x[0] = x;
    tinyrand_state.x[1] = y;

    return (y << 8) | x;
}



/* fast generator -- 8 bytes of RAM, one multiply per byte */

static struct
{
    uint8_t x[FAST_LAG];
    uint16_t c;
} fastrand_state;

void fastrand_seed(void const *s, uint8_t l)
{
    fastrand_state.c = 0x0080 | hash(fastrand_state.x, sizeof(fastrand_state.x), s, l);
}

uint16_t fastrand(void)
{
    uint8_t x, y;
    uint16_t c;
    uint8_t i;

    c = fastrand_state.c;
    i = (c >> 8) & 6;
    c &= 0x1ff;

    x = fastrand_state.x[i + 0];
    y = fastrand_state.x[i + 1];

    FAST_PERMUTE(FAST_MULTIPLIER, x, c);
    FAST_PERMUTE(FAST_MULTIPLIER, y, c);

    fastrand_state.x[i++] = x;
    fastrand_state.x[i++] = y;

    if (i == sizeof(fastrand_state.x))
        i = 0;
    fastrand_state.c = c | ((uint16_t)i << 8);

    return (y << 8) | x;
}


/* shift generator -- like fast, but using shifts instead of multipliers (so, not fast) */

void shiftrand_seed(void const *s, uint8_t l)
{
    fastrand_seed(s, l);
}

uint16_t shiftrand(void)
{
    uint8_t x, y, i;
    uint16_t c, t;

    c = fastrand_state.c;
    i = (c >> 8) & 6;
    c &= 0x1ff;

    x = fastrand_state.x[i + 0];
    y = fastrand_state.x[i + 1];

    t = (x << 6) - x - (x << 2) + c;
    c = (t >> 8) + x;
    x = t & 0xff;
    t = (y << 6) - y - (y << 2) + c;
    c = (t >> 8) + y;
    y = t & 0xff;

    fastrand_state.x[i++] = x;
    fastrand_state.x[i++] = y;

    if (i == sizeof(fastrand_state.x))
        i = 0;
    fastrand_state.c = c | ((uint16_t)i << 8);

    return (y << 8) | x;
}


/* another fast generator -- 12 bytes of RAM, one multiply per byte */

static struct
{
    uint8_t x[FAST2_LAG];
    uint8_t c[4];
} fastrand2_state;

void fastrand2_seed(void const *s, uint8_t l)
{
    fastrand2_state.c[0] = hash(fastrand2_state.x, sizeof(fastrand2_state.x), s, l);
    fastrand2_state.c[1] = 0x55;
    fastrand2_state.c[2] = 0;
    fastrand2_state.c[3] = 0;
}

uint16_t fastrand2(void)
{
    uint8_t x, y;
    uint8_t i;

    i = (fastrand2_state.c[3] >> 1);

    x = fastrand2_state.x[i + 0];
    y = fastrand2_state.x[i + 1];

    REALLYWIDE_PERMUTE(FAST2_MULTIPLIER, x, fastrand2_state.c);
    REALLYWIDE_PERMUTE(FAST2_MULTIPLIER, y, fastrand2_state.c);

    fastrand2_state.x[i++] = x;
    fastrand2_state.x[i++] = y;

    fastrand2_state.c[3] |= (i & 7) << 1;

    return (y << 8) | x;
}


example_ptr_t examples[6] =
{
    { basicrand_seed, basicrand, BASIC_MULTIPLIER, BASIC_LAG, rand_state.x, &rand_state.c },
    { newrand_seed, newrand, WIDE_MULTIPLIER, WIDE_LAG, newrand_state.x, &newrand_state.c0 },
    { tinyrand_seed, tinyrand, TINY_MULTIPLIER, TINY_LAG, tinyrand_state.x, &tinyrand_state.c },
    { fastrand_seed, fastrand, FAST_MULTIPLIER, FAST_LAG, fastrand_state.x, &fastrand_state.c },
    { shiftrand_seed, shiftrand, FAST_MULTIPLIER, FAST_LAG, fastrand_state.x, &fastrand_state.c },
    { fastrand2_seed, fastrand2, FAST2_MULTIPLIER, FAST2_LAG, fastrand2_state.x, (void *)fastrand2_state.c },
};
