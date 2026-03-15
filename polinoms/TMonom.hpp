#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>

const double EPS = 1e-10;

class TMonom
{
private:
  double coef;
  std::vector<double> powers;

public:
  TMonom();
  TMonom(double c, const std::vector<double>& p);
  TMonom(double c, std::vector<double>&& p);
  TMonom(const TMonom& other);
  TMonom(TMonom&& other);
  
  ~TMonom();
  
  TMonom& operator=(const TMonom& other);
  TMonom& operator=(TMonom&& other);

  double GetCoef() const;
  const std::vector<double>& GetPowers() const;
  size_t GetDim() const;
  
  void SetCoef(double c);
  void SetPowers(const std::vector<double>& p);

  bool IsZero() const;

  bool operator<(const TMonom& other) const;

  bool operator==(const TMonom& other) const;
  bool operator!=(const TMonom& other) const;

  TMonom Differentiate(int varIdx) const;

  TMonom Integrate(int varIdx) const;

  TMonom operator*(const TMonom& other) const;
  
  TMonom operator*(double scalar) const;
  friend TMonom operator*(double scalar, const TMonom& monom);

  void Print(std::ostream& os) const;
  
  friend std::ostream& operator<<(std::ostream& os, const TMonom& monom);
  friend std::istream& operator>>(std::istream& is, TMonom& monom);
};