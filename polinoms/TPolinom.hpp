#pragma once
#include "TMonom.hpp"
#include <vector>
#include <iostream>
#include <string>

// Класс полинома (сумма мономов)
class TPolinom
{
private:
  std::vector<TMonom> monoms;   // список мономов
  size_t dim;                   // размерность пространства (количество переменных)

  // Приведение подобных, удаление нулевых мономов, сортировка
  void Normalize();

public:
  // Конструкторы
  TPolinom();
  explicit TPolinom(size_t d);
  TPolinom(const TMonom& m);
  TPolinom(const std::vector<TMonom>& monoms);
  TPolinom(const TPolinom& other);
  TPolinom(TPolinom&& other);
  
  // Деструктор
  ~TPolinom();
  
  // Операторы присваивания
  TPolinom& operator=(const TPolinom& other);
  TPolinom& operator=(TPolinom&& other);

  // Получение размерности
  size_t GetDim() const;
  size_t GetMonomCount() const;
  const std::vector<TMonom>& GetMonoms() const;

  // Арифметические операции (возвращают новый полином)
  TPolinom operator+(const TPolinom& other) const;
  TPolinom operator-(const TPolinom& other) const;
  TPolinom operator*(const TPolinom& other) const;

  // Унарный минус
  TPolinom operator-() const;

  // Операторы сравнения
  bool operator==(const TPolinom& other) const;
  bool operator!=(const TPolinom& other) const;

  // Умножение на скаляр
  TPolinom operator*(double scalar) const;
  TPolinom& operator*=(double scalar);

  // Дружественная функция для умножения скаляра на полином (коммутативность)
  friend TPolinom operator*(double scalar, const TPolinom& p);

  // Операторы +=, -=
  TPolinom& operator+=(const TPolinom& other);
  TPolinom& operator-=(const TPolinom& other);
  TPolinom& operator*=(const TPolinom& other);

  // Дифференцирование по переменной varIdx
  TPolinom Differentiate(int varIdx) const;

  // Интегрирование по переменной varIdx (без добавления константы)
  TPolinom Integrate(int varIdx) const;

  // Добавление константы (полезно после интегрирования)
  void AddConstant(double c);
  
  // Добавление монома
  void AddMonom(const TMonom& monom);

  // Проверка на нулевой полином
  bool IsZero() const;

  // Вычисление значения полинома в точке
  double Evaluate(const std::vector<double>& point) const;

  // Вывод полинома в поток
  void Print(std::ostream& os) const;

  // Операторы ввода/вывода
  friend std::ostream& operator<<(std::ostream& os, const TPolinom& p);
  friend std::istream& operator>>(std::istream& is, TPolinom& p);
  
  // Сохранение/загрузка в файл
  void SaveToFile(const std::string& filename) const;
  void LoadFromFile(const std::string& filename);
};