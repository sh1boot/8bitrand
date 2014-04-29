#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <strings.h>

#include "8bitrand.h"
#include "tables.h"
#include "examples.h"

static uint32_t ref_state[8];
static uint32_t ref_a;
static uint32_t ref_c;
static uint8_t ref_r;

static void ref_init(uint32_t a, uint8_t r, uint32_t c, uint8_t const *seed)
{
    int i;

    ref_a = a;
    ref_r = r;
    ref_c = c;

    for (i = 0; i + 3 < r; i += 4)
        ref_state[i >> 2] = seed[i] + ((uint32_t)seed[i + 1] << 8) + ((uint32_t)seed[i + 2] << 16) + ((uint32_t)seed[i + 3] << 24);
    if (i < r) ref_state[i >> 2] = seed[i];
    if (i + 1 < r) ref_state[i >> 2] += (uint32_t)seed[i + 1] << 8;
    if (i + 2 < r) ref_state[i >> 2] += (uint32_t)seed[i + 2] << 16;
}

static void ref_memtest(uint8_t const *src, int length, uint64_t count)
{
    unsigned int a = ref_a;
    unsigned int c = ref_c;
    int r = ref_r;
    int i, j;

    for (i = 0; i < length; i += r)
    {
        for (j = 0; j < r; j += 4)
        {
            uint32_t dut;
            uint32_t x = ref_state[j >> 2];
            uint64_t t;
            int bits;

            t = (uint64_t)x * a + c;

            bits = (r - j) * 8;
            if (bits >= 32)
            {
                bits = 32;
                x = (uint32_t)t;

                dut = src[0] + ((uint32_t)src[1] << 8) + ((uint32_t)src[2] << 16) + ((uint32_t)src[3] << 24);
                src += 4;
            }
            else
            {
                x = (uint32_t)t & ~(0xffffffff << bits);

                dut = *src++;
                if (bits > 8) dut += (uint32_t)*src++ << 8;
                if (bits > 16) dut += (uint32_t)*src++ << 16;
            }

            if (dut != x)
            {
                fprintf(stderr, "0x%llx: 0x%08x != 0x%08x\n", count + i+j, dut, x);
                fprintf(stderr, "t=0x%010llx=(a=0x%04x * s[%d]=0x%08x + c=0x%04x)\n", t, a, j, ref_state[j >> 2], c);
                exit(EXIT_FAILURE);
            }

            c = (uint32_t)(t >> bits);
            ref_state[j >> 2] = x;
        }
    }
    ref_c = c;
}

static void dut_memrand(uint8_t *dst, int length, uint16_t (*genfn)(void))
{
    int i;
    uint8_t *p = dst;

    for (i = 0; i < length; i += 2)
    {
        uint16_t x = genfn();
        *p++ = x & 0xff;
        *p++ = x >> 8;
    }
}

static void generate_loop(unsigned long long size, uint16_t (*genfn)(void))
{
    while (size > 0)
    {
        uint8_t buf[1 << 20];
        int out;

        out = sizeof(buf);
        if (out > size)
            out = size;

        out = out & ~1;
        if (out == 0)
            break;

        dut_memrand(buf, out, genfn);

        out = fwrite(buf, 1, out, stdout);
        if (out < 0)
            break;
        if (size != ~0ULL) size -= out;
    }
}

static void test_loop(unsigned long long size, uint16_t (*genfn)(void), uint32_t a, uint8_t r, uint32_t c, uint8_t const *seed)
{
    unsigned long long count = 0;

    ref_init(a, r, c, seed);

    while (count < size)
    {
        uint8_t buf[1 << 20];
        int out;

        out = sizeof(buf);
        if (out > count - size)
            out = count - size;
        out -= out % (r * 2);
        if (out == 0)
            break;

        dut_memrand(buf, out, genfn);
        ref_memtest(buf, out, count);

        count += out;
    }
}

static void usage(void)
{
    printf("8bitrand -W <i> [-r <r>] [-s <xxx>] [-c <c>]  really-wide multiply\n"
           "8bitrand -w <i> [-r <r>] [-s <xxx>] [-c <c>]  wide multiply\n"
           "8bitrand -n <i> [-r <r>] [-s <xxx>] [-c <c>]  narrow multiply\n"
           "8bitrand -u <i> [-r <r>] [-s <xxx>] [-c <c>]  narrow multiply (unsafe primes)\n"
           "8bitrand -a <a> [-r <r>] [-s <xxx>] [-c <c>]  user-specified multiply\n"
           "8bitrand -e <i>          [-s <xxx>] [-c <c>]  example implementation\n"
           "\t-{w|n|u} <i> index of known-good multiplier to use\n"
           "\t-a <a> multiplier to use\n"
           "\t-e <i> index of example implementation to use\n"
           "\t-r <r> generator lag\n"
           "\t-s <xxx> hexadecimal starting seed in hexadecimal\n"
           "\t-c <c> hexadecimal starting carry\n");
}

int main(int argc, char *argv[])
{
    /* generator config */
    uint32_t a = 0;
    uint16_t c = 1;
    uint8_t r = 0;
    uint8_t seed[SEED_MAX] = { 0 };
    uint8_t seedlen = 0;
    example_ptr_t *impl = NULL;
    uint16_t (*genfn)(void);

    enum { MODE_GENERATE, MODE_TEST } mode = MODE_GENERATE;
    tabptr_t const *table = NULL;
    long tab_index = 0;
    uint8_t a_lo;
    uint16_t a_hi;
    int optc;
    int i;

    while ((optc = getopt(argc, argv, "e:W:w:n:u:a:r:s:c:gth")) != -1)
        switch (optc)
        {
        case 'e':
            {
                long i = strtol(optarg, NULL, 0);
                if (i >= sizeof(examples) / sizeof(*examples))
                {
                    fprintf(stderr, "Only %d known example functions.\n", sizeof(examples) / sizeof(*examples));
                    exit(EXIT_FAILURE);
                }
                impl = &examples[i];
                a = impl->a;
                r = impl->r;
            }
            break;
        case 'W':
            table = multiplier_table[SPARSE_TABLE];
            tab_index = strtol(optarg, NULL, 0);
            break;

        case 'w':
            table = multiplier_table[WIDE_TABLE];
            tab_index = strtol(optarg, NULL, 0);
            break;

        case 'n':
            table = multiplier_table[NARROW_TABLE];
            tab_index = strtol(optarg, NULL, 0);
            break;

        case 'u':
            table = multiplier_table[UNSAFE_TABLE];
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
                seedlen = i;
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

    if (r < 1 || r > SEED_MAX)
    {
        fprintf(stderr, "Out-of-range lag value.\n");
        exit(EXIT_FAILURE);
    }

    if (a == 0)
    {
        if (table == NULL)
        {
            fprintf(stderr, "Must choose multiplier.\n");
            exit(EXIT_FAILURE);
        }

        if (tab_index < 0)
            tab_index += table[r].len;
        if ((unsigned)tab_index >= table[r].len)
        {
            if (table[r].len > 0)
                fprintf(stderr, "Only %d known lag-%d multipliers.\n", table[r].len, r);
            else
                fprintf(stderr, "No known lag-%d multipliers.\n", r);
            exit(EXIT_FAILURE);
        }
        a = table[r].tab[tab_index];
    }
    a_lo = a & 0xff;
    a_hi = a >> 8;

    if (impl)
    {
        impl->seed(seed, seedlen);
        for (i = 0; i < r; i++)
            seed[i] = impl->x[i];
        c = *impl->c;
        genfn = impl->gen;
    }
    else
    {
        rand8_init(a, r, c, seed);
        switch (a_hi)
        {
        case 0:
            genfn = &rand16_8bit;
            break;
        case 1:
            genfn = &rand16_9bit;
            break;
        default:
            if ((a_hi & 0xff) == 0)
            {
                rand8_init(a_lo | a_hi, r, c, seed);
                genfn = &rand16_8z8bit;
            }
            else
                genfn = &rand16_16bit;
            break;
        }
    }

    fprintf(stderr, "b=256 a=0x%04x r=%d c=0x%04x s=0x", a, r, c);
    for (i = 0; i < r; i++)
        fprintf(stderr, "%02x", seed[i]);
    fprintf(stderr, "\n");

    if (255 * a_lo + (a & 0xffff) - 1 > 0xffff)
    {
        fprintf(stderr, "This multiplier could overflow internal arithmetic.\n");
        exit(EXIT_FAILURE);
    }

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

            generate_loop(size, genfn);
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

            test_loop(size, genfn, a, r, c, seed);
        }
        break;
    }

    return 0;
}
