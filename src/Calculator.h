#pragma once

#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <variant>

class Calculator {
 public:
  struct Operation {
    uint8_t priority;

    enum class Type {
      UnaryMinus,
      UnaryPlus,
      BinaryMinus,
      BinaryPlus,
      Multiplication,
      Division,
      OpeningBracket,
      ClosingBracket,
      None
    } type;

    Operation() : priority(0), type(Type::None) {}

    explicit Operation(Type type) : type(type) {
      switch (type) {
        case Type::Multiplication:
        case Type::Division:
        case Type::UnaryMinus:
        case Type::UnaryPlus:
          priority = 2;
          break;
        case Type::BinaryPlus:
        case Type::BinaryMinus:
          priority = 1;
          break;
        default:
          priority = 0;
      }
    }

    bool IsUnary() const {
      return type == Type::UnaryMinus || type == Type::UnaryPlus;
    }

    bool IsBinary() const {
      return !IsUnary();
    }
  };

  typedef std::vector<std::variant<double, Operation>> RPN;

  static double Evaluate(const std::string &query);
  static double Evaluate(const RPN &rpn);
 private:
  static RPN BuildRPN(const std::string &query);
};