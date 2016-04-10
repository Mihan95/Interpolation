#include <iostream>
#include <math.h>
#include <stdio.h>

#include "window.h"
#include "prots.h"

#define EPS 1.e-6

using namespace std;

double LinearInterpolate (double x1, double x2, double y1, double y2, double a)
{
    return (a - x1) * (y2 - y1) / (x2 - x1) + y1;
}

void CreateMatrix( double *a, double *x, int n )
{
    double *c = a + n;
    double *d = a + 2 * n;
    double *b = a + 3 * n;

    //create b
    b[0] = (a[0] * (x[2] - x[1]) * (2 * x[2] + x[1] - 3 * x[0]) + a[1] * (x[1] - x[0]) * (x[1] - x[0])) / (x[2] - x[0]);
    for( int i = 1; i < n - 1; i++ )
    {
        b[i] = 3 * a[i - 1] * (x[i + 1] - x[i]) + 3 * a[i] * (x[i] - x[i - 1]);
    }
    b[n - 1] = a[n - 3] * (x[n - 1] - x[n - 2]) * (x[n - 2] - x[n - 1]) - a[n - 2] * (x[n - 2] - x[n - 3]) * (3 * x[n - 1] - x[n - 2] - 2 * x[n - 3]);

    //create a
    a[0] = x[2] - x[1];
    for( int i = 1; i < n - 1; i++ )
    {
        a[i] = 2 * (x[i + 1] - x[i - 1]);
    }
    a[n - 1] = (x[n - 3] - x[n - 2]) * (x[n - 1] - x[n - 3]);

    //create c
    c[0] = x[2] - x[0];
    for( int i = 1; i < n - 1; i++ )
    {
        c[i] = x[i] - x[i - 1];
    }

    //create d
    for( int i = 0; i < n - 2; i++ )
    {
        d[i] = x[i + 2] - x[i + 1];
    }
    d[n - 2] = (x[n - 1] - x[n - 3]) * (x[n - 3] - x[n - 1]);
}

void ThreeDiagSolve( double *b, double *a, double *c, double *d, int n )
{
    c[0] = c[0] / a[0];
    for( int i = 1; i < n - 1; i++ )
    {
        a[i] = a[i] - d[i - 1] * c[i - 1];
        c[i] = c[i] / a[i];
    }
    a[n - 1] = a[n - 1] - d[n - 2] * c[n - 2];

    b[0] = b[0] / a[0];
    for( int i = 1; i < n; i++ )
        b[i] = (b[i] - d[i - 1] * b[i - 1]) / a[i];

    for( int i = n - 2; i > -1; i-- )
        b[i] = b[i] - c[i] * b[i + 1];
}
