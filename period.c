#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <strings.h>

#include "tables.h"

#define QUICK_PERIOD 0x10000000

static uint64_t p32(uint16_t a, uint16_t c, uint32_t x, uint64_t b, int s)
{
    uint32_t m = ~(0xfffffffe << (s - 1));
    uint16_t cc = c;
    uint64_t xx;
    uint64_t p = 0;

    xx = x &= m;

    do
    {
        uint64_t t = (uint64_t)x * a + c;
        x = t & m;
        c = t >> s;
        p++;

        if (p >= b)
            return b;
        if (c < cc || (c <= cc && x < xx))
            return 0;
    } while (x != xx || c != cc);

    return p;
}

static uint64_t p56(uint16_t a, uint16_t c, uint64_t x, uint64_t b, int s)
{
    uint64_t m = ~(0xfffffffffffffffeull << (s - 1));
    uint16_t cc = c;
    uint64_t xx;
    uint64_t p = 0;

    xx = x &= m;

    do
    {
        uint64_t t = x * a + c;
        x = t & m;
        c = t >> s;
        p++;

        if (p >= b)
            return b;
        if (c < cc || (c <= cc && x < xx))
            return 0;
    } while (x != xx || c != cc);

    return p;
}

static uint64_t p64(uint16_t a, uint16_t c, uint64_t x, uint64_t b, int s)
{
    uint32_t m = ~(0xfffffffe << ((s - 1) & 31));
    uint32_t x0 = x & 0xffffffff, x1 = (x >> 32) & m;
    uint16_t cc = c;
    uint64_t xx0 = x0;
    uint64_t xx1 = x1;
    uint64_t p = 0;

    s = ((s - 1) & 31) + 1;

    do
    {
        uint64_t t = (uint64_t)x0 * a + c;
        x0 = (uint32_t)t;
        c = t >> 32;
        t = (uint64_t)x1 * a + c;
        x1 = t & m;
        c = t >> s;
        p++;

        if (p >= b)
            return b;
        if (c < cc || (c <= cc && (x1 < xx1 || (x1 <= xx1 && x0 < xx0))))
            return 0;
    } while (x0 != xx0 || x1 != xx1 || c != cc);

    return p;
}

static uint64_t p96(uint16_t a, uint16_t c, uint64_t x, uint64_t b, int s)
{
    uint32_t m = ~(0xfffffffe << ((s - 1) & 31));
    uint32_t x0 = x & 0xffffffff, x1 = x >> 32, x2 = 0;
    uint16_t cc = c;
    uint64_t xx0 = x0;
    uint64_t xx1 = x1;
    uint64_t xx2 = x2;
    uint64_t p = 0;

    s = ((s - 1) & 31) + 1;

    do
    {
        uint64_t t = (uint64_t)x0 * a + c;
        x0 = (uint32_t)t;
        c = t >> 32;
        t = (uint64_t)x1 * a + c;
        x1 = (uint32_t)t;
        c = t >> 32;
        t = (uint64_t)x2 * a + c;
        x2 = t & m;
        c = t >> s;
        p++;

        if (p >= b)
            return b;
        if (c < cc || (c <= cc && (x2 < xx2 || (x2 <= xx2 && (x1 < xx1 || (x1 <= xx1 && x0 < xx0))))))
            return 0;
    } while (x0 != xx0 || x1 != xx1 || x2 != xx2 || c != cc);

    return p;
}

static uint64_t p128(uint16_t a, uint16_t c, uint64_t x, uint64_t b, int s)
{
    uint32_t m = ~(0xfffffffe << ((s - 1) & 31));
    uint32_t x0 = x & 0xffffffff, x1 = x >> 32, x2 = 0, x3 = 0;
    uint16_t cc = c;
    uint64_t xx0 = x0;
    uint64_t xx1 = x1;
    uint64_t xx2 = x2;
    uint64_t xx3 = x3;
    uint64_t p = 0;

    s = ((s - 1) & 31) + 1;

    do
    {
        uint64_t t = (uint64_t)x0 * a + c;
        x0 = (uint32_t)t;
        c = t >> 32;
        t = (uint64_t)x1 * a + c;
        x1 = (uint32_t)t;
        c = t >> 32;
        t = (uint64_t)x2 * a + c;
        x2 = (uint32_t)t;
        c = t >> 32;
        t = (uint64_t)x3 * a + c;
        x3 = t & m;
        c = t >> s;
        p++;

        if (p >= b)
            return b;
        if (c < cc || (c <= cc && (x3 < xx3 || (x3 <= xx3 && (x2 < xx2 || (x2 <= xx2 && (x1 < xx1 || (x1 <= xx1 && x0 < xx0))))))))
            return 0;
    } while (x0 != xx0 || x1 != xx1 || x2 != xx2 || x3 != xx3 || c != cc);

    return p;
}

static uint64_t period(uint16_t a, uint16_t c, uint64_t x, uint8_t r, uint64_t b)
{
    int s = r * 8;
    switch (r)
    {
    case 1: case 2: case 3: case 4:
        return p32(a, c, x, b, s);
    case 5: case 6:
        return p56(a, c, x, b, s);
    case 7: case 8:
        return p64(a, c, x, b, s);
    case 9: case 10: case 11: case 12:
        return p96(a, c, x, b, s);
    case 13: case 14: case 15: case 16:
        return p128(a, c, x, b, s);
    }
    fprintf(stderr, "unsupported r=%d.\n", r);
    exit(-1);
    return 0;
}

static void test_loop(uint16_t a, uint8_t r, int quick)
{
    int s = 8 * (r <= 6 ? r : 6);
    uint64_t b = (uint64_t)1 << s;
    uint64_t goal = ((uint64_t)a << s);
    uint64_t bail = goal + 1;
    uint64_t crappy = b;
    uint64_t ptot = 0;
    uint64_t pmin = 0xffffffffffffffff;
    uint64_t pmax = 0;
    uint64_t npmin = 0;
    uint64_t npmax = 0;
    uint64_t dups = 0;
    uint64_t np1 = 0;
    uint64_t loops = 0;
    uint64_t p;
    uint64_t x = 0;
    int stop = 0, overrun = 0;
    uint16_t c = 0;

    if (b <= QUICK_PERIOD)
        quick = 0;
    if (quick)
    {
        b = QUICK_PERIOD;
        goal = a * b;
        bail = b + 1;
        crappy = b;
    }
    if (period(a, a - 1, -1LL, r, 2) == 1)
        np1++, ptot++;

    for (c = 0; c < a; c++)
    {
        putchar('.');
        fflush(stdout);
        for (x = 0; x < b; x++)
        {
            p = period(a, c, x, r, bail);
            switch (p)
            {
            case 0:
                dups++;
                if (loops == 0)
                    stop = 1;
                break;
            case 1:
                np1++;
                if (np1 > 2)
                    stop = 1;
                break;
            default:
                if (p == bail && !quick)
                {
                    overrun = 1;
                    p = 0;
                    stop = 1;
                }
                else
                {
                    printf("x=0x%llx, c=0x%x, p%s%llu (0x%llx)\n", x, c, p == bail ? ">=" : "=", p, p);
                    loops++;
                    if (pmin > p) pmin = p, npmin = 0;
                    if (pmax < p) pmax = p, npmax = 0;
                    if (pmin == p) npmin++;
                    if (pmax == p) npmax++;
                    if (p <= crappy)
                        stop = 1;
                }
            }
            ptot += p;
            if (ptot >= goal)
                stop = 1;
            if (stop)
                break;
        }
        if (stop)
            break;
    }

    printf("\nStopped at x=0x%llx, c=0x%x\n", x, c);

    if (ptot >= goal)
        printf("Met coverage requirement (0x%llx >= 0x%llx)\n", ptot, goal);

    printf("a=0x%x, r=%d, %llux p=1, "
            "%llux pmin=%llu (0x%llx), "
            "%llux pmax=%llu (0x%llx) "
            "goal %s\n",
            a, r, np1,
            npmin, pmin, pmin,
            npmax, pmax, pmax,
            ptot >= goal ? "met" : "missed");
    printf("loops=%llu, dups=%llu\n", loops, dups);

    if (overrun > 0)
        printf("!!! Bailed out because test overran\n");
    if (dups != 0 && loops == 0)
        printf("!!! Bailed out for collapsing to zero\n");
    if (np1 > 2)
        printf("!!! Bailed out with too many p=1\n");
    if (pmin <= crappy)
        printf("!!! Bailed out because pmin=0x%llx is crappy\n", pmin);
}

static void usage(void)
{
    printf("8bitrand -w <i> -r <r>  wide multiply\n"
           "8bitrand -n <i> -r <r>  narrow multiply\n"
           "8bitrand -u <i> -r <r>  narrow multiply (unsafe primes)\n"
           "8bitrand -a <a> -r <r>  user-specified multiply\n"
           "\t-{w|n|u} <i> index of known-good multiplier to use\n"
           "\t-a <a> multiplier to use\n"
           "\t-r <r> generator lag\n");
}


int main(int argc, char *argv[])
{
    /* generator config */
    uint16_t a = 0;
    uint8_t r = 0;
    int quick = 0;

    tabptr_t const *table = NULL;
    long tab_index = 0;
    uint8_t a_lo;
    int optc;

    while ((optc = getopt(argc, argv, "w:n:u:a:r:qh")) != -1)
        switch (optc)
        {
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

        case 'q': quick = 1;
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
    printf("b=256 a=0x%04x r=%d\n", a, r);

    if (255 * a_lo + a - 1 > 0xffff)
    {
        fprintf(stderr, "This multiplier could overflow internal arithmetic.\n");
        exit(EXIT_FAILURE);
    }

    argc -= optind;
    argv += optind;

    test_loop(a, r, quick);

    return 0;
}
