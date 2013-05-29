#if !defined(EIGHTBITRAND_H_INCLUDED)
#define EIGHTBITRAND_H_INCLUDED

#include <stdint.h>

extern void rand8_init(uint16_t a, uint8_t r, uint16_t c, uint8_t const *seed);
extern uint8_t rand8_8bit(void);
extern uint16_t rand16_8bit(void);
extern uint32_t rand32_8bit(void);
extern uint8_t rand8_9bit(void);
extern uint16_t rand16_9bit(void);
extern uint32_t rand32_9bit(void);
extern uint8_t rand8_16bit(void);
extern uint16_t rand16_16bit(void);
extern uint32_t rand32_16bit(void);

#endif
