#pragma once
#include "TTable.hpp"
#include <vector>
#include <string>
#include <algorithm>

template<typename T>
class TArrayTable : public TTable<T>
{
private:
  struct TableEntry
  {
    std::string key;
    T value;
    bool isDeleted;
    
    TableEntry() : isDeleted(true) {}
    TableEntry(const std::string& k, const T& v) : key(k), value(v), isDeleted(false) {}
  };
  
  std::vector<TableEntry> data;
  size_t size;
  size_t capacity;
  
  int FindIndex(const std::string& key) const;
  void Resize();
  void Compact();

public:
  explicit TArrayTable(size_t initialCapacity = 16);
  ~TArrayTable() override = default;
  
  void Insert(const std::string& key, const T& value) override;
  bool Remove(const std::string& key) override;
  T* Find(const std::string& key) override;
  const T* Find(const std::string& key) const override;
  
  size_t GetSize() const override;
  bool IsEmpty() const override;
  void Clear() override;
  
  std::vector<std::string> GetKeys() const override;
  std::vector<std::pair<std::string, T>> GetAll() const override;
  
  bool Contains(const std::string& key) const override;
  
  size_t GetCapacity() const;
  double GetLoadFactor() const;
};


template<typename T>
TArrayTable<T>::TArrayTable(size_t initialCapacity) 
  : size(0), capacity(initialCapacity)
{
  data.resize(capacity);
}

template<typename T>
int TArrayTable<T>::FindIndex(const std::string& key) const
{
  for (size_t i = 0; i < capacity; ++i)
  {
    if (!data[i].isDeleted && data[i].key == key)
    {
      return static_cast<int>(i);
    }
  }
  return -1;
}

template<typename T>
void TArrayTable<T>::Resize()
{
  if (size >= capacity * 0.75)
  {
    size_t oldCapacity = capacity;
    capacity *= 2;
    std::vector<TableEntry> oldData = std::move(data);
    data.clear();
    data.resize(capacity);
    size = 0;
    
    for (size_t i = 0; i < oldCapacity; ++i)
    {
      if (!oldData[i].isDeleted)
      {
        Insert(oldData[i].key, oldData[i].value);
      }
    }
  }
}

template<typename T>
void TArrayTable<T>::Compact()
{
  std::vector<TableEntry> newData;
  newData.reserve(capacity);
  
  for (size_t i = 0; i < capacity; ++i)
  {
    if (!data[i].isDeleted)
    {
      newData.push_back(data[i]);
    }
  }
  
  data = std::move(newData);
  data.resize(capacity);
  
  for (size_t i = size; i < capacity; ++i)
  {
    data[i] = TableEntry();
  }
}

template<typename T>
void TArrayTable<T>::Insert(const std::string& key, const T& value)
{
  int existingIndex = FindIndex(key);
  if (existingIndex != -1)
  {
    data[existingIndex].value = value;
    return;
  }
  
  Resize();
  
  for (size_t i = 0; i < capacity; ++i)
  {
    if (data[i].isDeleted)
    {
      data[i] = TableEntry(key, value);
      ++size;
      return;
    }
  }
  
  throw std::runtime_error("Unable to insert element: table is full");
}

template<typename T>
bool TArrayTable<T>::Remove(const std::string& key)
{
  int index = FindIndex(key);
  if (index != -1)
  {
    data[index].isDeleted = true;
    --size;
    
    if (size < capacity * 0.25 && capacity > 16)
    {
      Compact();
    }
    
    return true;
  }
  return false;
}

template<typename T>
T* TArrayTable<T>::Find(const std::string& key)
{
  int index = FindIndex(key);
  return (index != -1) ? &data[index].value : nullptr;
}

template<typename T>
const T* TArrayTable<T>::Find(const std::string& key) const
{
  int index = FindIndex(key);
  return (index != -1) ? &data[index].value : nullptr;
}

template<typename T>
size_t TArrayTable<T>::GetSize() const
{
  return size;
}

template<typename T>
bool TArrayTable<T>::IsEmpty() const
{
  return size == 0;
}

template<typename T>
void TArrayTable<T>::Clear()
{
  for (size_t i = 0; i < capacity; ++i)
  {
    data[i] = TableEntry();
  }
  size = 0;
}

template<typename T>
std::vector<std::string> TArrayTable<T>::GetKeys() const
{
  std::vector<std::string> keys;
  keys.reserve(size);
  
  for (size_t i = 0; i < capacity; ++i)
  {
    if (!data[i].isDeleted)
    {
      keys.push_back(data[i].key);
    }
  }
  
  return keys;
}

template<typename T>
std::vector<std::pair<std::string, T>> TArrayTable<T>::GetAll() const
{
  std::vector<std::pair<std::string, T>> result;
  result.reserve(size);
  
  for (size_t i = 0; i < capacity; ++i)
  {
    if (!data[i].isDeleted)
    {
      result.emplace_back(data[i].key, data[i].value);
    }
  }
  
  return result;
}

template<typename T>
bool TArrayTable<T>::Contains(const std::string& key) const
{
  return FindIndex(key) != -1;
}

template<typename T>
size_t TArrayTable<T>::GetCapacity() const
{
  return capacity;
}

template<typename T>
double TArrayTable<T>::GetLoadFactor() const
{
  return static_cast<double>(size) / capacity;
}

using TPolinomArrayTable = TArrayTable<TPolinom>;