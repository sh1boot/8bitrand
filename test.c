#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <strings.h>

#include "8bitrand.h"
#include "tables.h"

static void generate_loop(unsigned long long size, uint8_t a_hi)
{
    while (size > 0)
    {
        uint8_t buf[1 << 20];
        int out;
        int i;

        out = sizeof(buf);
        if (out > size)
            out = size;

        switch (a_hi)
        {
        case 0:
            for (i = 0; i < out; i++)
                buf[i] = rand8_8bit();
            break;
        case 1:
            for (i = 0; i < out; i++)
                buf[i] = rand8_9bit();
            break;
        default:
            for (i = 0; i < out; i++)
                buf[i] = rand8_16bit();
            break;
        }
        out = fwrite(buf, 1, out, stdout);
        if (out < 0)
            break;
        if (size != ~0ULL) size -= out;
    }
}

static void test_loop(unsigned long long size, uint16_t a, uint8_t r, uint16_t c, uint8_t const *seed)
{
    int a_hi = a >> 8;
    if (a_hi > 2)
        a_hi = 2;

    for ( ; a_hi < 4; a_hi += 2)
    {
        unsigned long long count = 0;
        uint32_t state[8];
        uint32_t carry = c;
        int i, j;

        rand8_init(a, r, c, seed);

        for (i = 0; i + 3 < r; i += 4)
            state[i >> 2] = seed[i] + ((uint32_t)seed[i + 1] << 8) + ((uint32_t)seed[i + 2] << 16) + ((uint32_t)seed[i + 3] << 24);
        if (i < r) state[i >> 2] = seed[i];
        if (i + 1 < r) state[i >> 2] += (uint32_t)seed[i + 1] << 8;
        if (i + 2 < r) state[i >> 2] += (uint32_t)seed[i + 2] << 16;

        while (count < size)
        {
            uint8_t buf[1 << 20];
            int out;

            out = sizeof(buf);
            if (out > count - size)
                out = count - size;
            out -= out % r;
            if (out == 0)
                break;

            switch (a_hi)
            {
            case 0:
                for (i = 0; i < out; i++)
                    buf[i] = rand8_8bit();
                break;
            case 1:
                for (i = 0; i < out; i++)
                    buf[i] = rand8_9bit();
                break;
            default:
                for (i = 0; i < out; i++)
                    buf[i] = rand8_16bit();
                break;
            }

            for (i = 0; i < out; i += r)
            {
                for (j = 0; j < r; j += 4) 
                {
                    uint32_t mem;
                    uint32_t x = state[j >> 2];
                    uint64_t t;
                    int bits;

                    t = (uint64_t)x * a + carry;

                    bits = (r - j) * 8;
                    if (bits >= 32)
                    {
                        bits = 32;
                        x = (uint32_t)t;

                        mem = buf[i+j] + ((uint32_t)buf[i+j + 1] << 8) + ((uint32_t)buf[i+j + 2] << 16) + ((uint32_t)buf[i+j + 3] << 24);
                    }
                    else
                    {
                        x = (uint32_t)t & ~(0xffffffff << bits);

                        mem = buf[i+j];
                        if (bits > 8) mem += (uint32_t)buf[i+j + 1] << 8;
                        if (bits > 16) mem += (uint32_t)buf[i+j + 2] << 16;
                    }

                    if (mem != x)
                    {
                        fprintf(stderr, "0x%llx: 0x%08x != 0x%08x\n", count + i+j, mem, x);
                        fprintf(stderr, "t=0x%010llx=(a=0x%04x * s[%d]=0x%08x + c=0x%04x)\n", t, a, j, state[j >> 2], carry);
                        exit(EXIT_FAILURE);
                    }

                    carry = (uint32_t)(t >> bits);
                    state[j >> 2] = x;
                }
            }
            count += out;
        }
    }
}

static void usage(void)
{
    printf("8bitrand -w <i> [-r <r>] [-s <xxx>] [-c <c>]  wide multiply\n"
           "8bitrand -n <i> [-r <r>] [-s <xxx>] [-c <c>]  narrow multiply\n"
           "8bitrand -u <i> [-r <r>] [-s <xxx>] [-c <c>]  narrow multiply (unsafe primes)\n"
           "8bitrand -a <a> [-r <r>] [-s <xxx>] [-c <c>]  user-specified multiply\n"
           "\t-{w|n|u} <i> index of known-good multiplier to use\n"
           "\t-a <a> multiplier to use\n"
           "\t-r <r> generator lag\n"
           "\t-s <xxx> hexadecimal starting seed in hexadecimal\n"
           "\t-c <c> hexadecimal starting carry\n");
}


int main(int argc, char *argv[])
{
    /* generator config */
    uint16_t a = 0;
    uint16_t c = 1;
    uint8_t r = 0;
    uint8_t seed[30] = { 0 };

    enum { MODE_GENERATE, MODE_TEST } mode = MODE_GENERATE;
    tabptr_t const *table = NULL;
    size_t max_r = 0;
    long tab_index = 0;
    uint8_t a_lo, a_hi;
    int optc;
    int i;

    while ((optc = getopt(argc, argv, "w:n:u:a:r:s:c:gth")) != -1)
        switch (optc)
        {
        case 'w':
            table = wide_table;
            max_r = sizeof(wide_table) / sizeof(*wide_table);
            tab_index = strtol(optarg, NULL, 0);
            break;

        case 'n':
            table = narrow_table;
            max_r = sizeof(narrow_table) / sizeof(*narrow_table);
            tab_index = strtol(optarg, NULL, 0);
            break;

        case 'u':
            table = unsafe_table;
            max_r = sizeof(unsafe_table) / sizeof(*unsafe_table);
            tab_index = strtol(optarg, NULL, 0);
            break;

        case 'a':
            a = strtoul(optarg, NULL, 0);
            break;

        case 'r':
            r = strtoul(optarg, NULL, 0);
            break;

        case 's':
            {
                char const *ptr = optarg;
                if (strncasecmp(ptr, "0x", 2) == 0)
                    ptr += 2;
                for (i = 0; i < sizeof(seed); i++)
                {
                    unsigned int byte;
                    if (sscanf(ptr, "%02x", &byte) != 1)
                        break;
                    seed[i] = byte;
                    ptr += 2;
                }
                for ( ; i < sizeof(seed); i++)
                    seed[i] = 0;
            }
            break;

        case 'c':
            c = strtoul(optarg, NULL, 0);
            break;

        case 'g':
            mode = MODE_GENERATE;
            break;

        case 't':
            mode = MODE_TEST;
            break;

        case 'h':
            usage();
            return 0;

        default:
            fprintf(stderr, "Invalid argument.\n");
            usage();
            exit(EXIT_FAILURE);
        }

    if (a == 0)
    {
        int choices;

        if (table == NULL)
        {
            fprintf(stderr, "Must choose multiplier.\n");
            exit(EXIT_FAILURE);
        }

        choices = r < max_r ? table[r].len : 0;
        if (tab_index < 0)
            tab_index += choices;
        if ((unsigned)tab_index >= choices)
        {
            if (choices)
                fprintf(stderr, "Only %d known lag-%d multipliers.\n", choices, r);
            else
                fprintf(stderr, "No known lag-%d multipliers.\n", r);
            exit(EXIT_FAILURE);
        }
        a = table[r].tab[tab_index];
    }

    if (r < 1 || r > sizeof(seed))
    {
        fprintf(stderr, "Out-of-range lag value.\n");
        exit(EXIT_FAILURE);
    }

    a_lo = a & 0xff;
    a_hi = a >> 8;
    fprintf(stderr, "b=256 a=0x%04x r=%d c=0x%04x s=0x", a, r, c);
    for (i = 0; i < r; i++)
        fprintf(stderr, "%02x", seed[i]);
    fprintf(stderr, "\n");

    if (255 * a_lo + a - 1 > 0xffff)
    {
        fprintf(stderr, "This multiplier could overflow internal arithmetic.\n");
        exit(EXIT_FAILURE);
    }

    rand8_init(a, r, c, seed);

    argc -= optind;
    argv += optind;

    switch (mode)
    {
    case MODE_GENERATE:
        {
            unsigned long long size;

            if (*argv != NULL)
                size = strtoull(*argv, NULL, 0);
            else
                size = ~0ULL;

            generate_loop(size, a_hi);
        }
        break;

    case MODE_TEST:
        {
            unsigned long long size;

            if (*argv != NULL)
                size = strtoull(*argv, NULL, 0);
            else
                size = 1 << 30;

            /* TODO: should probably have seed step tests and stuff like that. */

            test_loop(size, a, r, c, seed);
        }
        break;
    }

    return 0;
}
