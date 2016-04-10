int GetData(int argc, char *argv[], double &a, double &b, int &n, double *x, double *f_x);
double LinearInterpolate (double x1, double x2, double y1, double y2, double a);
double f (double x);
int ChebCoeff (int n, double *f_x, double *a);
double ChebPol (double *alf, int n, double p, double a, double b);
void CreateMatrix( double *a, double *x, int n );
void ThreeDiagSolve( double *b, double *a, double *c, double *d, int n );
int CubeSplineCoeff( int n, double *x, double *f_x, double *a );
double CubeSplinePol(double *alf, double *x, double *f_x, int n, double p );
