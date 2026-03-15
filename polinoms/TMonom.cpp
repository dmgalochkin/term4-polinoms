#include "TMonom.hpp"
#include <algorithm>
#include <sstream>

// Конструкторы
TMonom::TMonom() : coef(0.0) {}

TMonom::TMonom(double c, const std::vector<double>& p)
  : coef(c), powers(p) {}

TMonom::TMonom(double c, std::vector<double>&& p)
  : coef(c), powers(std::move(p)) {}

TMonom::TMonom(const TMonom& other)
  : coef(other.coef), powers(other.powers) {}

TMonom::TMonom(TMonom&& other)
  : coef(other.coef), powers(std::move(other.powers)) {}

// Деструктор
TMonom::~TMonom() {}

// Операторы присваивания
TMonom& TMonom::operator=(const TMonom& other)
{
  if (this != &other)
  {
    coef = other.coef;
    powers = other.powers;
  }
  return *this;
}

TMonom& TMonom::operator=(TMonom&& other)
{
  if (this != &other)
  {
    coef = other.coef;
    powers = std::move(other.powers);
  }
  return *this;
}

// Геттеры
double TMonom::GetCoef() const
{
  return coef;
}

const std::vector<double>& TMonom::GetPowers() const
{
  return powers;
}

size_t TMonom::GetDim() const
{
  return powers.size();
}

// Сеттеры
void TMonom::SetCoef(double c)
{
  coef = c;
}

void TMonom::SetPowers(const std::vector<double>& p)
{
  powers = p;
}

// Проверка на нулевой коэффициент
bool TMonom::IsZero() const
{
  return std::abs(coef) < EPS;
}

// Сравнение мономов по степеням (лексикографически)
bool TMonom::operator<(const TMonom& other) const
{
  size_t n = powers.size();
  size_t m = other.powers.size();
  if (n != m)
  {
    return n < m;   // разные размерности – считаем меньшим тот, у кого меньше переменных
  }
  for (size_t i = 0; i < n; ++i)
  {
    if (std::abs(powers[i] - other.powers[i]) > EPS)
    {
      return powers[i] < other.powers[i];
    }
  }
  return false;   // равны
}

// Проверка на равенство степеней
bool TMonom::operator==(const TMonom& other) const
{
  if (powers.size() != other.powers.size())
  {
    return false;
  }
  for (size_t i = 0; i < powers.size(); ++i)
  {
    if (std::abs(powers[i] - other.powers[i]) > EPS)
    {
      return false;
    }
  }
  return true;
}

bool TMonom::operator!=(const TMonom& other) const
{
  return !(*this == other);
}

// Дифференцирование по переменной с индексом varIdx
TMonom TMonom::Differentiate(int varIdx) const
{
  if (varIdx < 0 || varIdx >= static_cast<int>(powers.size()))
  {
    throw std::out_of_range("Invalid variable index for differentiation");
  }
  double s = powers[varIdx];
  if (std::abs(s) < EPS)
  {
    // Производная константы (по этой переменной) равна нулю
    return TMonom(0.0, powers);
  }
  std::vector<double> newPowers = powers;
  newPowers[varIdx] = s - 1.0;
  double newCoef = coef * s;
  return TMonom(newCoef, newPowers);
}

// Интегрирование по переменной varIdx
TMonom TMonom::Integrate(int varIdx) const
{
  if (varIdx < 0 || varIdx >= static_cast<int>(powers.size()))
  {
    throw std::out_of_range("Invalid variable index for integration");
  }
  double s = powers[varIdx];
  if (std::abs(s + 1.0) < EPS)
  {
    throw std::invalid_argument("Cannot integrate 1/x term (power -1) as monomial");
  }
  std::vector<double> newPowers = powers;
  newPowers[varIdx] = s + 1.0;
  double newCoef = coef / (s + 1.0);
  return TMonom(newCoef, newPowers);
}

// Умножение двух мономов
TMonom TMonom::operator*(const TMonom& other) const
{
  if (powers.size() != other.powers.size())
  {
    throw std::invalid_argument("Monomial dimensions mismatch");
  }
  std::vector<double> newPowers(powers.size());
  for (size_t i = 0; i < powers.size(); ++i)
  {
    newPowers[i] = powers[i] + other.powers[i];
  }
  double newCoef = coef * other.coef;
  return TMonom(newCoef, newPowers);
}

// Умножение на скаляр
TMonom TMonom::operator*(double scalar) const
{
  return TMonom(coef * scalar, powers);
}

TMonom operator*(double scalar, const TMonom& monom)
{
  return monom * scalar;
}

// Вспомогательный вывод в поток
void TMonom::Print(std::ostream& os) const
{
  if (IsZero())
  {
    os << "0";
    return;
  }
  
  // Выводим коэффициент
  if (std::abs(coef - 1.0) > EPS || powers.empty())
  {
    if (std::abs(coef + 1.0) < EPS && !powers.empty())
    {
      os << "-";
    }
    else
    {
      os << coef;
    }
  }
  else if (!powers.empty())
  {
    // Коэффициент равен 1, но есть переменные - не выводим коэффициент
  }
  
  // Выводим переменные
  for (size_t i = 0; i < powers.size(); ++i)
  {
    if (std::abs(powers[i]) > EPS)
    {
      if (std::abs(coef - 1.0) > EPS && std::abs(coef + 1.0) > EPS)
      {
        os << "*";
      }
      os << "x" << i;
      if (std::abs(powers[i] - 1.0) > EPS)
      {
        os << "^(" << powers[i] << ")";
      }
    }
  }
}

// Операторы ввода/вывода
std::ostream& operator<<(std::ostream& os, const TMonom& monom)
{
  monom.Print(os);
  return os;
}

std::istream& operator>>(std::istream& is, TMonom& monom)
{
  double coef;
  size_t dim;
  is >> coef >> dim;
  std::vector<double> powers(dim);
  for (size_t i = 0; i < dim; ++i)
  {
    is >> powers[i];
  }
  monom = TMonom(coef, powers);
  return is;
}