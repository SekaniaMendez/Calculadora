/****
 * @file UICalculator.h
 * @brief Declaration of the UICalculator class, the main UI window for the
 * calculator.
 */
#pragma once
#include <QWidget>

// Forward declarations (keep header light)
class QGridLayout;
class QLineEdit;
class QPushButton;
class QKeyEvent;

class UICalculator : public QWidget {
  Q_OBJECT

public:
  explicit UICalculator(QWidget *parent = nullptr);
  ~UICalculator() = default;

protected:
  // Keyboard input handler
  void keyPressEvent(QKeyEvent *event) override;

private:
  // --- Helpers ---
  void createDigitButtons();
  void createUtilityAndOperatorButtons();
  void commitCurrentNumber();
  void appendDigit(int d);

  // --- Widgets ---
  QGridLayout *btnOrganizer = nullptr; // grid layout for all controls
  QLineEdit *symbolShower = nullptr;   // display

  // Utility / operator buttons
  QPushButton *btnClr = nullptr; // clear
  QPushButton *btnBck = nullptr; // backspace
  QPushButton *btnHex = nullptr; // (optional) hex base
  QPushButton *btnOct = nullptr; // (optional) oct base
  QPushButton *btnDec = nullptr; // (optional) dec base
  QPushButton *btnRan = nullptr; // (optional) random
  QPushButton *btnEql = nullptr; // equals
  QPushButton *btnSub = nullptr; // minus
  QPushButton *btnAdd = nullptr; // plus
  QPushButton *btnMul = nullptr; // multiply
  QPushButton *btnDiv = nullptr; // divide

  // Digits 0..9
  QPushButton *digitButtons[10] = {nullptr};

  // --- Number entry state ---
  long double value1_ = 0.0L;
  long double value2_ = 0.0L;
  bool enteringFirst_ = true; // true: filling value1_, false: filling value2_
};