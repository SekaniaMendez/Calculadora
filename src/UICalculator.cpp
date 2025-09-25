/**
 * @file UICalculator.cpp
 * @brief Implementation of the UICalculator class which provides a
 * multifunctional calculator UI.
 *
 * This file contains the implementation of the UICalculator class, responsible
 * for creating and managing the calculator's user interface, handling user
 * input via buttons and keyboard, and interacting with the calculation engine
 * to perform arithmetic and base conversion operations.
 */

#include "UICalculator.h"
#include "engine.h"
#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

/// @brief Constructor of the User Interface.
/// @param parent Widget pointer to which the class will be casted.
///
/// Initializes the calculator UI components, sets up the layout, creates digit
/// and operator buttons, and prepares the internal calculation engine.
UICalculator::UICalculator(QWidget *parent) : QWidget(parent) {
  qDebug() << "[UICalculator] ENTER ctor";
  engine_ = new Engine();
  setWindowTitle("Multifunctional Calculator");
  // Grid properties.
  qDebug() << "[UICalculator] Creating layout";
  btnOrganizer = new QGridLayout();
  setLayout(btnOrganizer);
  btnOrganizer->setContentsMargins(8, 8, 8, 8);
  btnOrganizer->setSpacing(5);
  // Allow last row (6) to expand for base buttons
  btnOrganizer->setRowStretch(6, 1);
  qDebug() << "[UICalculator] Layout ready";

  // -Display-
  // Commands shower properties.
  qDebug() << "[UICalculator] Creating display";
  symbolShower = new QLineEdit();
  symbolShower->setReadOnly(true);
  symbolShower->setAlignment(Qt::AlignCenter);
  symbolShower->setText("0");
  btnOrganizer->addWidget(symbolShower, 0, 0, 1, 4);
  qDebug() << "[UICalculator] Display placed";
  // buttons
  qDebug() << "[UICalculator] Calling createUtilityAndOperatorButtons()";
  createUtilityAndOperatorButtons();
  qDebug() << "[UICalculator] Utilities/operators created";
  // numbers
  qDebug() << "[UICalculator] Calling createDigitButtons()";
  createDigitButtons();
  qDebug() << "[UICalculator] Digits created";
  qDebug() << "[UICalculator] EXIT ctor";
}

/// @brief Creates the digit buttons layer and places them in the layout.
///
/// Sets up digit buttons 0-9 and the decimal point button, connects their
/// signals to the appropriate slots. Buttons are arranged in a grid layout
/// resembling a typical calculator keypad.
void UICalculator::createDigitButtons() {
  qDebug() << "[UICalculator] ENTER createDigitButtons";
  const char *numLabels[3][3] = {
      {"7", "8", "9"}, {"4", "5", "6"}, {"1", "2", "3"}};
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      int d = numLabels[row][col][0] - '0';
      digitButtons[d] = new QPushButton(QString::number(d));
      btnOrganizer->addWidget(digitButtons[d], row + 2, col);
      connect(digitButtons[d], &QPushButton::clicked, this,
              [this, d] { appendDigit(d); });
    }
  }
  // Botón 0 y punto
  digitButtons[0] = new QPushButton("0");
  QPushButton *btnDot = new QPushButton(".");
  btnOrganizer->addWidget(digitButtons[0], 5, 0, 1, 2);
  btnOrganizer->addWidget(btnDot, 5, 2);
  connect(digitButtons[0], &QPushButton::clicked, this,
          [this] { appendDigit(0); });
  connect(btnDot, &QPushButton::clicked, this, [this] {
    if (!symbolShower)
      return;
    QString cur = symbolShower->text();
    if (!cur.contains('.'))
      symbolShower->setText(cur + ".");
  });
  qDebug() << "[UICalculator] EXIT createDigitButtons";
}

/// @brief Creates utility and operator buttons and places them in the layout.
///
/// Buttons include clear, backspace, base conversion (Dec, Hex, Oct),
/// arithmetic operators (+, -, *, /), equals, and random number generator.
/// Connects their signals to appropriate slots for handling user actions.
void UICalculator::createUtilityAndOperatorButtons() {
  qDebug() << "[UICalculator] ENTER createUtilityAndOperatorButtons";
  Q_ASSERT(btnOrganizer);
  if (!btnOrganizer)
    return;

  // Create all buttons (no parent; layout will reparent)
  if (!btnClr)
    btnClr = new QPushButton("Clr");
  if (!btnBck)
    btnBck = new QPushButton("<-");
  if (!btnDec)
    btnDec = new QPushButton("Dec");
  if (!btnDiv)
    btnDiv = new QPushButton("/");
  if (!btnMul)
    btnMul = new QPushButton("*");
  if (!btnSub)
    btnSub = new QPushButton("-");
  if (!btnAdd)
    btnAdd = new QPushButton("+");
  if (!btnEql)
    btnEql = new QPushButton("=");
  if (!btnHex)
    btnHex = new QPushButton("Hex");
  if (!btnOct)
    btnOct = new QPushButton("Oct");
  if (!btnRan)
    btnRan = new QPushButton("Rnd");
  if (!btnBin)
    btnBin = new QPushButton("Bin");

  // Place them in the grid
  qDebug() << "[UICalculator] placing top row";
  btnOrganizer->addWidget(btnClr, 1, 0);
  btnOrganizer->addWidget(btnBck, 1, 1);
  btnOrganizer->addWidget(btnDec, 1, 2);
  btnOrganizer->addWidget(btnDiv, 1, 3);

  qDebug() << "[UICalculator] placing right column (*,-,+)";
  btnOrganizer->addWidget(btnMul, 2, 3);
  btnOrganizer->addWidget(btnSub, 3, 3);
  btnOrganizer->addWidget(btnAdd, 4, 3);

  qDebug() << "[UICalculator] placing equals";
  btnOrganizer->addWidget(btnEql, 5, 3);

  // Optional extra row: show Hex/Oct/Rnd
  qDebug() << "[UICalculator] placing base row (Hex/Oct/Rnd)";
  btnOrganizer->addWidget(btnHex, 6, 0);
  btnOrganizer->addWidget(btnOct, 6, 1);
  btnOrganizer->addWidget(btnRan, 6, 2);
  btnOrganizer->addWidget(btnBin, 6, 3);

  // Connections
  connect(btnClr, &QPushButton::clicked, this, [this] {
    symbolShower->setText("0");
    value1_ = 0.0L;
    value2_ = 0.0L;
    enteringFirst_ = true;
  });
  connect(btnBck, &QPushButton::clicked, this, [this] {
    if (!symbolShower)
      return;
    QString cur = symbolShower->text();
    if (cur.size() <= 1) {
      symbolShower->setText("0");
      return;
    }
    cur.chop(1);
    symbolShower->setText(cur);
  });
  connect(btnAdd, &QPushButton::clicked, this,
          [this] { onOperatorPressed(0); });
  connect(btnSub, &QPushButton::clicked, this,
          [this] { onOperatorPressed(1); });
  connect(btnMul, &QPushButton::clicked, this,
          [this] { onOperatorPressed(2); });
  connect(btnDiv, &QPushButton::clicked, this,
          [this] { onOperatorPressed(3); });
  connect(btnDec, &QPushButton::clicked, this, [this] { onBasePressed(0); });
  connect(btnHex, &QPushButton::clicked, this, [this] { onBasePressed(1); });
  connect(btnOct, &QPushButton::clicked, this, [this] { onBasePressed(2); });
  connect(btnBin, &QPushButton::clicked, this, [this] { onBinPressed(); });
  connect(btnRan, &QPushButton::clicked, this, [this] { onRandomPressed(); });
  connect(btnEql, &QPushButton::clicked, this, [this] { onEqualsPressed(); });

  qDebug() << "[UICalculator] EXIT createUtilityAndOperatorButtons";
}

/// @brief Handles keyboard input for digits, operators, and control keys.
/// @param event The key press event containing the key code.
///
/// Processes key presses to append digits, insert decimal points, handle
/// backspace, clear input, and trigger arithmetic operations or evaluation.
void UICalculator::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_0:
  case Qt::Key_1:
  case Qt::Key_2:
  case Qt::Key_3:
  case Qt::Key_4:
  case Qt::Key_5:
  case Qt::Key_6:
  case Qt::Key_7:
  case Qt::Key_8:
  case Qt::Key_9: {
    int d = event->key() - Qt::Key_0;
    appendDigit(d);
    if (d >= 0 && d <= 9 && digitButtons[d])
      digitButtons[d]->animateClick();
    break;
  }
  case Qt::Key_Period:
  case Qt::Key_Comma: {
    if (!symbolShower)
      break;
    QString cur = symbolShower->text();
    if (!cur.contains('.'))
      symbolShower->setText(cur + ".");
    break;
  }
  case Qt::Key_Backspace: {
    if (!symbolShower)
      break;
    QString cur = symbolShower->text();
    if (cur.size() <= 1) {
      symbolShower->setText("0");
      break;
    }
    cur.chop(1);
    symbolShower->setText(cur);
    if (btnBck)
      btnBck->animateClick();
    break;
  }
  case Qt::Key_Escape:
    if (symbolShower)
      symbolShower->setText("0");
    value1_ = 0.0L;
    value2_ = 0.0L;
    enteringFirst_ = true;
    if (engine_)
      engine_->clear();
    if (btnClr)
      btnClr->animateClick();
    break;
  case Qt::Key_Plus:
    onOperatorPressed(0);
    break;
  case Qt::Key_Minus:
    onOperatorPressed(1);
    break;
  case Qt::Key_Asterisk:
    onOperatorPressed(2);
    break;
  case Qt::Key_Slash:
    onOperatorPressed(3);
    break;
  case Qt::Key_Equal:
  case Qt::Key_Return:
  case Qt::Key_Enter:
    onEqualsPressed();
    break;
  default:
    QWidget::keyPressEvent(event);
    break;
  }
}

/// @brief Appends a digit to the current input displayed on the calculator
/// screen.
/// @param d The digit to append (0-9).
void UICalculator::appendDigit(int d) {
  if (!symbolShower)
    return;
  QString cur = symbolShower->text();
  if (cur == "0")
    cur.clear();
  symbolShower->setText(cur + QString::number(d));
}

/// @brief Commits the current number displayed into either the first or second
/// operand.
///
/// Converts the current displayed string to a floating point value and stores
/// it in value1_ or value2_ depending on whether the user is entering the first
/// or second operand.
void UICalculator::commitCurrentNumber() {
  if (!symbolShower)
    return;
  bool ok = false;
  double v = symbolShower->text().toDouble(&ok);
  long double lv = ok ? static_cast<long double>(v) : 0.0L;
  if (enteringFirst_) {
    value1_ = lv;
    enteringFirst_ = false;
  } else {
    value2_ = lv;
  }
  symbolShower->setText("0");
}

/// @brief Converts an integer operator code to the corresponding Engine::Op
/// enum value.
/// @param code Operator code (0=Add, 1=Sub, 2=Mul, 3=Div).
/// @return Corresponding Engine::Op enum value.
static inline Engine::Op fromCode(int code) {
  switch (code) {
  case 0:
    return Engine::Op::Add;
  case 1:
    return Engine::Op::Sub;
  case 2:
    return Engine::Op::Mul;
  case 3:
    return Engine::Op::Div;
  default:
    return Engine::Op::None;
  }
}

/// @brief Handles an operator button press.
/// @param opCode Integer code representing the operator (0=Add, 1=Sub, 2=Mul,
/// 3=Div).
///
/// Commits the current number, sets the operator in the engine, and prepares
/// for the next operand.
void UICalculator::onOperatorPressed(int opCode) {
  if (!engine_)
    return;
  // Store current entry into value1_/value2_
  commitCurrentNumber();
  // Ensure engine knows v1
  if (!engine_->hasV1())
    engine_->setValue1(value1_);
  engine_->setOp(fromCode(opCode));
}

/// @brief Handles the equals button press to evaluate the current expression.
///
/// Commits the current number as the second operand, evaluates the expression
/// using the engine, and updates the display with the result or an error
/// message.
void UICalculator::onEqualsPressed() {
  if (!engine_)
    return;
  // Finalize current entry into value2
  commitCurrentNumber();
  if (!engine_->hasV1())
    engine_->setValue1(value1_);
  if (!engine_->hasV2())
    engine_->setValue2(value2_);
  auto res = engine_->evaluate();
  if (res.has_value() && symbolShower) {
    long double r = *res;
    symbolShower->setText(QString::number(static_cast<double>(r)));
    // Prepare for chaining
    value1_ = r;
    value2_ = 0.0L;
    enteringFirst_ = false;
    engine_->clear();
    engine_->setValue1(value1_);
  } else {
    if (symbolShower)
      symbolShower->setText("Error");
    enteringFirst_ = true;
    value1_ = value2_ = 0.0L;
    engine_->clear();
  }
}

/// @brief Formats a numeric value for display according to the selected base.
/// @param v The numeric value to format.
/// @param baseCode The base code (0=Decimal, 1=Hexadecimal, 2=Octal).
/// @return Formatted string representation of the value in the specified base.
QString UICalculator::formatValue(long double v, int baseCode) const {
  switch (baseCode) {
  case 1: { // Hex
    long long n = static_cast<long long>(std::llround(static_cast<double>(v)));
    return QString::number(n, 16).toUpper();
  }
  case 2: { // Oct
    long long n = static_cast<long long>(std::llround(static_cast<double>(v)));
    return QString::number(n, 8);
  }
  case 3: { // Bin
    long long n = static_cast<long long>(std::llround(static_cast<double>(v)));
    return QString::number(n, 2);
  }
  case 0:
  default: // Dec
    return QString::number(static_cast<double>(v));
  }
}

/// @brief Handles base conversion button presses (Dec, Hex, Oct).
/// @param baseCode The base code indicating which base conversion to perform
/// (0=Dec, 1=Hex, 2=Oct).
///
/// Converts the current value displayed to the selected base and updates the
/// display accordingly.
void UICalculator::onBasePressed(int baseCode) {
  if (!engine_ || !symbolShower)
    return;
  // Use current display as value1
  bool ok = false;
  double v = symbolShower->text().toDouble(&ok);
  if (!ok)
    return;
  long double lv = static_cast<long double>(v);
  engine_->clear();
  engine_->setValue1(lv);
  // Map baseCode to engine op
  if (baseCode == 0)
    engine_->setOp(Engine::Op::ToDec);
  else if (baseCode == 1)
    engine_->setOp(Engine::Op::ToHex);
  else if (baseCode == 2)
    engine_->setOp(Engine::Op::ToOct);
  auto res = engine_->evaluate();
  if (res.has_value()) {
    symbolShower->setText(formatValue(*res, baseCode));
  }
}

/// @brief Handle Binary conversion button.
///
/// Uses Engine::ToBin and shows the value formatted in base-2.
void UICalculator::onBinPressed() {
  if (!engine_ || !symbolShower)
    return;
  bool ok = false;
  double v = symbolShower->text().toDouble(&ok);
  if (!ok)
    return;
  long double lv = static_cast<long double>(v);
  engine_->clear();
  engine_->setValue1(lv);
  engine_->setOp(Engine::Op::ToBin);
  auto res = engine_->evaluate();
  if (res.has_value()) {
    symbolShower->setText(formatValue(*res, 3)); // binary baseCode=3
  }
}

/// @brief Handle Random button.
///
/// Uses Engine::Random, displays the generated value in decimal, and prepares
/// it as value1 for chaining operations.
void UICalculator::onRandomPressed() {
  if (!engine_ || !symbolShower)
    return;
  engine_->clear();
  engine_->setOp(Engine::Op::Random);
  auto res = engine_->evaluate();
  if (res.has_value()) {
    long double r = *res;
    symbolShower->setText(QString::number(static_cast<double>(r)));
    // Prepare state: treat random as the first operand
    value1_ = r;
    value2_ = 0.0L;
    enteringFirst_ = false; // next typed number becomes value2_
    engine_->clear();
    engine_->setValue1(value1_);
  }
}
