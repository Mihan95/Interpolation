#include <math.h>
#include <string.h>
#include <stdio.h>
#include "prots.h"
#include <iostream>

#define pi 3.1415926535897932

using namespace std;

int ChebCoeff (int n, double *f_x, double *a)
{
    memset (a, 0, 2 * n * sizeof (double));

    for (int i = 0; i < 2 * n; i++)
        a[i] = 0;

    double *g = a + n;
    double  z = 0;

    for (int j = 1; j <= n; j++)
    {
        z =  2 * cos(pi * (2 * j - 1) / (2*n));

        g[0]  = f_x[j - 1];
        a[0] += g[0];
        g[1]  = g[0] * z / 2;
        a[1] += g[1];
        for (int i = 2; i < n; i++)
        {
            g[i]  = z * g[i - 1] - g[i - 2];
            a[i] += g[i];
        }
    }

    a[0] = a[0] / n;
    for (int i = 1; i < n; i++)
        a[i] = a[i] * 2 / n;

    return 0;
}

double ChebPol (double *alf, int n, double p, double a, double b)
{
    double z = 2 * (2 * p - (b + a)) / (b - a);

    double Pf, T;
    double prev_T = z / 2, prev_prev_T = 1;

    Pf = alf[0] + alf[1] * prev_T;
    T = z * prev_T - prev_prev_T;

    for (int i = 2; i < n; i++)
    {
        Pf += alf[i] * T;
        prev_prev_T = prev_T;
        prev_T = T;
        T = z * prev_T - prev_prev_T;
    }

    return Pf;
}

int CubeSplineCoeff( int n, double *x, double *f_x, double *a )
{
    for (int i = 0; i < 4 * n; i++)
        a[i] = 0;

    double *a2, *a3, *a4;
    a2 = a + n;
    a3 = a + 2 * n;
    a4 = a + 3 * n;

    for (int i = 1; i < n; i++)
        a[i - 1] = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);

    CreateMatrix( a, x, n );
    ThreeDiagSolve( a4, a, a2, a3, n );

    for (int i = 1; i < n; i++)
        a[i - 1] = (f_x[i] - f_x[i - 1]) / (x[i] - x[i - 1]);

    for (int i = 0; i < n - 1; i++)
        a3[i] = (a[i] - a4[i]) / (x[i + 1] - x[i]);

    for (int i = 0; i < n - 1; i++)
        a2[i] = (a4[i] + a4[i + 1] - 2 * a[i]) / ((x[i + 1] - x[i]) * (x[i + 1] - x[i]));

    for( int i = 0; i < n - 1; i++ )
       a3[i] = a3[i] - a2[i] * (x[i+1] - x[i]);

    return 0;
}

double CubeSplinePol( double *alf, double *x, double *f_x, int n, double p )
{
    int head = 0, tail = n - 1;
    int old_tail = tail;
    double a = x[head], b = x[tail];
    int h;

        while( 1 )
        {
            if ( fabs( b - p ) > 1e-12 )
            {

                if(( p > a || fabs( a - p ) < 1e-15 ) && ( p < b))
                {
                    if( tail - head == 1 )
                        break;
                    old_tail = tail;
                    tail = tail - (tail - head) / 2;

                    b = x[tail];
                }
                else
                {
                    head = tail;
                    tail = old_tail;
                    a = b;
                    b = x[tail];
                }
            }
            else
            {
                head = tail;
                break;
            }
        }
        h = head;


    /*int h = -1;
    for (int i = 0; i < n - 1; i++)
    {
        h++;
        if (((p > x[i]) || (fabs (x[i] - p) < 1e-15))
          && (p < x[i + 1]))
                break;
    }*/

    return  f_x[h]
            + alf[3 * n + h] * (p - x[h])
            + alf[2 * n + h] * (p - x[h]) * (p - x[h])
            + alf[1 * n + h] * (p - x[h]) * (p - x[h]) * (p - x[h]);
}

























