#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setApplicationName("Polynomial Algebra System");
  app.setApplicationVersion("1.0");
  app.setStyle(QStyleFactory::create("Fusion"));
  MainWindow window;
  window.show();
  return app.exec();
}
