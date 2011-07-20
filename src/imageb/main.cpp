#include <QApplication>
#include "imagebform.h"
#include <stdio.h>
#include <stdlib.h>

//! The main method that runs the applications and calls ImabeB main Window
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  ImageBForm imagebform;
  imagebform.show();
  return app.exec();
}

