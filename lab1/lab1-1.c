#include <stdio.h>
#include <math.h>

double recurse(double x, int i, int n, double fi, double sum)
{
    printf("down   i=%-2d fi=%+.8f sum=%+.8f\n", i, fi, sum);

    if (i == n)
    {
        printf("up     i=%-2d res=%+.8f (base)\n", i, sum + fi);
        return sum + fi;
    }

    double q = 1.0 - 4.0 * x / 3.0;   
    double res = recurse(x, i + 1, n, fi * q, sum + fi);

    printf("up     i=%-2d res=%+.8f\n", i, res);
    return res;
}

int main(void)
{
    double x;
    int n;

    
    printf("F1 = 4/3\n");
    printf("Fi+1 = Fi * (1 - 4x/3)\n");
    printf("Sum(Fi) = 1/x,  0.5 < x < 1\n\n");

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
    double approx = recurse(x, 1, n, 4.0 / 3.0, 0.0);

    printf("\napprox = %.10f\n", approx);
    printf("exact  = %.10f\n", 1.0 / x);
    printf("error  = %.2e\n", fabs(approx - 1.0 / x));

    return 0;
}
