#include <stdio.h>
#include <math.h>

int main(void)
{
    double x;
    int n;

    printf("iterative: Variant 27\n");
    printf("F1 = 4/3\n");
    printf("Fi+1 = Fi * (1 - 4x/3)\n");
    printf("Sum(Fi) = 1/x\n\n");

    printf("enter x (0.5 < x < 1): ");
    scanf("%lf", &x);

    printf("enter n: ");
    scanf("%d", &n);

    if (x <= 0.5 || x >= 1.0 || n < 1)
    {
        printf("invalid input\n");
        return 1;
    }

    printf("\nstep-by-step:\n");

    double fi = 4.0 / 3.0, sum = 0.0;
    for (int i = 1; i <= n; i++)
    {
        if (i > 1)
            fi = fi * (1.0 - 4.0 * x / 3.0);

        sum += fi;
        printf("  i=%-2d fi=%+.8f sum=%+.8f\n", i, fi, sum);
    }

    printf("\napprox = %.10f\nexact  = %.10f\nerror  = %.2e\n",
           sum, 1.0 / x, fabs(sum - 1.0 / x));

    return 0;
}