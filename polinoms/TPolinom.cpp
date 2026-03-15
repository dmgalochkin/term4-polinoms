#include "TPolinom.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>

// Конструкторы
TPolinom::TPolinom() : dim(0) {}

TPolinom::TPolinom(size_t d) : dim(d) {}

TPolinom::TPolinom(const TMonom& m) : dim(m.GetDim())
{
  if (!m.IsZero())
  {
    monoms.push_back(m);
  }
}

TPolinom::TPolinom(const std::vector<TMonom>& monomVector)
{
  if (monomVector.size() > 0)
  {
    dim = monomVector[0].GetDim();
    for (const auto& m : monomVector)
    {
      if (m.GetDim() != dim)
      {
        throw std::invalid_argument("Inconsistent monomial dimensions in vector");
      }
      if (!m.IsZero())
      {
        monoms.push_back(m);
      }
    }
    Normalize();
  }
  else
  {
    dim = 0;
  }
}

TPolinom::TPolinom(const TPolinom& other)
  : monoms(other.monoms), dim(other.dim) {}

TPolinom::TPolinom(TPolinom&& other)
  : monoms(std::move(other.monoms)), dim(other.dim) {}

// Деструктор
TPolinom::~TPolinom() {}

// Операторы присваивания
TPolinom& TPolinom::operator=(const TPolinom& other)
{
  if (this != &other)
  {
    monoms = other.monoms;
    dim = other.dim;
  }
  return *this;
}

TPolinom& TPolinom::operator=(TPolinom&& other)
{
  if (this != &other)
  {
    monoms = std::move(other.monoms);
    dim = other.dim;
  }
  return *this;
}

// Получение размерности
size_t TPolinom::GetDim() const
{
  return dim;
}

size_t TPolinom::GetMonomCount() const
{
  return monoms.size();
}

const std::vector<TMonom>& TPolinom::GetMonoms() const
{
  return monoms;
}

// Приведение подобных, удаление нулевых мономов, сортировка
void TPolinom::Normalize()
{
  if (monoms.empty())
  {
    return;
  }
  
  // Сортируем по степеням
  std::sort(monoms.begin(), monoms.end(),
            [](const TMonom& a, const TMonom& b) { return a < b; });
  
  std::vector<TMonom> result;
  result.reserve(monoms.size());
  auto it = monoms.begin();
  
  while (it != monoms.end())
  {
    auto next = it + 1;
    TMonom current = *it;
    
    // Объединяем одинаковые мономы
    while (next != monoms.end() && current == *next)
    {
      current.SetCoef(current.GetCoef() + next->GetCoef());
      ++next;
    }
    
    if (!current.IsZero())
    {
      result.push_back(current);
    }
    it = next;
  }
  
  monoms = std::move(result);
}

// Арифметические операции
TPolinom TPolinom::operator+(const TPolinom& other) const
{
  if (dim != other.dim)
  {
    throw std::invalid_argument("Polynomial dimension mismatch in addition");
  }
  
  TPolinom result = *this;
  result.monoms.insert(result.monoms.end(), other.monoms.begin(), other.monoms.end());
  result.Normalize();
  return result;
}

TPolinom TPolinom::operator-(const TPolinom& other) const
{
  if (dim != other.dim)
  {
    throw std::invalid_argument("Polynomial dimension mismatch in subtraction");
  }
  
  TPolinom result = *this;
  for (const auto& m : other.monoms)
  {
    TMonom neg = m;
    neg.SetCoef(-neg.GetCoef());
    result.monoms.push_back(neg);
  }
  result.Normalize();
  return result;
}

TPolinom TPolinom::operator*(const TPolinom& other) const
{
  if (dim != other.dim)
  {
    throw std::invalid_argument("Polynomial dimension mismatch in multiplication");
  }
  
  TPolinom result(dim);
  for (const auto& m1 : monoms)
  {
    for (const auto& m2 : other.monoms)
    {
      TMonom prod = m1 * m2;
      if (!prod.IsZero())
      {
        result.monoms.push_back(prod);
      }
    }
  }
  result.Normalize();
  return result;
}

// Унарный минус
TPolinom TPolinom::operator-() const
{
  TPolinom result = *this;
  for (auto& m : result.monoms)
  {
    m.SetCoef(-m.GetCoef());
  }
  return result;
}

// Операторы сравнения
bool TPolinom::operator==(const TPolinom& other) const
{
  if (dim != other.dim || monoms.size() != other.monoms.size())
  {
    return false;
  }
  
  // Создаем копии для нормализации
  TPolinom temp1 = *this;
  TPolinom temp2 = other;
  temp1.Normalize();
  temp2.Normalize();
  
  if (temp1.monoms.size() != temp2.monoms.size())
  {
    return false;
  }
  
  for (size_t i = 0; i < temp1.monoms.size(); ++i)
  {
    if (temp1.monoms[i] != temp2.monoms[i] || 
        std::abs(temp1.monoms[i].GetCoef() - temp2.monoms[i].GetCoef()) > EPS)
    {
      return false;
    }
  }
  return true;
}

bool TPolinom::operator!=(const TPolinom& other) const
{
  return !(*this == other);
}

// Умножение на скаляр
TPolinom TPolinom::operator*(double scalar) const
{
  TPolinom result = *this;
  for (auto& m : result.monoms)
  {
    m.SetCoef(m.GetCoef() * scalar);
  }
  if (std::abs(scalar) < EPS)
  {
    result.monoms.clear();   // обнуление
  }
  return result;
}

TPolinom& TPolinom::operator*=(double scalar)
{
  for (auto& m : monoms)
  {
    m.SetCoef(m.GetCoef() * scalar);
  }
  if (std::abs(scalar) < EPS)
  {
    monoms.clear();   // обнуление
  }
  return *this;
}

TPolinom operator*(double scalar, const TPolinom& p)
{
  return p * scalar;
}

// Операторы +=, -=, *=
TPolinom& TPolinom::operator+=(const TPolinom& other)
{
  *this = *this + other;
  return *this;
}

TPolinom& TPolinom::operator-=(const TPolinom& other)
{
  *this = *this - other;
  return *this;
}

TPolinom& TPolinom::operator*=(const TPolinom& other)
{
  *this = *this * other;
  return *this;
}

// Дифференцирование
TPolinom TPolinom::Differentiate(int varIdx) const
{
  if (varIdx < 0 || varIdx >= static_cast<int>(dim))
  {
    throw std::out_of_range("Variable index out of range");
  }
  
  TPolinom result(dim);
  for (const auto& m : monoms)
  {
    TMonom dm = m.Differentiate(varIdx);
    if (!dm.IsZero())
    {
      result.monoms.push_back(dm);
    }
  }
  result.Normalize();
  return result;
}

// Интегрирование
TPolinom TPolinom::Integrate(int varIdx) const
{
  if (varIdx < 0 || varIdx >= static_cast<int>(dim))
  {
    throw std::out_of_range("Variable index out of range");
  }
  
  TPolinom result(dim);
  for (const auto& m : monoms)
  {
    TMonom im = m.Integrate(varIdx);
    if (!im.IsZero())
    {
      result.monoms.push_back(im);
    }
  }
  result.Normalize();
  return result;
}

// Добавление константы
void TPolinom::AddConstant(double c)
{
  if (std::abs(c) < EPS)
  {
    return;
  }
  if (dim == 0)
  {
    // Если размерность не определена, устанавливаем её равной 1 (условно)
    dim = 1;
  }
  std::vector<double> zeroPowers(dim, 0.0);
  TMonom constMonom(c, zeroPowers);
  monoms.push_back(constMonom);
  Normalize();
}

// Добавление монома
void TPolinom::AddMonom(const TMonom& monom)
{
  if (dim == 0)
  {
    dim = monom.GetDim();
  }
  else if (monom.GetDim() != dim)
  {
    throw std::invalid_argument("Monomial dimension mismatch");
  }
  
  if (!monom.IsZero())
  {
    monoms.push_back(monom);
    Normalize();
  }
}

// Проверка на нулевой полином
bool TPolinom::IsZero() const
{
  return monoms.empty();
}

// Вычисление значения полинома в точке
double TPolinom::Evaluate(const std::vector<double>& point) const
{
  if (point.size() != dim)
  {
    throw std::invalid_argument("Point dimension mismatch");
  }
  
  double result = 0.0;
  for (const auto& m : monoms)
  {
    double monomValue = m.GetCoef();
    const auto& powers = m.GetPowers();
    for (size_t i = 0; i < powers.size(); ++i)
    {
      monomValue *= std::pow(point[i], powers[i]);
    }
    result += monomValue;
  }
  return result;
}

// Вывод полинома в поток
void TPolinom::Print(std::ostream& os) const
{
  if (monoms.empty())
  {
    os << "0";
    return;
  }
  
  bool first = true;
  for (const auto& m : monoms)
  {
    if (!first && m.GetCoef() > 0)
    {
      os << " + ";
    }
    else if (!first && m.GetCoef() < 0)
    {
      os << " ";
    }
    m.Print(os);
    first = false;
  }
}

// Операторы ввода/вывода
std::ostream& operator<<(std::ostream& os, const TPolinom& p)
{
  p.Print(os);
  return os;
}

std::istream& operator>>(std::istream& is, TPolinom& p)
{
  size_t dim, monomCount;
  is >> dim >> monomCount;
  
  p = TPolinom(dim);
  for (size_t i = 0; i < monomCount; ++i)
  {
    TMonom m;
    is >> m;
    p.AddMonom(m);
  }
  return is;
}

// Сохранение/загрузка в файл
void TPolinom::SaveToFile(const std::string& filename) const
{
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file for writing: " + filename);
  }
  
  file << dim << " " << monoms.size() << std::endl;
  for (const auto& m : monoms)
  {
    file << m.GetCoef() << " " << m.GetDim();
    for (double power : m.GetPowers())
    {
      file << " " << power;
    }
    file << std::endl;
  }
  file.close();
}

void TPolinom::LoadFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file for reading: " + filename);
  }
  
  size_t monomCount;
  file >> dim >> monomCount;
  
  monoms.clear();
  for (size_t i = 0; i < monomCount; ++i)
  {
    double coef;
    size_t monomDim;
    file >> coef >> monomDim;
    
    std::vector<double> powers(monomDim);
    for (size_t j = 0; j < monomDim; ++j)
    {
      file >> powers[j];
    }
    
    TMonom m(coef, powers);
    if (!m.IsZero())
    {
      monoms.push_back(m);
    }
  }
  
  Normalize();
  file.close();
}