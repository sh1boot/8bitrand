#include <stdint.h>

#include "8bitrand.h"

#define MUL8x8(a, b) (uint16_t)((uint8_t)(a) * (uint8_t)(b))

static struct
{
    uint8_t state[SEED_MAX]; /* would normally be smaller */
    uint16_t c;
    /* Could be folded into c if storage is tight */
    uint8_t i;

    /* Would normally be compile-time constants */
    uint8_t r;
    uint8_t mul_lo;
    uint8_t mul_hi;
} rand8;

void rand8_init(uint16_t a, uint8_t r, uint16_t c, uint8_t const *seed)
{
    int i;

    rand8.mul_lo = a & 0xff;
    rand8.mul_hi = a >> 8;
    rand8.r = r;

    for (i = 0; i < r; i++)
        rand8.state[i] = seed[i];

    rand8.c = c;
    rand8.i = 0;
}

/* Use this version for multipliers less than 256.
 */
uint8_t rand8_8bit(void)
{
    uint8_t i = rand8.i;
    uint8_t x = rand8.state[i];
    uint16_t c = rand8.c;
    uint16_t t;

    t = MUL8x8(x, rand8.mul_lo) + c;
    c = t >> 8;
    x = t & 255;
    rand8.state[i] = x;
    rand8.c = c;
    if (++i >= rand8.r)
        i = 0;
    rand8.i = i;
    return x;
}

uint16_t rand16_8bit(void)
{
    uint8_t x, y;
    x = rand8_8bit();
    y = rand8_8bit();
    return ((uint16_t)y << 8) + x;
}

uint32_t rand32_8bit(void)
{
    uint16_t x, y;
    x = rand16_8bit();
    y = rand16_8bit();
    return ((uint32_t)y << 16) + x;
}


/* Use this version for multipliers between 256 and 511.
 */
uint8_t rand8_9bit(void)
{
    int i = rand8.i;
    uint8_t x = rand8.state[i];
    uint16_t c = rand8.c;
    uint16_t t;

    t = MUL8x8(x, rand8.mul_lo) + c;
    c = t >> 8;
    c += x;
    x = t & 255;
    rand8.state[i] = x;
    rand8.c = c;
    if (++i >= rand8.r)
        i = 0;
    rand8.i = i;
    return x;
}

uint16_t rand16_9bit(void)
{
    uint8_t x, y;
    x = rand8_9bit();
    y = rand8_9bit();
    return ((uint16_t)y << 8) + x;
}

uint32_t rand32_9bit(void)
{
    uint16_t x, y;
    x = rand16_9bit();
    y = rand16_9bit();
    return ((uint32_t)y << 16) + x;
}


/* Use this version for multipliers 512 and larger.
 */
uint8_t rand8_16bit(void)
{
    int i = rand8.i;
    uint8_t x = rand8.state[i];
    uint16_t c = rand8.c;
    uint16_t t;

    t = MUL8x8(x, rand8.mul_lo) + c;
    c = t >> 8;
    c += MUL8x8(x, rand8.mul_hi);
    x = t & 255;
    rand8.state[i] = x;
    rand8.c = c;
    if (++i >= rand8.r)
        i = 0;
    rand8.i = i;
    return x;
}

uint16_t rand16_16bit(void)
{
    uint8_t x, y;
    x = rand8_16bit();
    y = rand8_16bit();
    return ((uint16_t)y << 8) + x;
}

uint32_t rand32_16bit(void)
{
    uint16_t x, y;
    x = rand16_16bit();
    y = rand16_16bit();
    return ((uint32_t)y << 16) + x;
}
