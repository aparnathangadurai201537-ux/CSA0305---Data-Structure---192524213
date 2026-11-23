#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100000
#define BAD_M 1000
#define GOOD_M 100003

unsigned bad_hash(const char *s)
{
    return ((unsigned char)s[0] +
            (unsigned char)s[1]) % BAD_M;
}

unsigned poly_hash(const char *s)
{
    unsigned long long h = 0;

    while (*s)
        h = h * 31u + (unsigned char)*s++;

    return (unsigned)(h % GOOD_M);
}

void make_reg(char *s, int i)
{
    snprintf(s, 16, "TN%02dAB%04d",
             (i % 90) + 1, i % 10000);
}

int main()
{
    unsigned *bad = calloc(BAD_M, sizeof(*bad));
    unsigned *good = calloc(GOOD_M, sizeof(*good));

    if (!bad || !good)
        return 1;

    for (int i = 0; i < N; i++)
    {
        char reg[16];

        make_reg(reg, i);

        bad[bad_hash(reg)]++;
        good[poly_hash(reg)]++;
    }

    unsigned bad_max = 0;
    unsigned good_max = 0;

    double bad_avg = 0.0;
    double good_avg = 0.0;

    for (int i = 0; i < BAD_M; i++)
    {
        if (bad[i] > bad_max)
            bad_max = bad[i];

        if (bad[i])
            bad_avg += bad[i];
    }

    for (int i = 0; i < GOOD_M; i++)
    {
        if (good[i] > good_max)
            good_max = good[i];

        if (good[i])
            good_avg += good[i];
    }

    printf("Bad hash: longest chain = %u, average non-empty chain = %.2f\n",
           bad_max, bad_avg);

    printf("Polynomial: longest chain = %u, average non-empty chain = %.2f\n",
           good_max, good_avg);

    free(bad);
    free(good);

    return 0;
}