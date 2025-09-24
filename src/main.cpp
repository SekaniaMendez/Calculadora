#include "UICalculator.h"
#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv); // inicializa el sistema Qt
  UICalculator screen1;
  screen1.show();

  return app.exec(); // inicia el loop de eventos
}
