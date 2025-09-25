/****
 * @file UICalculator.h
 * @brief Declaration of the UICalculator class (main application window).
 *
 * This header defines the Qt Widgets-based UI container. The class manages the
 * display, digit/operator buttons, and keyboard interaction. Arithmetic logic
 * is delegated to the Engine class.
 */
#pragma once

#include <QString>
#include <QWidget>

// Lightweight forward declarations to keep the header minimal
class QGridLayout;
class QLineEdit;
class QPushButton;
class QKeyEvent;
class Engine;

/**
 * @class UICalculator
 * @brief Calculator window implemented with QWidget.
 *
 * @details
 * UICalculator builds a grid (QGridLayout) containing a display and the
 * buttons required for a basic calculator. It supports both keyboard and mouse
 * input and coordinates with the calculation engine (Engine) to evaluate
 * operations and show results.
 *
 * ### Responsibilities
 * - Create and arrange widgets (display, digits, operators).
 * - Handle user input (keyboard and mouse).
 * - Convert the textual display to numeric values.
 * - Coordinate with `Engine` to prepare/evaluate operations.
 * - (Optional) Format output in decimal/hexadecimal/octal.
 *
 * ### Non-responsibilities
 * - Performing low-level arithmetic (delegated to `Engine`).
 */
class UICalculator : public QWidget {
  Q_OBJECT

public:
  /**
   * @brief Construct the main calculator window.
   * @param parent Optional QWidget parent.
   */
  explicit UICalculator(QWidget *parent = nullptr);
  /// Default destructor.
  ~UICalculator() = default;

protected:
  /**
   * @brief Handle keyboard input.
   *
   * Supports digits (0–9), decimal point, Backspace, Escape (clear), operators
   * (+, −, ×, ÷) and Enter/Return (=).
   *
   * @param event Key press event.
   */
  void keyPressEvent(QKeyEvent *event) override;

private:
  // ======================= Helpers (construction / interaction)
  // =======================
  /**
   * @brief Create digit buttons (0–9) and the decimal point, place them onto
   *        the grid, and connect clicks to input logic.
   */
  void createDigitButtons();

  /**
   * @brief Create utility buttons (Clr, Back, Dec/Hex/Oct) and operator
   *        buttons (+, −, ×, ÷, =), place them onto the grid, and connect
   *        their signals to the appropriate handlers.
   */
  void createUtilityAndOperatorButtons();

  /**
   * @brief Convert the display text into a number and register it as the
   *        active operand (value1_ or value2_), depending on input state.
   *
   * After commit, the display resets to "0" to capture the next number.
   */
  void commitCurrentNumber();

  /**
   * @brief Append a digit to the display text, handling the initial "0" case.
   * @param d Digit (0–9) to append.
   */
  void appendDigit(int d);

  /**
   * @brief Handler for binary operator click/key.
   * @param opCode Operator code: 0:+, 1:−, 2:×, 3:÷.
   */
  void onOperatorPressed(int opCode); // 0:+ 1:- 2:* 3:/

  /**
   * @brief Handler for equals (=) button/key. Evaluates via Engine and shows
   *        the result, preparing state for chained operations.
   */
  void onEqualsPressed();

  /**
   * @brief Handler for base selection.
   * @param baseCode 0: decimal, 1: hexadecimal, 2: octal.
   */
  void onBasePressed(int baseCode); // 0:Dec 1:Hex 2:Oct

  /**
   * @brief Handler for binary base conversion.
   *
   * Uses Engine::ToBin and formats the result in base-2.
   */
  void onBinPressed();

  /**
   * @brief Handler for random number generation.
   *
   * Uses Engine::Random and displays the generated value.
   */
  void onRandomPressed();

  /**
   * @brief Format a value for the display in the given base.
   * @param v Value to present.
   * @param baseCode 0: dec, 1: hex, 2: oct.
   * @return Formatted string for the display.
   */
  QString formatValue(long double v,
                      int baseCode) const; // baseCode: 0 dec,1 hex,2 oct

  // ================================== Widgets
  // ==================================
  QGridLayout *btnOrganizer = nullptr; ///< Main grid organizing all controls.
  QLineEdit *symbolShower = nullptr;   ///< Calculator display.

  // Utility / operator buttons
  QPushButton *btnClr = nullptr; ///< Clear (reset display and input state).
  QPushButton *btnBck = nullptr; ///< Backspace (remove one character).
  QPushButton *btnHex = nullptr; ///< (Optional) switch to hexadecimal view.
  QPushButton *btnOct = nullptr; ///< (Optional) switch to octal view.
  QPushButton *btnDec = nullptr; ///< (Optional) switch to decimal view.
  QPushButton *btnRan = nullptr; ///< (Optional) helper button (Rnd).
  QPushButton *btnBin = nullptr; ///< (Optional) switch to binary view.
  QPushButton *btnEql = nullptr; ///< Equals (=) triggers evaluation.
  QPushButton *btnSub = nullptr; ///< Subtraction (−).
  QPushButton *btnAdd = nullptr; ///< Addition (+).
  QPushButton *btnMul = nullptr; ///< Multiplication (×).
  QPushButton *btnDiv = nullptr; ///< Division (÷).

  /// Array of digit buttons (0..9). Entries may be null until created.
  QPushButton *digitButtons[10] = {nullptr};

  // =============================== Input state ===============================
  long double value1_ = 0.0L; ///< First accumulated operand.
  long double value2_ = 0.0L; ///< Second accumulated operand.
  bool enteringFirst_ =
      true;                  ///< true while filling value1_, false for value2_.
  Engine *engine_ = nullptr; ///< Calculation engine managed by the UI.
};