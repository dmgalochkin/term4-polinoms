#pragma once
#include "TPolinom.hpp"
#include <string>
#include <vector>

template<typename T>
class TTable
{
public:
  virtual ~TTable() = default;
  
  virtual void Insert(const std::string& key, const T& value) = 0;
  virtual bool Remove(const std::string& key) = 0;
  virtual T* Find(const std::string& key) = 0;
  virtual const T* Find(const std::string& key) const = 0;
  
  virtual size_t GetSize() const = 0;
  virtual bool IsEmpty() const = 0;
  virtual void Clear() = 0;
  
  virtual std::vector<std::string> GetKeys() const = 0;
  virtual std::vector<std::pair<std::string, T>> GetAll() const = 0;
  
  virtual bool Contains(const std::string& key) const = 0;
};

using TPolinomTable = TTable<TPolinom>;