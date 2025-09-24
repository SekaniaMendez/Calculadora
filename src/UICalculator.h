/****
 * @file UICalculator.h
 * @brief Declaration of the UICalculator class, the main UI window for the
 * calculator.
 */
#pragma once
#include <QWidget>

// Foward declaration helps optimize the software.
class QGridLayout;
class QLineEdit;
class QPushButton;

class UICalculator : public QWidget {
  // Qt macro to initialize any QWidget class.
  Q_OBJECT
private:
  // Use to organice the symbols in the window.
  QGridLayout *symbolOrganizer;
  // shows the numbers or symbols that were press.
  QLineEdit *symbolShower;
  // Clear buttom.
  QPushButton *btnClr;
  // Erase buttom.
  QPushButton *btnBck;
  // Hexadecimal buttom.
  QPushButton *btnHex;
  // Octal buttom.
  QPushButton *btnOct;
  // Decimal buttom.
  QPushButton *btnDec;
  // Random buttom.
  QPushButton *btnRan;
  // Equal buttom.
  QPushButton *btnEql;
  // Substract buttom.
  QPushButton *btnSub;
  // Add-Sum buttom.
  QPushButton *btnAdd;
  // Multiply buttom.
  QPushButton *btnMul;
  // Division buttom.
  QPushButton *btnDiv;
  // 0 - 9 buttoms.
  QPushButton *digitButtons[10];

public:
  // Constructor
  explicit UICalculator(QWidget *parent = nullptr);

  // Destructor
  ~UICalculator() = default;
};