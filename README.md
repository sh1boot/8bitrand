8bitrand
========
A [multiply-with-carry][mwc] pseudo-random number generator optimised for 8-bit
processors with 8-bit hardware multipliers.  Inspired by a [question][] on
StackOverflow.

Four parameterised implementations -- eight bit, nine bit, sixteen bit,
and a sparse 16-bit -- are provided.  The eight and nine bit versions
use only a single 8-bit multiply operation per eight random bits
produced (the the 9-bit version needs an extra add, but avoids needing
an extra shift).  The sixteen bit versions uses two multiplies per eight
bits of output, but are still slightly constrained in the multipliers
they're able to use (one risks internal overflow with certain values,
the other needs a zero-byte between two non-zero bytes).

This is in contrast with a typical [linear congruential generator][lcg] which
would typically use seven to ten 8-bit multiplies (and more additions) to
produce about 16 bits of usable output, and maybe 16 more bits of garbage
appended to that.

For the nine-bit implementation, parameter sets with around 12 bytes of state
can almost scrape through all of [dieharder][] randomness tests (contrast with
[Mersenne Twister][mt]'s 2500 bytes).  The
less-computationally-efficient sixteen-bit implementation can survive
all [dieharder][]'s default tests with as few as seven bytes of state.

Provided the MCU has an 8-bit hardware multiplier I expect this code to
outperform most shift-and-add approaches to random number generation (including
MT), as most of these seem to rely on 32-bit words and shifters which can shift
further than a single bit in a cycle -- both of which are often unavailable in
the 8-bit world.

Real-world tuning
-----------------
Any real-world use of this code should hard-code the multiplier and
length and pick the appropriate implementation.  The seed function must
never allow both carry and state to be all zeroes.  It's probably for
the best to initialise carry to some large random constant and to fill
the state bytes from the user-provided seed.

[examples.c][] shows some implementations where the state index and
carry register are overlapped to save memory, and one implementation
([tinyrand][]()) where the state is so short that the index is omitted
altogether.

[question]: http://stackoverflow.com/q/16746971/2417578
[mwc]: http://en.wikipedia.org/wiki/Multiply_with_Carry
[lcg]: http://en.wikipedia.org/wiki/Linear_congruential_generator
[dieharder]: http://www.phy.duke.edu/~rgb/General/dieharder.php
[mt]: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html

[examples.c]: examples.c
[tinyrand]: examples.c#tinyrand
