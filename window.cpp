#include <QMessageBox>
#include <QPainter>
#include <stdio.h>
#include <math.h>
#include <iostream>


#include "window.h"
#include "prots.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 10
#define pi 3.1415926535897932
#define EPS 1e-6

using namespace std;

double f (double x)
{
  return x*cos(x);
}

Window::Window (QWidget *parent)
  : QWidget (parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  n_cheb   = DEFAULT_N;
  n_spline = DEFAULT_N;
  isRepaintCheb   = true;
  isRepaintSpline = true;
}

QSize Window::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize Window::sizeHint () const
{
  return QSize (1000, 1000);
}

int Window::parse_command_line (int argc, char *argv[])
{
  int n;

  if (argc == 1)
    return 0;

  if (argc == 2)
    return -1;

  if (   sscanf (argv[1], "%lf", &a) != 1
      || sscanf (argv[2], "%lf", &b) != 1
      || b - a < 1.e-6
      || (argc > 3 && sscanf (argv[3], "%d", &n) != 1)
      || n <= 0)
    return -2;

  n_cheb   = n;
  n_spline = n;
  return 0;
}

/// render graph
void Window::paintEvent (QPaintEvent * /* event */)
{
  if( isRepaintCheb == true )
  {
      ChebCoeff (n_cheb, f_x_cheb, q_cheb);
      isRepaintCheb = false;
  }
  if( isRepaintSpline == true )
  {
      CubeSplineCoeff (n_spline, x_spline, f_x_spline, q_spline);
      isRepaintSpline = false;
  }

  QPainter painter (this);
  double x1, x2, y1, y2;
  double max_y, min_y, old_min_y, old_max_y;
  double delta_y;
  double delta_x = (b - a) / (width());

  // calculate min and max for current function
  max_y = min_y = 0;
  for (x1 = a; x1 - b < 1.e-6; x1 += delta_x)
    {
      y1 = f (x1);
      if (y1 < min_y)
        min_y = y1;
      if (y1 > max_y)
        max_y = y1;
    }

  delta_y = 0.01 * (max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;
  painter.setRenderHint(QPainter::Qt4CompatiblePainting);
  painter.setRenderHint(QPainter::Antialiasing, true);

  // save current Coordinate System
  painter.save ();

  // make Coordinate Transformations
  painter.translate (0.5 * width (), 0.5 * height ());
  painter.scale (width () / (b - a), -height () / (max_y - min_y));
  painter.translate (-0.5 * (a + b), -0.5 * (min_y + max_y));

  //draw axis
  painter.setPen ("red");
  painter.drawLine (a, 0, b, 0);
  old_min_y = min_y;
  old_max_y = max_y;
  if ((max_y > 0) && (max_y < 1))
          max_y = 1;
  if  ((min_y < 0) && (min_y > -1))
            min_y = -1;

  painter.drawLine (0, max_y + 10, 0, min_y - 10);

  min_y = old_min_y;
  max_y = old_max_y;

  painter.setPen ( "purple" );

  // draw approximated line for graph
  //Cheb
  x1 = a;
  y1 = ChebPol (q_cheb, n_cheb, x1, a, b);
  for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
  {
      y2 = ChebPol (q_cheb, n_cheb, x2, a, b);
      painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

      x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = ChebPol (q_cheb, n_cheb, x2, a, b);
  painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

  //Spline
  painter.setPen( "blue" );
  x1 = a;
  y1 = CubeSplinePol (q_spline, x_spline, f_x_spline, n_spline, x1);
  for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
  {
      y2 = CubeSplinePol (q_spline, x_spline, f_x_spline, n_spline, x2);
      painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

      x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = CubeSplinePol (q_spline, x_spline, f_x_spline, n_spline, x2);
  painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

  //drow residual graphs
  double spline_res, cheb_res;
  //Spline
  painter.setPen( "aqua" );
  x1 = a;
  y1 = fabs( CubeSplinePol (q_spline, x_spline, f_x_spline, n_spline, x1) - f(x1) );
  spline_res = y1;
  for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
  {
      y2 = fabs( CubeSplinePol (q_spline, x_spline, f_x_spline, n_spline, x2) - f(x2) );
      painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));
      if( spline_res < y2 )
          spline_res = y2;

      x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = fabs( CubeSplinePol (q_spline, x_spline, f_x_spline, n_spline, x2) - f(x2) );
  painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));
  if( spline_res < y2 )
      spline_res = y2;

  //Cheb
  painter.setPen( "fuchsia" );
  x1 = a;
  y1 = fabs( ChebPol (q_cheb, n_cheb, x1, a, b) - f(x1) );
  cheb_res = y1;
  for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
  {
      y2 = fabs( ChebPol (q_cheb, n_cheb, x2, a, b) - f(x2) );
      painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));
      if( cheb_res < y2 )
          cheb_res = y2;

      x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = fabs( ChebPol (q_cheb, n_cheb, x2, a, b) - f(x2) );
  painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));
  if( cheb_res  < y2 )
      cheb_res = y2;

  painter.setPen( "green" );
  x1 = a;
  y1 = f(x1);
  for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
  {
      y2 = f(x2);
      painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

      x1 = x2, y1 = y2;
  }
  x2 = b;
  y2 = f(x2);
  painter.drawLine (QPointF (x1, y1), QPointF (x2, y2));

  //draw mesh
  painter.setPen (QPen(Qt::gray, 0, Qt::DotLine));
  delta_x = (b - a) / (n_spline - 1);
  x1 = a;
  for (int i = 0; i < n_spline; i++)
  {
    painter.drawLine (QPointF (x1, max_y), QPointF (x1, min_y));
    x1 += delta_x;
  }
  //draw arrows
  painter.setPen("red");
  QPolygonF polygon;
  QPolygonF polygon1;

  polygon << QPointF(b/1.025, max_y/25);
  polygon << QPointF( b, 0 );
  polygon << QPointF( b/1.025, -max_y/25);

  double p = (b - a) / (max_y - min_y);

  polygon1 << QPointF(max_y/1.04, p*max_y/45);
  polygon1 << QPointF(max_y, 0 );
  polygon1 << QPointF(max_y/1.04, -p*max_y/45);

  painter.setBrush(QBrush(Qt::red));
  painter.drawConvexPolygon( polygon );

  double arrowAngle = 90;

  painter.rotate( arrowAngle );
  painter.drawConvexPolygon( polygon1 );
  arrowAngle = -90;
  painter.rotate( arrowAngle );

  // restore previously saved Coordinate System
  painter.restore ();

  // print information
  QString str = "a  =  ";
  QString str_a = QString("%1").arg(a, 0, 'f', 0);

  str += str_a; str += ",  b  =  ";

  str_a = QString("%1").arg(a, 0, 'f', 0);
  str += str_a;

  painter.setPen( "black" );
  painter.drawText(0, 15, str);
  str =  QString("%1").arg((double)n_spline, 0, 'f', 0);
  painter.drawText(0, 90, QString( "n =  " ) + str);

  painter.setPen( "purple" );
  painter.drawText(0, 30, "Chebushev's Polygons");

  painter.setPen( "blue" );
  painter.drawText(0, 45, "Cube Splines");

  painter.setPen( "fuchsia" );
  str = QString("%1").arg( cheb_res, 0, 'e', 5 );
  painter.drawText( 0, 60, QString( "Chebushev residual  " ) + str );

  painter.setPen( "aqua" );
  str = QString("%1").arg( spline_res, 0, 'e', 5 );
  painter.drawText( 0, 75, QString( "Spline residual  " ) + str );
}

void Window::points_cheb()
{
    x_cheb   = new double [n_cheb];
    f_x_cheb = new double [n_cheb];
    q_cheb   = new double [2 * n_cheb];

    for (int i = 1; i <= n_cheb; i++)
    {
        x_cheb[i - 1]   = (a + b) / 2 + ((b - a) / 2) * cos (pi * (2 * i - 1) / (2 * n_cheb));
        f_x_cheb[i - 1] = f (x_cheb[i - 1]);
    }
}

void Window::points_spline()
{
    x_spline   = new double [n_spline];
    f_x_spline = new double [n_spline];
    q_spline   = new double [4 * n_spline];

    double delta_x = (b - a) / (n_spline - 1);
    double x1 = a;

    for (int i = 0; i < n_spline; i++)
    {
        x_spline[i]   = x1;
        f_x_spline[i] = f(x1);
        x1 += delta_x;
    }
}



void Window::more_points()
{
    if (n_cheb <= 25)
    {
        delete [] x_cheb;
        delete [] f_x_cheb;
        delete [] q_cheb;

        isRepaintCheb = true;

        n_cheb *= 2;

        points_cheb();
    }

    delete [] x_spline;
    delete [] f_x_spline;
    delete [] q_spline;

    isRepaintSpline = true;

    n_spline *= 2;

    points_spline();

    update ();
}

void Window::less_points()
{
    if (n_spline > 2)
    {

        delete [] x_spline;
        delete [] f_x_spline;
        delete [] q_spline;

        isRepaintSpline = true;

        n_spline /= 2;

        points_spline();

        if( n_cheb >= n_spline )
        {
            n_cheb = n_spline;

            delete [] x_cheb;
            delete [] f_x_cheb;
            delete [] q_cheb;

            isRepaintCheb = true;

            points_cheb();
        }

        update ();

    }
    else
    {
        QMessageBox::warning (0, "Achtung, fatal error!",
                              "It is not possible to devide 'Number of Points'");
        update();
    }
}
