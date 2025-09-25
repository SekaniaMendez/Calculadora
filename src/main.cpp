/****
 * @file main.cpp
 * @brief Application entry point for the Calculator (Qt Widgets).
 *
 * Initializes the Qt application, constructs the main UI window (UICalculator),
 * shows it, and starts the Qt event loop.
 *
 * @author Giovanni Daniel Mendez Sanchez (B54354)
 * @date 2025-09-24
 */
#include "UICalculator.h"
#include <QApplication>
#include <QWidget>

/**
 * @brief Program entry point.
 *
 * Creates a QApplication instance, instantiates the calculator UI window and
 * shows it, then starts the Qt event loop.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Exit code returned by QApplication::exec().
 */
int main(int argc, char *argv[]) {
  QApplication app(argc, argv); // inicializa el sistema Qt
  UICalculator screen1;         // Ventana inicial.
  screen1.show();

  return app.exec(); // inicia el loop de eventos
}
