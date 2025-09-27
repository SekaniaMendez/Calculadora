/**
 * @file engine.cpp
 * @brief Implementation of the Engine class (calculation engine).
 *
 * This file implements the Engine class methods declared in engine.h.
 * It manages state for two operands and an operator, and provides
 * arithmetic operations, base passthrough, and evaluation dispatch.
 *
 * @author Giovanni Daniel Mendez Sanchez (B54354)
 * @date 2025-09-24
 */

#include "engine.h"
#include <cmath>
#include <random>

// --- State management ---
/**
 * @brief Reset all state (operands, operator, flags).
 */
void Engine::clear() {
  value1_ = 0.0L;
  value2_ = 0.0L;
  op_ = Op::None;
  hasV1_ = false;
  hasV2_ = false;
}

/**
 * @brief Set the current operator.
 * @param op Operator to set.
 */
void Engine::setOp(Op op) { op_ = op; }

/**
 * @brief Get the current operator.
 * @return Current operator value.
 */
Engine::Op Engine::op() const { return op_; }

/**
 * @brief Set the first operand value.
 * @param v Value to set as first operand.
 */
void Engine::setValue1(long double v) {
  value1_ = v;
  hasV1_ = true;
}

/**
 * @brief Set the second operand value.
 * @param v Value to set as second operand.
 */
void Engine::setValue2(long double v) {
  value2_ = v;
  hasV2_ = true;
}

/**
 * @brief Get the first operand value.
 * @return The first operand.
 */
long double Engine::value1() const { return value1_; }

/**
 * @brief Get the second operand value.
 * @return The second operand.
 */
long double Engine::value2() const { return value2_; }

/**
 * @brief Check if first operand is set.
 * @return True if first operand is set, false otherwise.
 */
bool Engine::hasV1() const { return hasV1_; }

/**
 * @brief Check if second operand is set.
 * @return True if second operand is set, false otherwise.
 */
bool Engine::hasV2() const { return hasV2_; }

// --- Arithmetic ops ---
/**
 * @brief Add value1 and value2.
 * @return Sum of operands, or std::nullopt if operands missing.
 */
std::optional<long double> Engine::add() const {
  if (!hasV1_ || !hasV2_)
    return std::nullopt;
  return value1_ + value2_;
}

/**
 * @brief Subtract value2 from value1.
 * @return Difference of operands, or std::nullopt if operands missing.
 */
std::optional<long double> Engine::sub() const {
  if (!hasV1_ || !hasV2_)
    return std::nullopt;
  return value1_ - value2_;
}

/**
 * @brief Multiply value1 by value2.
 * @return Product of operands, or std::nullopt if operands missing.
 */
std::optional<long double> Engine::mul() const {
  if (!hasV1_ || !hasV2_)
    return std::nullopt;
  return value1_ * value2_;
}

/**
 * @brief Divide value1 by value2.
 * @return Quotient of operands, or std::nullopt if operands missing or division
 * by zero.
 */
std::optional<long double> Engine::div() const {
  if (!hasV1_ || !hasV2_)
    return std::nullopt;
  if (value2_ == 0.0L)
    return std::nullopt;
  return value1_ / value2_;
}

// --- Base ops (value1-only passthrough; UI handles formatting) ---
/**
 * @brief Pass through value1 as decimal.
 * @note UI handles formatting.
 * @return value1 if set, else std::nullopt.
 */
std::optional<long double> Engine::toDec() const {
  if (!hasV1_)
    return std::nullopt;
  return value1_;
}

/**
 * @brief Pass through value1 as hexadecimal.
 * @note UI handles formatting.
 * @return value1 if set, else std::nullopt.
 */
std::optional<long double> Engine::toHex() const {
  if (!hasV1_)
    return std::nullopt;
  return value1_;
}

/**
 * @brief Pass through value1 as octal.
 * @note UI handles formatting.
 * @return value1 if set, else std::nullopt.
 */
std::optional<long double> Engine::toOct() const {
  if (!hasV1_)
    return std::nullopt;
  return value1_;
}

/**
 * @brief Pass through value1 as binary (UI formats the string).
 * @note UI handles formatting to base-2.
 * @return value1 if set, else std::nullopt.
 */
std::optional<long double> Engine::toBin() const {
  if (!hasV1_)
    return std::nullopt;
  return value1_;
}

/**
 * @brief Generate a random number for the calculator.
 *
 * Currently returns an integer in [0, 999999]. The UI may format this value
 * or adjust the range as needed.
 *
 * @return A random value as long double.
 */
std::optional<long double> Engine::random() const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<long long> dist(0, 999999);
  return static_cast<long double>(dist(gen));
}

// --- Dispatch helper ---
/**
 * @brief Evaluate the current operation based on the operator and operands.
 * @return Result of the operation or std::nullopt if invalid or missing
 * operands.
 */
std::optional<long double> Engine::evaluate() const {
  switch (op_) {
  case Op::Add:
    return add();
  case Op::Sub:
    return sub();
  case Op::Mul:
    return mul();
  case Op::Div:
    return div();
  case Op::ToDec:
    return toDec();
  case Op::ToHex:
    return toHex();
  case Op::ToOct:
    return toOct();
  case Op::ToBin:
    return toBin();
  case Op::Random:
    return random();
  case Op::None:
  default:
    return std::nullopt;
  }
}
