#include <stdio.h>
#include <math.h>

typedef struct
{
    double fi, sum;
} Result;

Result recurse(double x, int i)
{
    printf("down   i=%-2d\n", i);

    Result r;
    if (i == 1)
    {
        r.fi = 4.0 / 3.0;
        r.sum = r.fi;
        printf("up     i=%-2d fi=%+.8f sum=%+.8f (base)\n", i, r.fi, r.sum);
        return r;
    }

    Result p = recurse(x, i - 1);
    r.fi = p.fi * (1.0 - 4.0 * x / 3.0);
    r.sum = p.sum + r.fi;

    printf("up     i=%-2d fi=%+.8f sum=%+.8f\n", i, r.fi, r.sum);
    return r;
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

    if (x <= 0.5 || x >= 1.0 || n < 1)
    {
        printf("invalid input\n");
        return 1;
    }

    printf("\nrecursion trace:\n");
    Result r = recurse(x, n);

    printf("\napprox = %.10f\nexact  = %.10f\nerror  = %.2e\n",
           r.sum, 1.0 / x, fabs(r.sum - 1.0 / x));

    return 0;
}