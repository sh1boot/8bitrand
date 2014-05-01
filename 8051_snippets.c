static unsigned char rnd_x[8], rnd_c0, rnd_c1, rnd_c2, rnd_c3;

/* more-or-less perfect results in dieharder,
 * uses x[0]-x[2], c0-c2 */

unsigned char asmrand(void)
{
    _asm;
    mov R0,(_rnd_x + 2)
    mov A, (_rnd_x + 0)
    mov B, #0xc8
    mul AB
    add A, (_rnd_c0)
    mov (_rnd_x + 2), A
    mov A, B
    addc A, (_rnd_c1)
    mov (_rnd_c0), A
    mov AC, C
    mov A, (_rnd_x + 0)
    mov B, #0xfd
    mul AB
    mov C, AC
    addc A, (_rnd_c2)
    mov (_rnd_c1), A
    mov A, B
    addc A, #0
    mov (_rnd_c2), A

    mov A, (_rnd_x + 1)
    mov (_rnd_x + 0), A
    mov (_rnd_x + 1), R0
    _endasm;
    return rnd_x[2];
}


/* no failures, 5 weak cases in dieharder default configuration,
 * uses x[0], c0-c2 */

unsigned char asmrand2(void)
{
    _asm;
    mov A, _rnd_x
    mov B, #0xab
    mul AB
    add A, _rnd_c0
    mov R0, A
    mov A, B
    addc A, _rnd_c1
    mov _rnd_c0, A
    mov AC, C
    mov A, _rnd_x
    mov B, #0xed
    mul AB
    mov C, AC
    addc A, _rnd_c2
    mov _rnd_c1, A
    mov A, B
    addc A, #0
    mov _rnd_c2, A
    mov _rnd_x, R0
    _endasm;
    return rnd_x;
}

/* long period, most state could be xdata, good dieharder results, only
 * one multiply but the buffer addressing eats up more than enough
 * compute time to offset that.
 * uses x[0]-x[7], c0-c3 */

unsigned char asmrand3(void)
{
    _asm;
    mov A, _rnd_c3
    rr  A
    anl A, #7
    add A, #_rnd_x
    mov R0, A
    mov A, @R0
    mov R1, A
    add A, _rnd_c0
    mov @R0, A
    mov R0, A  /* result */
    mov A, #0
    addc A, _rnd_c1
    mov _rnd_c0, A
    mov AC, C
    mov A, R1
    mov B, #0x0d
    mul AB
    mov C, AC
    addc A, _rnd_c2
    mov _rnd_c1, A
    mov A, _rnd_c3
    anl A, #1
    addc A, B
    mov F0, C
    add A, R1
    mov _rnd_c2, A
    orl C, F0
    mov A, _rnd_c3
    mov ACC.0, C
    add A, #2
    mov _rnd_c3, A
    mov A, R0
    _endasm;

    return ACC;
}
