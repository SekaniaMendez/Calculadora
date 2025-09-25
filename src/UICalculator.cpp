// src/UICalculator.cpp

#include "UICalculator.h"
#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

/// @brief Contructor of the User Interface.
/// @param parent widget pointer to which the class will be casted.
UICalculator::UICalculator(QWidget *parent) : QWidget(parent) {
  qDebug() << "[UICalculator] ENTER ctor";
  setWindowTitle("Multifunctional Calculator");
  // Grid properties.
  qDebug() << "[UICalculator] Creating layout";
  btnOrganizer = new QGridLayout();
  setLayout(btnOrganizer);
  btnOrganizer->setContentsMargins(8, 8, 8, 8);
  btnOrganizer->setSpacing(5);
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

/// @brief creates the digit numbers layer
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

  // Optional extra row
  // btnOrganizer->addWidget(btnHex, 6, 0);
  // btnOrganizer->addWidget(btnOct, 6, 1);
  // btnOrganizer->addWidget(btnRan, 6, 2);

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
  // For now, on operator press just commit the number being typed
  connect(btnAdd, &QPushButton::clicked, this,
          [this] { commitCurrentNumber(); });
  connect(btnSub, &QPushButton::clicked, this,
          [this] { commitCurrentNumber(); });
  connect(btnMul, &QPushButton::clicked, this,
          [this] { commitCurrentNumber(); });
  connect(btnDiv, &QPushButton::clicked, this,
          [this] { commitCurrentNumber(); });
  connect(btnEql, &QPushButton::clicked, this, [this] {
    commitCurrentNumber(); /* TODO evaluate */
  });

  qDebug() << "[UICalculator] EXIT createUtilityAndOperatorButtons";
}

/// @brief Handle keyboard input for digits and operators
/// @param event key press event
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
    if (btnClr)
      btnClr->animateClick();
    break;
  case Qt::Key_Plus:
  case Qt::Key_Minus:
  case Qt::Key_Asterisk:
  case Qt::Key_Slash:
    commitCurrentNumber();
    break;
  case Qt::Key_Equal:
  case Qt::Key_Return:
  case Qt::Key_Enter:
    commitCurrentNumber();
    // TODO: evaluate using value1_ and value2_
    break;
  default:
    QWidget::keyPressEvent(event);
    break;
  }
}

void UICalculator::appendDigit(int d) {
  if (!symbolShower)
    return;
  QString cur = symbolShower->text();
  if (cur == "0")
    cur.clear();
  symbolShower->setText(cur + QString::number(d));
}

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