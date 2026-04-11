#include <stdio.h>
#include <math.h>

#define MAX_N 1000
static double F[MAX_N + 1];

double recurse(double x, int i, int n)
{
    F[i] = (i == 1) ? 4.0 / 3.0 : F[i - 1] * (1.0 - 4.0 * x / 3.0);

    printf("down   i=%-2d F[%d]=%+.8f\n", i, i, F[i]);

    if (i == n)
    {
        printf("up     i=%-2d return=%+.8f (base)\n", i, F[i]);
        return F[i];
    }

    double tail = recurse(x, i + 1, n);

    printf("up     i=%-2d F[%d] + tail = %+.8f + %+.8f = %+.8f\n",
           i, i, F[i], tail, F[i] + tail);

    return F[i] + tail;
}

int main(void)
{
    double x;
    int n;

    printf("F1 = 4/3\n");
    printf("Fi+1 = Fi * (1 - 4x/3)\n");
    printf("Sum(Fi) = 1/x\n\n");

    printf("enter x (0.5 < x < 1): ");
    scanf("%lf", &x);

    printf("enter n: ");
    scanf("%d", &n);

    if (x <= 0.5 || x >= 1.0 || n < 1 || n > MAX_N)
    {
        printf("invalid input\n");
        return 1;
    }

    printf("\nrecursion trace:\n");
    double approx = recurse(x, 1, n);

    printf("\napprox = %.10f\nexact  = %.10f\nerror  = %.2e\n",
           approx, 1.0 / x, fabs(approx - 1.0 / x));

    return 0;
}