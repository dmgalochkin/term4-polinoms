#pragma once
#include "TTable.hpp"
#include <string>
#include <vector>

template<typename T>
class TOrderedArrayTable : public TTable<T>
{
private:
    struct TableEntry
    {
        std::string key;
        T value;
        
        TableEntry() = default;
        TableEntry(const std::string& k, const T& v) : key(k), value(v) {}
    };
    
    TableEntry* data;
    size_t size;
    size_t capacity;
    
    size_t BinarySearch(const std::string& key) const
    {
        if (size == 0)
        {
            return 0;
        }
        
        size_t left = 0;
        size_t right = size;
        
        while (left < right)
        {
            size_t mid = left + (right - left) / 2;
            if (data[mid].key < key)
            {
                left = mid + 1;
            }
            else
            {
                right = mid;
            }
        }
        
        return left;
    }
    
    size_t FindExact(const std::string& key) const
    {
        size_t pos = BinarySearch(key);
        if (pos < size && data[pos].key == key)
        {
            return pos;
        }
        return size;
    }
    
    void Resize()
    {
        size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
        TableEntry* newData = new TableEntry[newCapacity];
        
        for (size_t i = 0; i < size; ++i)
        {
            newData[i] = data[i];
        }
        
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }
    
    void ShiftRight(size_t pos)
    {
        for (size_t i = size; i > pos; --i)
        {
            data[i] = data[i - 1];
        }
    }
    
    void ShiftLeft(size_t pos)
    {
        for (size_t i = pos; i < size - 1; ++i)
        {
            data[i] = data[i + 1];
        }
    }
    
public:
    TOrderedArrayTable() : data(nullptr), size(0), capacity(0) {}
    
    ~TOrderedArrayTable()
    {
        delete[] data;
    }
    
    TOrderedArrayTable(const TOrderedArrayTable& other)
        : data(nullptr), size(other.size), capacity(other.capacity)
    {
        if (capacity > 0)
        {
            data = new TableEntry[capacity];
            for (size_t i = 0; i < size; ++i)
            {
                data[i] = other.data[i];
            }
        }
    }
    
    TOrderedArrayTable& operator=(const TOrderedArrayTable& other)
    {
        if (this != &other)
        {
            delete[] data;
            
            size = other.size;
            capacity = other.capacity;
            
            if (capacity > 0)
            {
                data = new TableEntry[capacity];
                for (size_t i = 0; i < size; ++i)
                {
                    data[i] = other.data[i];
                }
            }
            else
            {
                data = nullptr;
            }
        }
        return *this;
    }
    
    TOrderedArrayTable(TOrderedArrayTable&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity)
    {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    
    TOrderedArrayTable& operator=(TOrderedArrayTable&& other) noexcept
    {
        if (this != &other)
        {
            delete[] data;
            
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }
    
    void Insert(const std::string& key, const T& value) override
    {
        size_t pos = FindExact(key);
        
        if (pos < size)
        {
            data[pos].value = value;
        }
        else
        {
            if (size >= capacity)
            {
                Resize();
            }
            
            pos = BinarySearch(key);
            ShiftRight(pos);
            data[pos] = TableEntry(key, value);
            size++;
        }
    }
    
    T* Find(const std::string& key) override
    {
        size_t pos = FindExact(key);
        return (pos < size) ? &(data[pos].value) : nullptr;
    }
    
    const T* Find(const std::string& key) const override
    {
        size_t pos = FindExact(key);
        return (pos < size) ? &(data[pos].value) : nullptr;
    }
    
    bool Remove(const std::string& key) override
    {
        size_t pos = FindExact(key);
        if (pos < size)
        {
            ShiftLeft(pos);
            size--;
            return true;
        }
        return false;
    }
    
    bool Contains(const std::string& key) const override
    {
        return FindExact(key) < size;
    }
    
    void Clear() override
    {
        size = 0;
    }
    
    size_t GetSize() const override
    {
        return size;
    }
    
    bool IsEmpty() const override
    {
        return size == 0;
    }
    
    std::vector<std::string> GetKeys() const override
    {
        std::vector<std::string> keys;
        keys.reserve(size);
        
        for (size_t i = 0; i < size; ++i)
        {
            keys.push_back(data[i].key);
        }
        
        return keys;
    }
    
    std::vector<std::pair<std::string, T>> GetAll() const override
    {
        std::vector<std::pair<std::string, T>> result;
        result.reserve(size);
        
        for (size_t i = 0; i < size; ++i)
        {
            result.emplace_back(data[i].key, data[i].value);
        }
        
        return result;
    }
};

using TPolinomOrderedArrayTable = TOrderedArrayTable<TPolinom>;