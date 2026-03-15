#pragma once
#include "TPolinom.hpp"
#include <string>
#include <vector>

// Базовый интерфейс для всех типов таблиц
template<typename T>
class TTable
{
public:
  virtual ~TTable() = default;
  
  // Основные операции
  virtual void Insert(const std::string& key, const T& value) = 0;
  virtual bool Remove(const std::string& key) = 0;
  virtual T* Find(const std::string& key) = 0;
  virtual const T* Find(const std::string& key) const = 0;
  
  // Информационные методы
  virtual size_t GetSize() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual void Clear() = 0;
  
  // Получение всех ключей и значений
  virtual std::vector<std::string> GetKeys() const = 0;
  virtual std::vector<std::pair<std::string, T>> GetAll() const = 0;
  
  // Проверка существования ключа
  virtual bool Contains(const std::string& key) const = 0;
};

// Специализация для полиномов
using TPolinomTable = TTable<TPolinom>;