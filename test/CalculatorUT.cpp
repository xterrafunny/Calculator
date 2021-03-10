#include "gtest/gtest.h"
#include <Calculator.h>
#include <cmath>

TEST(Calculator, Numbers) {
  EXPECT_EQ(Calculator::Evaluate("3"), 3.);
  EXPECT_EQ(Calculator::Evaluate("0"), 0.);
  EXPECT_EQ(Calculator::Evaluate("1.1"), 1.1);
  EXPECT_EQ(Calculator::Evaluate("1.12"), 1.12);
  EXPECT_THROW(Calculator::Evaluate("1.123"), std::invalid_argument);
  EXPECT_THROW(Calculator::Evaluate("1. 123"), std::invalid_argument);
  EXPECT_THROW(Calculator::Evaluate("1."), std::invalid_argument);
}

TEST(Calculator, SimpleCorrectPlus) {
  EXPECT_EQ(Calculator::Evaluate("0 + 0"), 0);
  EXPECT_EQ(Calculator::Evaluate("0 + 1"), 1);
  EXPECT_EQ(Calculator::Evaluate("1 + 0"), 1);
  EXPECT_EQ(Calculator::Evaluate("1 + 1"), 2);
  EXPECT_EQ(Calculator::Evaluate("+1"), 1);
  EXPECT_THROW(Calculator::Evaluate("++1"), std::invalid_argument);
}

TEST(Calculator, SimpleCorrectMinus) {
  EXPECT_EQ(Calculator::Evaluate("0 - 0"), 0);
  EXPECT_EQ(Calculator::Evaluate("0 - 1"), -1);
  EXPECT_EQ(Calculator::Evaluate("1 - 0"), 1);
  EXPECT_EQ(Calculator::Evaluate("1 - 1"), 0);
  EXPECT_EQ(Calculator::Evaluate("-1"), -1);
  EXPECT_THROW(Calculator::Evaluate("--1"), std::invalid_argument);
}

TEST(Calculator, SimpleCorrectMultiplication) {
  EXPECT_EQ(Calculator::Evaluate("0 * 0"), 0);
  EXPECT_EQ(Calculator::Evaluate("0 * 1"), 0);
  EXPECT_EQ(Calculator::Evaluate("1 * 0"), 0);
  EXPECT_EQ(Calculator::Evaluate("1 * 1"), 1);
  EXPECT_EQ(Calculator::Evaluate("1 * 2"), 2);
  EXPECT_EQ(Calculator::Evaluate("2 * 1"), 2);
}

TEST(Calculator, Brackets) {
  EXPECT_EQ(Calculator::Evaluate("(1)"), 1);
  EXPECT_EQ(Calculator::Evaluate("((1))"), 1);
  EXPECT_EQ(Calculator::Evaluate("()(1)"), 1);
  EXPECT_EQ(Calculator::Evaluate("(1)()"), 1);
  EXPECT_THROW(Calculator::Evaluate(")(1)"), std::invalid_argument);
  EXPECT_THROW(Calculator::Evaluate("(1)("), std::invalid_argument);
}

TEST(Calculator, SamplesFromEmail) {
  EXPECT_EQ(Calculator::Evaluate("-1 + 5 - 3"), 1);
  EXPECT_EQ(Calculator::Evaluate("-10 + (8 * 2.5) - (3 / 1,5)"), 8);
  EXPECT_EQ(Calculator::Evaluate("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)"), 11);
  EXPECT_THROW(Calculator::Evaluate("1.1 + 2.1 + abc"), std::invalid_argument);
}

TEST(Calculator, InvalidQueries) {
  EXPECT_THROW(Calculator::Evaluate("1 + * 2"), std::invalid_argument);
  EXPECT_THROW(Calculator::Evaluate("1 2"), std::invalid_argument);
  EXPECT_THROW(Calculator::Evaluate("1 // 2"), std::invalid_argument);
  EXPECT_THROW(Calculator::Evaluate("1 % 2"), std::invalid_argument);
  EXPECT_THROW(Calculator::Evaluate("1 + -((2 + 3))"), std::invalid_argument);
}

TEST(Calculator, CorrectQueries) {
  EXPECT_EQ(Calculator::Evaluate("2+3"), 5);
  EXPECT_EQ(Calculator::Evaluate("7*8 - 120/6 + 4"), 40);
  EXPECT_EQ(Calculator::Evaluate("10/(6-2)+8"), 10.5);
}