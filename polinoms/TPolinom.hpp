#pragma once
#include "TMonom.hpp"
#include <vector>
#include <iostream>
#include <string>

class TPolinom
{
private:
  std::vector<TMonom> monoms;
  size_t dim;

  void Normalize();

public:
  TPolinom();
  explicit TPolinom(size_t d);
  TPolinom(const TMonom& m);
  TPolinom(const std::vector<TMonom>& monoms);
  TPolinom(const TPolinom& other);
  TPolinom(TPolinom&& other);
  
  ~TPolinom();
  
  TPolinom& operator=(const TPolinom& other);
  TPolinom& operator=(TPolinom&& other);

  size_t GetDim() const;
  size_t GetMonomCount() const;
  const std::vector<TMonom>& GetMonoms() const;

  TPolinom operator+(const TPolinom& other) const;
  TPolinom operator-(const TPolinom& other) const;
  TPolinom operator*(const TPolinom& other) const;

  TPolinom operator-() const;

  bool operator==(const TPolinom& other) const;
  bool operator!=(const TPolinom& other) const;

  TPolinom operator*(double scalar) const;
  TPolinom& operator*=(double scalar);

  friend TPolinom operator*(double scalar, const TPolinom& p);

  TPolinom& operator+=(const TPolinom& other);
  TPolinom& operator-=(const TPolinom& other);
  TPolinom& operator*=(const TPolinom& other);

  TPolinom Differentiate(int varIdx) const;

  TPolinom Integrate(int varIdx) const;

  void AddConstant(double c);
  
  void AddMonom(const TMonom& monom);

  bool IsZero() const;

  double Evaluate(const std::vector<double>& point) const;

  void Print(std::ostream& os) const;

  friend std::ostream& operator<<(std::ostream& os, const TPolinom& p);
  friend std::istream& operator>>(std::istream& is, TPolinom& p);
  
  void SaveToFile(const std::string& filename) const;
  void LoadFromFile(const std::string& filename);
};