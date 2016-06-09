//this library was constructed with functions taken from the PCG random numbers library which can be found
//at a link supplied in Appendix V of the final report, therefore I do not take credit for the code in this
//library.

typedef struct { long long state;  long long inc; } pcg32_random_t;

int pcg32_random_r(pcg32_random_t* rng)
{
    long long oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    int xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    int rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

int pcg32_boundedrand_r(pcg32_random_t* rng, int bound)
{
    // To avoid bias, we need to make the range of the RNG a multiple of
    // bound, which we do by dropping output less than a threshold.
    // A naive scheme to calculate the threshold would be to do
    //
    //     uint32_t threshold = 0x100000000ull % bound;
    //
    // but 64-bit div/mod is slower than 32-bit div/mod (especially on
    // 32-bit platforms).  In essence, we do
    //
    //     uint32_t threshold = (0x100000000ull-bound) % bound;
    //
    // because this version will calculate the same modulus, but the LHS
    // value is less than 2^32.

    int threshold = -bound % bound;

    // Uniformity guarantees that this loop will terminate.  In practice, it
    // should usually terminate quickly; on average (assuming all bounds are
    // equally likely), 82.25% of the time, we can expect it to require just
    // one iteration.  In the worst case, someone passes a bound of 2^31 + 1
    // (i.e., 2147483649), which invalidates almost 50% of the range.  In 
    // practice, bounds are typically small and only a tiny amount of the range
    // is eliminated.
    for (;;) {
        int r = pcg32_random_r(rng);
        if (r >= threshold)
            return r % bound;
    }
}

#define PCG32_INITIALIZER   { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

static pcg32_random_t pcg32_global = PCG32_INITIALIZER;

int pcg32_boundedrand(int bound)
{
    return pcg32_boundedrand_r(&pcg32_global, bound);
}

int pcg32_random()
{
    return pcg32_random_r(&pcg32_global);
}

void pcg32_srandom_r(pcg32_random_t* rng, long long initstate, long long initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg32_random_r(rng);
    rng->state += initstate;
    pcg32_random_r(rng);
}

void pcg32_srandom(long long seed, long long seq)
{
    pcg32_srandom_r(&pcg32_global, seed, seq);
}