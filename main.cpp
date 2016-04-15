
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <iostream>

#include "window.h"
#include "prots.h"

using namespace std;

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
  Window *graph_area = new Window (window);
  QAction *action;

  if (graph_area->parse_command_line (argc, argv) != 0)
  {
      QMessageBox::warning (0, "Wrong input arguments!",
                            "Wrong input arguments!");
      return -1;
  }

  graph_area->points_cheb();
  graph_area->points_spline();

  action = tool_bar->addAction ("Points *", graph_area, SLOT (more_points ()));
  action->setShortcut (QString ("*"));

  action = tool_bar->addAction ("Points /", graph_area, SLOT (less_points()));
  action->setShortcut (QString ("/"));

  action = tool_bar->addAction ("E&xit", window, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+X"));

  action = tool_bar->addAction( "&Change method", graph_area, SLOT( change_method() ));
  action->setShortcut( QString( "Ctrl+C" ) );

  action = tool_bar->addAction( "&Delta function", graph_area, SLOT( delta_func() ));
  action->setShortcut( QString( "Ctrl+D" ));

  action = tool_bar->addAction( "Minus delt&a function", graph_area, SLOT( minus_delta_func() ));
  action->setShortcut( QString( "Ctrl+A" ));

  action = tool_bar->addAction( "Zoom+", graph_area, SLOT( more_zoom() ));
  action->setShortcut( QString( "+" ));

  action = tool_bar->addAction( "Zoom-", graph_area, SLOT(less_zoom() ));
  action->setShortcut( QString( "-" ));

  tool_bar->setMaximumHeight (30);

  window->setMenuBar (tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Graph");

  window->show ();
  return app.exec ();
}
