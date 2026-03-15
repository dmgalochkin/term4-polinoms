#include <gtest/gtest.h>
#include "TMonom.hpp"

TEST(TMonomTest, DefaultConstructor)
{
  TMonom m;
  EXPECT_TRUE(m.IsZero());
  EXPECT_EQ(m.GetCoef(), 0.0);
  EXPECT_EQ(m.GetDim(), 0);
}

TEST(TMonomTest, ParameterizedConstructor)
{
  std::vector<double> powers = {1.0, 2.0, 0.5};
  TMonom m(3.5, powers);
  
  EXPECT_EQ(m.GetCoef(), 3.5);
  EXPECT_EQ(m.GetDim(), 3);
  EXPECT_EQ(m.GetPowers()[0], 1.0);
  EXPECT_EQ(m.GetPowers()[1], 2.0);
  EXPECT_EQ(m.GetPowers()[2], 0.5);
  EXPECT_FALSE(m.IsZero());
}

TEST(TMonomTest, MoveConstructor)
{
  std::vector<double> powers = {2.0, 1.5};
  TMonom m(2.0, std::move(powers));
  
  EXPECT_EQ(m.GetCoef(), 2.0);
  EXPECT_EQ(m.GetDim(), 2);
  EXPECT_EQ(m.GetPowers()[0], 2.0);
  EXPECT_EQ(m.GetPowers()[1], 1.5);
}

TEST(TMonomTest, CopyConstructor)
{
  TMonom original(4.0, {1.0, 2.0});
  TMonom copy(original);
  
  EXPECT_EQ(copy.GetCoef(), 4.0);
  EXPECT_EQ(copy.GetDim(), 2);
  EXPECT_EQ(copy.GetPowers()[0], 1.0);
  EXPECT_EQ(copy.GetPowers()[1], 2.0);
}

TEST(TMonomTest, AssignmentOperator)
{
  TMonom original(3.0, {2.0, 1.0});
  TMonom assigned;
  assigned = original;
  
  EXPECT_EQ(assigned.GetCoef(), 3.0);
  EXPECT_EQ(assigned.GetDim(), 2);
  EXPECT_EQ(assigned.GetPowers()[0], 2.0);
  EXPECT_EQ(assigned.GetPowers()[1], 1.0);
}

TEST(TMonomTest, IsZero)
{
  TMonom zero(0.0, {1.0, 2.0});
  TMonom nonZero(1.0, {1.0, 2.0});
  TMonom verySmall(1e-15, {1.0, 2.0});
  
  EXPECT_TRUE(zero.IsZero());
  EXPECT_FALSE(nonZero.IsZero());
  EXPECT_TRUE(verySmall.IsZero()); // Should be considered zero due to EPS
}

TEST(TMonomTest, EqualityOperator)
{
  TMonom m1(2.0, {1.0, 2.0});
  TMonom m2(3.0, {1.0, 2.0}); // Same powers, different coef
  TMonom m3(2.0, {1.0, 2.0}); // Same powers, same coef
  TMonom m4(2.0, {1.0, 3.0}); // Different powers
  
  EXPECT_TRUE(m1 == m2);  // Equality checks only powers
  EXPECT_TRUE(m1 == m3);
  EXPECT_FALSE(m1 == m4);
  EXPECT_TRUE(m1 != m4);
}

TEST(TMonomTest, LessThanOperator)
{
  TMonom m1(1.0, {1.0, 2.0});
  TMonom m2(1.0, {2.0, 1.0});
  TMonom m3(1.0, {1.0, 2.0, 0.0});
  TMonom m4(1.0, {1.0});
  
  EXPECT_TRUE(m1 < m2);   // 1.0 < 2.0 in first position
  EXPECT_TRUE(m4 < m1);   // Smaller dimension
  EXPECT_TRUE(m1 < m3);   // m1 has smaller dimension (2 < 3)
}

TEST(TMonomTest, MultiplicationOperator)
{
  TMonom m1(2.0, {1.0, 2.0});
  TMonom m2(3.0, {2.0, 1.0});
  
  TMonom result = m1 * m2;
  
  EXPECT_EQ(result.GetCoef(), 6.0);
  EXPECT_EQ(result.GetPowers()[0], 3.0); // 1.0 + 2.0
  EXPECT_EQ(result.GetPowers()[1], 3.0); // 2.0 + 1.0
}

TEST(TMonomTest, MultiplicationDimensionMismatch)
{
  TMonom m1(2.0, {1.0, 2.0});
  TMonom m2(3.0, {2.0}); // Different dimension
  
  EXPECT_THROW(m1 * m2, std::invalid_argument);
}

TEST(TMonomTest, ScalarMultiplication)
{
  TMonom m(2.0, {1.0, 2.0});
  TMonom result1 = m * 3.0;
  TMonom result2 = 3.0 * m;
  
  EXPECT_EQ(result1.GetCoef(), 6.0);
  EXPECT_EQ(result2.GetCoef(), 6.0);
  EXPECT_EQ(result1.GetPowers()[0], 1.0);
  EXPECT_EQ(result1.GetPowers()[1], 2.0);
}

TEST(TMonomTest, Differentiate)
{
  TMonom m(6.0, {2.0, 3.0});
  
  // Differentiate by x0
  TMonom d0 = m.Differentiate(0);
  EXPECT_EQ(d0.GetCoef(), 12.0); // 6.0 * 2.0
  EXPECT_EQ(d0.GetPowers()[0], 1.0); // 2.0 - 1.0
  EXPECT_EQ(d0.GetPowers()[1], 3.0); // unchanged
  
  // Differentiate by x1
  TMonom d1 = m.Differentiate(1);
  EXPECT_EQ(d1.GetCoef(), 18.0); // 6.0 * 3.0
  EXPECT_EQ(d1.GetPowers()[0], 2.0); // unchanged
  EXPECT_EQ(d1.GetPowers()[1], 2.0); // 3.0 - 1.0
}

TEST(TMonomTest, DifferentiateConstant)
{
  TMonom m(5.0, {0.0, 2.0}); // Constant in x0
  
  TMonom d0 = m.Differentiate(0);
  EXPECT_TRUE(d0.IsZero()); // Derivative of constant is zero
}

TEST(TMonomTest, DifferentiateInvalidIndex)
{
  TMonom m(2.0, {1.0, 2.0});
  
  EXPECT_THROW(m.Differentiate(-1), std::out_of_range);
  EXPECT_THROW(m.Differentiate(2), std::out_of_range);
}

TEST(TMonomTest, Integrate)
{
  TMonom m(6.0, {2.0, 3.0});
  
  // Integrate by x0
  TMonom i0 = m.Integrate(0);
  EXPECT_EQ(i0.GetCoef(), 2.0); // 6.0 / (2.0 + 1.0)
  EXPECT_EQ(i0.GetPowers()[0], 3.0); // 2.0 + 1.0
  EXPECT_EQ(i0.GetPowers()[1], 3.0); // unchanged
  
  // Integrate by x1
  TMonom i1 = m.Integrate(1);
  EXPECT_EQ(i1.GetCoef(), 1.5); // 6.0 / (3.0 + 1.0)
  EXPECT_EQ(i1.GetPowers()[0], 2.0); // unchanged
  EXPECT_EQ(i1.GetPowers()[1], 4.0); // 3.0 + 1.0
}

TEST(TMonomTest, IntegrateInversePower)
{
  TMonom m(1.0, {-1.0, 2.0}); // 1/x * x^2
  
  EXPECT_THROW(m.Integrate(0), std::invalid_argument); // Cannot integrate 1/x
}

TEST(TMonomTest, IntegrateInvalidIndex)
{
  TMonom m(2.0, {1.0, 2.0});
  
  EXPECT_THROW(m.Integrate(-1), std::out_of_range);
  EXPECT_THROW(m.Integrate(2), std::out_of_range);
}

TEST(TMonomTest, SettersAndGetters)
{
  TMonom m;
  
  m.SetCoef(5.0);
  EXPECT_EQ(m.GetCoef(), 5.0);
  
  std::vector<double> powers = {1.0, 2.0, 3.0};
  m.SetPowers(powers);
  EXPECT_EQ(m.GetDim(), 3);
  EXPECT_EQ(m.GetPowers()[0], 1.0);
  EXPECT_EQ(m.GetPowers()[1], 2.0);
  EXPECT_EQ(m.GetPowers()[2], 3.0);
}

TEST(TMonomTest, PrintOutput)
{
  TMonom zero;
  TMonom constant(5.0, {0.0, 0.0});
  TMonom simple(1.0, {1.0, 0.0});
  TMonom complex(2.5, {0.5, 1.3});
  TMonom negative(-3.0, {2.0, 0.0});
  
  std::ostringstream oss;
  
  // Test zero monomial
  oss.str("");
  zero.Print(oss);
  EXPECT_EQ(oss.str(), "0");
  
  // Test constant
  oss.str("");
  constant.Print(oss);
  EXPECT_EQ(oss.str(), "5");
  
  // Test simple variable
  oss.str("");
  simple.Print(oss);
  EXPECT_EQ(oss.str(), "x0");
  
  // Test complex monomial
  oss.str("");
  complex.Print(oss);
  EXPECT_NE(oss.str().find("2.5"), std::string::npos);
  EXPECT_NE(oss.str().find("x0"), std::string::npos);
  EXPECT_NE(oss.str().find("x1"), std::string::npos);
}