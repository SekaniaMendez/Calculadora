/****
 * @file UICalculator.h
 * @brief Declaration of the UICalculator class, the main UI window for the
 * calculator.
 */
#pragma once
#include <QtWidgets>

class UICalculator : public QWidget {
  Q_OBJECT
private:
  // shows the numbers or symbols that were press.
  QLineEdit *symbolShower;
  // Use to organice the symbols in the window.
  QGridLayout *symbolOrganizer;
  QPushButton *btnClr;
  QPushButton *btnHex;
  QPushButton *btnOct;
  QPushButton *btnDec;
  QPushButton *btnRan;
  QPushButton *btnEql;
  QPushButton *btnRes;
  QPushButton *btnAdd;
  QPushButton *btnMul;
  QPushButton *btnDiv;

public:
  // Constructor
  explicit UICalculator(/* args */) { QWidget *parent = nullptr; }

  // Destructor
  ~UICalculator() = default;
};