#include <gtest/gtest.h>
#include "TPolinom.hpp"
#include "TMonom.hpp"

TEST(TPolinomTest, DefaultConstructor)
{
  TPolinom p;
  EXPECT_EQ(p.GetDim(), 0);
  EXPECT_EQ(p.GetMonomCount(), 0);
  EXPECT_TRUE(p.IsZero());
}

TEST(TPolinomTest, DimensionConstructor)
{
  TPolinom p(3);
  EXPECT_EQ(p.GetDim(), 3);
  EXPECT_EQ(p.GetMonomCount(), 0);
  EXPECT_TRUE(p.IsZero());
}

TEST(TPolinomTest, MonomConstructor)
{
  TMonom m(2.5, {1.0, 2.0});
  TPolinom p(m);
  EXPECT_EQ(p.GetDim(), 2);
  EXPECT_EQ(p.GetMonomCount(), 1);
  EXPECT_FALSE(p.IsZero());
}

TEST(TPolinomTest, ZeroMonomConstructor)
{
  TMonom zero(0.0, {1.0, 2.0});
  TPolinom p(zero);
  EXPECT_EQ(p.GetDim(), 2);
  EXPECT_EQ(p.GetMonomCount(), 0);
  EXPECT_TRUE(p.IsZero());
}

TEST(TPolinomTest, VectorConstructor)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  TMonom m3(-1.0, {2.0, 0.0});
  std::vector<TMonom> monoms = {m1, m2, m3};
  TPolinom p(monoms);
  EXPECT_EQ(p.GetDim(), 2);
  EXPECT_EQ(p.GetMonomCount(), 3);
  EXPECT_FALSE(p.IsZero());
}

TEST(TPolinomTest, VectorConstructorWithLikeTerms)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {1.0, 0.0});
  std::vector<TMonom> monoms = {m1, m2};
  TPolinom p(monoms);
  EXPECT_EQ(p.GetDim(), 2);
  EXPECT_EQ(p.GetMonomCount(), 1);
  EXPECT_EQ(p.GetMonoms()[0].GetCoef(), 5.0);
}

TEST(TPolinomTest, VectorConstructorDimensionMismatch)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {1.0});
  std::vector<TMonom> monoms = {m1, m2};
  EXPECT_THROW({TPolinom p(monoms);}, std::invalid_argument);
}

TEST(TPolinomTest, CopyConstructor)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  std::vector<TMonom> monoms = {m1, m2};
  TPolinom original(monoms);
  TPolinom copy(original);
  EXPECT_EQ(copy.GetDim(), 2);
  EXPECT_EQ(copy.GetMonomCount(), 2);
  EXPECT_TRUE(copy == original);
}

TEST(TPolinomTest, MoveConstructor)
{
  TMonom m(2.0, {1.0, 0.0});
  TPolinom original(m);
  TPolinom moved(std::move(original));
  EXPECT_EQ(moved.GetDim(), 2);
  EXPECT_EQ(moved.GetMonomCount(), 1);
}

TEST(TPolinomTest, AssignmentOperator)
{
  TMonom m1(2.0, {1.0, 0.0});
  TPolinom original(m1);
  TPolinom assigned;
  assigned = original;
  EXPECT_EQ(assigned.GetDim(), 2);
  EXPECT_EQ(assigned.GetMonomCount(), 1);
  EXPECT_TRUE(assigned == original);
}

TEST(TPolinomTest, AdditionOperator)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  TPolinom p1(m1);
  TPolinom p2(m2);
  TPolinom result = p1 + p2;
  EXPECT_EQ(result.GetDim(), 2);
  EXPECT_EQ(result.GetMonomCount(), 2);
}

TEST(TPolinomTest, AdditionWithLikeTerms)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {1.0, 0.0});
  TPolinom p1(m1);
  TPolinom p2(m2);
  TPolinom result = p1 + p2;
  EXPECT_EQ(result.GetDim(), 2);
  EXPECT_EQ(result.GetMonomCount(), 1);
  EXPECT_EQ(result.GetMonoms()[0].GetCoef(), 5.0);
}

TEST(TPolinomTest, AdditionDimensionMismatch)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {1.0});
  TPolinom p1(m1);
  TPolinom p2(m2);
  EXPECT_THROW(p1 + p2, std::invalid_argument);
}

TEST(TPolinomTest, SubtractionOperator)
{
  TMonom m1(5.0, {1.0, 0.0});
  TMonom m2(2.0, {1.0, 0.0});
  TPolinom p1(m1);
  TPolinom p2(m2);
  TPolinom result = p1 - p2;
  EXPECT_EQ(result.GetDim(), 2);
  EXPECT_EQ(result.GetMonomCount(), 1);
  EXPECT_EQ(result.GetMonoms()[0].GetCoef(), 3.0);
}

TEST(TPolinomTest, MultiplicationOperator)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  TPolinom p1(m1);
  TPolinom p2(m2);
  TPolinom result = p1 * p2;
  EXPECT_EQ(result.GetDim(), 2);
  EXPECT_EQ(result.GetMonomCount(), 1);
  EXPECT_EQ(result.GetMonoms()[0].GetCoef(), 6.0);
  EXPECT_EQ(result.GetMonoms()[0].GetPowers()[0], 1.0);
  EXPECT_EQ(result.GetMonoms()[0].GetPowers()[1], 1.0);
}

TEST(TPolinomTest, DivisionByZero)
{
    TMonom m1(2.0, {1.0});
    TPolinom dividend(m1);
    TMonom m2(0.0, {0.0});
    TPolinom divisorZero;
    divisorZero.AddConstant(0.0);
    EXPECT_THROW(dividend / divisorZero, std::invalid_argument);
    EXPECT_THROW(dividend /= divisorZero, std::invalid_argument);
}

TEST(TPolinomTest, DivisionDimensionMismatch)
{
    TMonom m1(2.0, {1.0});
    TMonom m2(2.0, {1.0, 0.0});
    TPolinom p1(m1);
    TPolinom p2(m2);
    EXPECT_THROW(p1 / p2, std::invalid_argument);
    EXPECT_THROW(p2 / p1, std::invalid_argument);
}

TEST(TPolinomTest, DivisionByConstant)
{
    TMonom m1(6.0, {2.0});  // 6*x^2
    TMonom m2(3.0, {0.0});  // 3
    TPolinom dividend(m1);
    TPolinom divisor(m2);
    TPolinom quotient = dividend / divisor;
    EXPECT_EQ(quotient.GetDim(), 1);
    EXPECT_EQ(quotient.GetMonomCount(), 1);
    const auto& monom = quotient.GetMonoms()[0];
    EXPECT_NEAR(monom.GetCoef(), 2.0, EPS);
    EXPECT_NEAR(monom.GetPowers()[0], 2.0, EPS);
}

TEST(TPolinomTest, DivisionExact)
{
    // (x^2 - 1) / (x - 1) = x + 1
    TMonom m1(1.0, {2.0});
    TMonom m2(-1.0, {0.0});
    TPolinom dividend(std::vector<TMonom>{m1, m2});
    TMonom m3(1.0, {1.0});
    TMonom m4(-1.0, {0.0});
    TPolinom divisor(std::vector<TMonom>{m3, m4});
    TPolinom quotient = dividend / divisor;
    EXPECT_EQ(quotient.GetMonomCount(), 2);
    bool foundX = false, foundConst = false;
    for (const auto& mon : quotient.GetMonoms()) {
        if (mon.GetPowers()[0] == 1.0 && std::abs(mon.GetCoef() - 1.0) < EPS) foundX = true;
        if (mon.GetPowers()[0] == 0.0 && std::abs(mon.GetCoef() - 1.0) < EPS) foundConst = true;
    }
    EXPECT_TRUE(foundX);
    EXPECT_TRUE(foundConst);
    TPolinom remainder = dividend - divisor * quotient;
    EXPECT_TRUE(remainder.IsZero());
}

TEST(TPolinomTest, DivisionWithRemainder)
{
    // (x^2 + 1) / (x - 1) = x + 1, остаток 2
    TMonom m1(1.0, {2.0});
    TMonom m2(1.0, {0.0});
    TPolinom dividend(std::vector<TMonom>{m1, m2});
    TMonom m3(1.0, {1.0});
    TMonom m4(-1.0, {0.0});
    TPolinom divisor(std::vector<TMonom>{m3, m4});
    TPolinom quotient = dividend / divisor;
    EXPECT_EQ(quotient.GetMonomCount(), 2);
    bool foundX = false, foundConst = false;
    for (const auto& mon : quotient.GetMonoms()) {
        if (mon.GetPowers()[0] == 1.0 && std::abs(mon.GetCoef() - 1.0) < EPS) foundX = true;
        if (mon.GetPowers()[0] == 0.0 && std::abs(mon.GetCoef() - 1.0) < EPS) foundConst = true;
    }
    EXPECT_TRUE(foundX);
    EXPECT_TRUE(foundConst);
    TPolinom remainder = dividend - divisor * quotient;
    EXPECT_EQ(remainder.GetMonomCount(), 1);
    EXPECT_NEAR(remainder.GetMonoms()[0].GetCoef(), 2.0, EPS);
    EXPECT_NEAR(remainder.GetMonoms()[0].GetPowers()[0], 0.0, EPS);
}

TEST(TPolinomTest, ScalarMultiplication)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  std::vector<TMonom> monoms = {m1, m2};
  TPolinom p(monoms);
  TPolinom result1 = p * 2.0;
  TPolinom result2 = 2.0 * p;
  EXPECT_EQ(result1.GetMonomCount(), 2);
  bool found_4 = false, found_6 = false;
  for (const auto& m : result1.GetMonoms()) {
    if (std::abs(m.GetCoef() - 4.0) < EPS) found_4 = true;
    if (std::abs(m.GetCoef() - 6.0) < EPS) found_6 = true;
  }
  EXPECT_TRUE(found_4);
  EXPECT_TRUE(found_6);
  EXPECT_TRUE(result1 == result2);
}

TEST(TPolinomTest, ScalarMultiplicationByZero)
{
  TMonom m(2.0, {1.0, 0.0});
  TPolinom p(m);
  TPolinom result = p * 0.0;
  EXPECT_TRUE(result.IsZero());
  EXPECT_EQ(result.GetMonomCount(), 0);
}

TEST(TPolinomTest, UnaryMinus)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(-3.0, {0.0, 1.0});
  std::vector<TMonom> monoms = {m1, m2};
  TPolinom p(monoms);
  TPolinom result = -p;
  EXPECT_EQ(result.GetMonomCount(), 2);
  bool found_neg2 = false, found_pos3 = false;
  for (const auto& m : result.GetMonoms()) {
    if (std::abs(m.GetCoef() - (-2.0)) < EPS) found_neg2 = true;
    if (std::abs(m.GetCoef() - 3.0) < EPS) found_pos3 = true;
  }
  EXPECT_TRUE(found_neg2);
  EXPECT_TRUE(found_pos3);
}

TEST(TPolinomTest, EqualityOperator)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  std::vector<TMonom> monoms12 = {m1, m2};
  std::vector<TMonom> monoms1 = {m1};
  TPolinom p1(monoms12);
  TPolinom p2(monoms12);
  TPolinom p3(monoms1);
  EXPECT_TRUE(p1 == p2);
  EXPECT_FALSE(p1 == p3);
  EXPECT_FALSE(p1 != p2);
  EXPECT_TRUE(p1 != p3);
}

TEST(TPolinomTest, CompoundAssignmentOperators)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  TPolinom p1(m1);
  TPolinom p2(m2);
  p1 += p2;
  EXPECT_EQ(p1.GetMonomCount(), 2);
  p1 -= p2;
  EXPECT_EQ(p1.GetMonomCount(), 1);
  EXPECT_EQ(p1.GetMonoms()[0].GetCoef(), 2.0);
  p1 *= 3.0;
  EXPECT_EQ(p1.GetMonoms()[0].GetCoef(), 6.0);
}

TEST(TPolinomTest, Differentiate)
{
  TMonom m1(6.0, {2.0, 0.0});
  TMonom m2(4.0, {1.0, 1.0});
  std::vector<TMonom> monoms = {m1, m2};
  TPolinom p(monoms);
  TPolinom dx = p.Differentiate(0);
  EXPECT_EQ(dx.GetMonomCount(), 2);
}

TEST(TPolinomTest, DifferentiateInvalidIndex)
{
  TMonom m(2.0, {1.0, 0.0});
  TPolinom p(m);
  EXPECT_THROW(p.Differentiate(-1), std::out_of_range);
  EXPECT_THROW(p.Differentiate(2), std::out_of_range);
}

TEST(TPolinomTest, Integrate)
{
  TMonom m1(6.0, {2.0, 0.0});
  TMonom m2(4.0, {1.0, 1.0});
  std::vector<TMonom> monoms = {m1, m2};
  TPolinom p(monoms);
  TPolinom intx = p.Integrate(0);
  EXPECT_EQ(intx.GetMonomCount(), 2);
}

TEST(TPolinomTest, IntegrateInvalidIndex)
{
  TMonom m(2.0, {1.0, 0.0});
  TPolinom p(m);
  EXPECT_THROW(p.Integrate(-1), std::out_of_range);
  EXPECT_THROW(p.Integrate(2), std::out_of_range);
}

TEST(TPolinomTest, AddConstant)
{
  TMonom m(2.0, {1.0, 0.0});
  TPolinom p(m);
  p.AddConstant(5.0);
  EXPECT_EQ(p.GetMonomCount(), 2);
}

TEST(TPolinomTest, AddMonom)
{
  TPolinom p(2);
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  p.AddMonom(m1);
  EXPECT_EQ(p.GetMonomCount(), 1);
  p.AddMonom(m2);
  EXPECT_EQ(p.GetMonomCount(), 2);
}

TEST(TPolinomTest, AddMonomDimensionMismatch)
{
  TPolinom p(2);
  TMonom m(2.0, {1.0});
  EXPECT_THROW(p.AddMonom(m), std::invalid_argument);
}

TEST(TPolinomTest, Evaluate)
{
  TMonom m1(2.0, {2.0, 0.0});
  TMonom m2(3.0, {0.0, 1.0});
  TMonom m3(1.0, {0.0, 0.0});
  std::vector<TMonom> monoms = {m1, m2, m3};
  TPolinom p(monoms);
  std::vector<double> point = {2.0, 3.0};
  double result = p.Evaluate(point);
  EXPECT_EQ(result, 18.0);
}

TEST(TPolinomTest, EvaluateDimensionMismatch)
{
  TMonom m(2.0, {1.0, 0.0});
  TPolinom p(m);
  std::vector<double> point = {2.0};
  EXPECT_THROW(p.Evaluate(point), std::invalid_argument);
}

TEST(TPolinomTest, SaveAndLoadFile)
{
  TMonom m1(2.5, {0.5, 1.3});
  TMonom m2(-3.0, {2.0, 0.0});
  std::vector<TMonom> monoms = {m1, m2};
  TPolinom original(monoms);
  original.SaveToFile("test_polynomial.txt");
  TPolinom loaded;
  loaded.LoadFromFile("test_polynomial.txt");
  EXPECT_TRUE(original == loaded);
}

TEST(TPolinomTest, NormalizationRemovesZeroTerms)
{
  TMonom m1(2.0, {1.0, 0.0});
  TMonom m2(-2.0, {1.0, 0.0});
  TMonom m3(3.0, {0.0, 1.0});
  std::vector<TMonom> monoms = {m1, m2, m3};
  TPolinom p(monoms);
  EXPECT_EQ(p.GetMonomCount(), 1);
  EXPECT_EQ(p.GetMonoms()[0].GetCoef(), 3.0);
}

TEST(TPolinomTest, EmptyPolynomialOperations)
{
  TPolinom empty1;
  TPolinom empty2;
  EXPECT_TRUE((empty1 + empty2).IsZero());
  EXPECT_TRUE((empty1 - empty2).IsZero());
  EXPECT_TRUE((empty1 * empty2).IsZero());
  EXPECT_TRUE((empty1 * 5.0).IsZero());
  TPolinom empty3(2);
  TPolinom empty4(2);
  TMonom m(2.0, {1.0, 0.0});
  TPolinom nonEmpty(m);
  EXPECT_TRUE((empty3 + empty4).IsZero());
  TPolinom result = empty3 + nonEmpty;
  EXPECT_TRUE(result == nonEmpty);
}
