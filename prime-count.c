//
// Evaluate the number of primes with a given number of bits.
//
// Thierry Lelegard, 2015-01-28
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//
// Cygwin does not have math library support for long double
//
#if defined(__CYGWIN__)
    typedef double FLOAT;
    #define POW pow
    #define LOG log
    #define FLOAT_FORMAT "g"
#else
    typedef long double FLOAT;
    #define POW powl
    #define LOG logl
    #define FLOAT_FORMAT "Lg"
#endif

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s bit-count\n", argv[0]);
        return EXIT_FAILURE;
    }
    else {
        const int   bits    = atoi(argv[1]);
        const FLOAT values  = POW((FLOAT)2, (FLOAT)bits);
        const FLOAT values1 = POW((FLOAT)2, (FLOAT)(bits - 1));
        const FLOAT primes  = values / LOG(values - 1);
        const FLOAT primes1 = values1 / LOG(values1 - 1);

        printf("Number of primes < 2^%d: %.2" FLOAT_FORMAT "\n", bits - 1, primes1);
        printf("Number of primes < 2^%d: %.2" FLOAT_FORMAT "\n", bits, primes);
        printf("Number of values < 2^%d: %.2" FLOAT_FORMAT "\n", bits, values);
        printf("Number of %d-bit primes: %.2" FLOAT_FORMAT "\n", bits, primes - primes1);
        printf("Number of %d-bit values: %.2" FLOAT_FORMAT "\n", bits, values - values1);

        return EXIT_SUCCESS;
    }
}
