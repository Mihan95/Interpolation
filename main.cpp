
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
  //коммент
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

  action = tool_bar->addAction ("Add points", graph_area, SLOT (more_points ()));
  action->setShortcut (QString ("*"));

  action = tool_bar->addAction ("Reduce points", graph_area, SLOT (less_points()));
  action->setShortcut (QString ("/"));

  action = tool_bar->addAction ("E&xit", window, SLOT (close ()));
  action->setShortcut (QString ("Ctrl+X"));

  tool_bar->setMaximumHeight (30);

  window->setMenuBar (tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Graph");

  window->show ();
  return app.exec ();
}
