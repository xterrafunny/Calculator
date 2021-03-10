#include "Calculator.h"
#include <stack>
#include <cmath>

double ReadValue(const std::string &query, size_t &idx) {
  double value = query[idx] - '0';
  ++idx;
  size_t dot_idx = 0;
  while (idx < query.length() && query[idx] != ' ') {
    if (std::isdigit(query[idx])) {
      value = value * 10 + (query[idx] - '0');
    } else if (query[idx] == '.' || query[idx] == ',') {
      dot_idx = idx;
    } else {
      break;
    }
    ++idx;
  }

  if (dot_idx != 0) {
    if (idx - dot_idx > 3) {
      throw std::invalid_argument("Too much digits after dot");
    } else if (idx == dot_idx + 1) {
      throw std::invalid_argument("Number can't end with dot");
    }

    return value / std::pow(10, idx - dot_idx - 1);
  }

  return value;
}

bool IsOperation(char c) {
  switch (c) {
    case '*':
    case '/':
    case '+':
    case '-':
      return true;
    default:
      return false;
  }
}

Calculator::RPN Calculator::BuildRPN(const std::string &query) {
  enum class OperandType {
    OpeningBracket,
    ClosingBracket,
    Number,
    Operation,
    None
  } previous_operand = OperandType::None;

  RPN rpn;
  std::stack<Operation> operations;
  size_t idx = 0;

  while (idx < query.length()) {
    if (query[idx] == ' ') {
      ++idx;
    } else if (std::isdigit(query[idx])) {
      rpn.emplace_back(ReadValue(query, idx));
      previous_operand = OperandType::Number;
    } else if (query[idx] == '(') {
      operations.push(Operation(Operation::Type::OpeningBracket));
      previous_operand = OperandType::OpeningBracket;
      ++idx;
    } else if (query[idx] == ')') {
      while (!operations.empty() && operations.top().type != Operation::Type::OpeningBracket) {
        rpn.emplace_back(operations.top());
        operations.pop();
      }
      if (operations.empty() || operations.top().type != Operation::Type::OpeningBracket) {
        throw std::invalid_argument("Brackets mismatch");
      }
      operations.pop();
      previous_operand = OperandType::ClosingBracket;
      ++idx;
    } else if (IsOperation(query[idx])) {
      bool is_prefix = false;
      if (query[idx] == '+') {
        switch (previous_operand) {
          case OperandType::None:
          case OperandType::OpeningBracket:
            operations.push(Operation(Operation::Type::UnaryPlus));
            is_prefix = true;
            break;
          default:
            break;
        }
      } else if (query[idx] == '-') {
        switch (previous_operand) {
          case OperandType::None:
          case OperandType::OpeningBracket:
            operations.push(Operation(Operation::Type::UnaryMinus));
            is_prefix = true;
            break;
          default:
            break;
        }
      }

      if (is_prefix) {
        previous_operand = OperandType::Operation;
        ++idx;
        continue;
      }

      Operation current;
      switch (query[idx]) {
        case '*':
          current = Operation(Operation::Type::Multiplication);
          break;
        case '/':
          current = Operation(Operation::Type::Division);
          break;
        case '+':
          current = Operation(Operation::Type::BinaryPlus);
          break;
        case '-':
          current = Operation(Operation::Type::BinaryMinus);
          break;
      }

      while (!operations.empty()
          && (operations.top().IsUnary() || operations.top().priority >= current.priority)) {
        rpn.emplace_back(operations.top());
        operations.pop();
      }
      operations.push(current);
      previous_operand = OperandType::Operation;
      ++idx;
    } else {
      throw std::invalid_argument("Incorrect symbol");
    }
  }

  while (!operations.empty()) {
    if (operations.top().type == Operation::Type::OpeningBracket) {
      throw std::invalid_argument("Brackets mismatch");
    }
    rpn.emplace_back(operations.top());
    operations.pop();
  }
  return rpn;
}

double Calculator::Evaluate(const Calculator::RPN &rpn) {
  std::stack<double> values;

  for (const auto &item: rpn) {
    switch (item.index()) {
      case 0:
        values.push(std::get<double>(item));
        break;
      case 1: {
        const auto &op = std::get<Operation>(item);

        if (values.empty()) {
          throw std::invalid_argument("fail");
        }

        if (op.type == Operation::Type::UnaryMinus) {
          double lhs = values.top();
          values.pop();
          values.push(-1 * lhs);
        }

        if (op.IsUnary()) {
          continue;
        }

        if (values.size() < 2) {
          throw std::invalid_argument("fail");
        }

        double rhs = values.top();
        values.pop();
        double lhs = values.top();
        values.pop();

        switch (op.type) {
          case Operation::Type::BinaryPlus:
            values.push(lhs + rhs);
            break;
          case Operation::Type::BinaryMinus:
            values.push(lhs - rhs);
            break;
          case Operation::Type::Multiplication:
            values.push(lhs * rhs);
            break;
          case Operation::Type::Division:
            values.push(lhs / rhs);
            break;
        }
      }
    }
  }

  if (values.size() > 1) {
    throw std::invalid_argument("fail");
  }

  return std::round(values.top() * 100) / 100;
}

double Calculator::Evaluate(const std::string &query) {
  return Evaluate(BuildRPN(query));
}
