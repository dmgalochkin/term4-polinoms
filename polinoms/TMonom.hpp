#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <stdexcept>

// Порог для сравнения чисел с плавающей точкой
const double EPS = 1e-10;

// Структура, представляющая одночлен (моном) от нескольких переменных
// с дробными показателями степеней.
class TMonom
{
private:
  double coef;                      // коэффициент
  std::vector<double> powers;       // степени по каждой переменной

public:
  // Конструкторы
  TMonom();
  TMonom(double c, const std::vector<double>& p);
  TMonom(double c, std::vector<double>&& p);
  TMonom(const TMonom& other);
  TMonom(TMonom&& other);
  
  // Деструктор
  ~TMonom();
  
  // Операторы присваивания
  TMonom& operator=(const TMonom& other);
  TMonom& operator=(TMonom&& other);

  // Геттеры
  double GetCoef() const;
  const std::vector<double>& GetPowers() const;
  size_t GetDim() const;
  
  // Сеттеры
  void SetCoef(double c);
  void SetPowers(const std::vector<double>& p);

  // Проверка на нулевой коэффициент
  bool IsZero() const;

  // Сравнение мономов по степеням (лексикографически) – для сортировки и приведения подобных
  bool operator<(const TMonom& other) const;

  // Проверка на равенство степеней (с учётом EPS)
  bool operator==(const TMonom& other) const;
  bool operator!=(const TMonom& other) const;

  // Дифференцирование по переменной с индексом varIdx (0 .. n-1)
  // Возвращает новый моном – результат дифференцирования.
  TMonom Differentiate(int varIdx) const;

  // Интегрирование по переменной varIdx (неопределённый интеграл, без константы)
  // Возвращает новый моном. Для степени -1 выбрасывает исключение (интеграл даёт логарифм).
  TMonom Integrate(int varIdx) const;

  // Умножение двух мономов (используется при умножении полиномов)
  TMonom operator*(const TMonom& other) const;
  
  // Умножение на скаляр
  TMonom operator*(double scalar) const;
  friend TMonom operator*(double scalar, const TMonom& monom);

  // Вспомогательный вывод в поток
  void Print(std::ostream& os) const;
  
  // Операторы ввода/вывода
  friend std::ostream& operator<<(std::ostream& os, const TMonom& monom);
  friend std::istream& operator>>(std::istream& is, TMonom& monom);
};