#pragma once
#include <optional>

/**
 * @file engine.h
 * @brief Declaration of the Engine class (core calculation engine).
 *
 * The Engine stores two operands and an operator, and exposes per-operation
 * methods to evaluate results. It is intentionally UI-agnostic; formatting and
 * presentation are handled by the UI layer (e.g., UICalculator).
 *
 * @author Giovanni Daniel Mendez Sanchez (B54354)
 * @date 2025-09-24
 */
class Engine {
public:
  /**
   * @brief Supported operations for the engine.
   * - Arithmetic: Add, Sub, Mul, Div
   * - Base conversions: ToDec, ToHex, ToOct (UI formats the string; engine
   *   only passes through the numeric value1_ when present)
   */
  enum class Op {
    None,
    Add,
    Sub,
    Mul,
    Div,
    ToDec,
    ToHex,
    ToOct,
    ToBin,
    Random
  };

  // --- State management ---
  /** @brief Reset all internal state (operands and operator). */
  void clear();

  /** @brief Set the current operator. @param op Operator to apply. */
  void setOp(Op op);
  /** @brief Get the current operator. @return Current operator. */
  Op op() const;

  /** @brief Set first operand and mark it present. @param v Operand value. */
  void setValue1(long double v);
  /** @brief Set second operand and mark it present. @param v Operand value. */
  void setValue2(long double v);
  /** @brief Get first operand. @return value1_. */
  long double value1() const;
  /** @brief Get second operand. @return value2_. */
  long double value2() const;
  /** @brief Whether first operand is present. */
  bool hasV1() const;
  /** @brief Whether second operand is present. */
  bool hasV2() const;

  // --- Arithmetic operations (use stored operands) ---
  /**
   * @brief Sum of value1_ and value2_.
   * @return Result if both operands are present; std::nullopt otherwise.
   */
  std::optional<long double> add() const;
  /**
   * @brief Difference value1_ - value2_.
   * @return Result if both operands are present; std::nullopt otherwise.
   */
  std::optional<long double> sub() const;
  /**
   * @brief Product value1_ * value2_.
   * @return Result if both operands are present; std::nullopt otherwise.
   */
  std::optional<long double> mul() const;
  /**
   * @brief Quotient value1_ / value2_.
   * @return Result if both operands are present and value2_ != 0; std::nullopt
   *         on division by zero or missing inputs.
   */
  std::optional<long double>
  div() const; // nullopt on division by zero or missing inputs

  // --- Base/display related operations (act on value1 only) ---
  /**
   * @brief Identity over value1_ when present (UI formats as decimal).
   * @return value1_ if present; std::nullopt otherwise.
   */
  std::optional<long double> toDec() const; // returns value1 if present
  /**
   * @brief Identity over value1_ when present (UI formats as hexadecimal).
   * @return value1_ if present; std::nullopt otherwise.
   */
  std::optional<long double>
  toHex() const; // value1 passthrough; UI formats base
  /**
   * @brief Identity over value1_ when present (UI formats as octal).
   * @return value1_ if present; std::nullopt otherwise.
   */
  std::optional<long double>
  toOct() const; // value1 passthrough; UI formats base

  /**
   * @brief Identity over value1_ when present (UI formats as binary).
   * @return value1_ if present; std::nullopt otherwise.
   */
  std::optional<long double>
  toBin() const; // value1 passthrough; UI formats base

  /**
   * @brief Generate a random number between 0 and |max|.
   * @param max Upper bound (absolute value is used if negative).
   * @return A random value between 0 and abs(max).
   */
  std::optional<long double> random(long long int max) const;

  // --- Dispatch helper using current op (implemented in engine.cpp) ---
  /**
   * @brief Evaluate according to the current operator and stored operands.
   * @return The computed result or std::nullopt on invalid state.
   */
  std::optional<long double> evaluate() const;

private:
  long double value1_ = 0.0L; ///< First operand.
  long double value2_ = 0.0L; ///< Second operand.
  Op op_ = Op::None;          ///< Current operator.
  bool hasV1_ = false;        ///< Whether value1_ is set.
  bool hasV2_ = false;        ///< Whether value2_ is set.
};
