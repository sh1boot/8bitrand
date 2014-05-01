#if !defined(EXAMPLES_H_INCLUDED)

#include <stdint.h>

#define EXAMPLE_RAND_MAX (uint16_t)0xffff

/* basic generator -- 11 bytes of RAM and two multiplies per byte of output */

void basicrand_seed(void const *s, uint8_t l);
uint16_t basicrand(void);


/* Tiny generator -- 4 bytes of RAM, but two multiplies per byte */

void tinyrand_seed(void const *s, uint8_t l);
uint16_t tinyrand(void);


/* fast generator -- 8 bytes of RAM, one multiply per byte */

void fastrand_seed(void const *s, uint8_t l);
uint16_t fastrand(void);


/* shift generator -- like fast, but using shifts instead of multipliers (so, not fast) */

void shiftrand_seed(void const *s, uint8_t l);
uint16_t shiftrand(void);


typedef struct
{
    void (*seed)(void const *seed, uint8_t length);
    uint16_t (*gen)(void);
    uint32_t a;
    uint8_t r;
    uint8_t *x;
    uint16_t *c;
} example_ptr_t;

extern example_ptr_t examples[6];

#endif
