
#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class Window : public QWidget
{
  Q_OBJECT

private:
  double a;
  double b;
  bool isRepaintCheb;
  bool isRepaintSpline;
  int n_cheb;
  int n_spline;

public:
  Window (QWidget *parent);

  QSize minimumSizeHint () const;
  QSize sizeHint () const;
  double *x_cheb;
  double *f_x_cheb;
  double *q_cheb;
  double *q_spline;
  double *x_spline;
  double *f_x_spline;

  void points_cheb();
  void points_spline();
  int parse_command_line (int argc, char *argv[]);

public slots:
  void more_points ();
  void less_points ();

protected:
  void paintEvent (QPaintEvent *event);
};

#endif
