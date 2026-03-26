#pragma once
#include "TTable.hpp"
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <algorithm>

template<typename T>
class THashTable : public TTable<T>
{
private:
    std::vector<std::list<std::pair<std::string, T>>> buckets;
    size_t numElements;
    const double MAX_LOAD_FACTOR = 0.75;

    size_t hash(const std::string& key) const
    {
        return std::hash<std::string>{}(key);
    }

    void rehash()
    {
        size_t newSize = buckets.size() * 2;
        std::vector<std::list<std::pair<std::string, T>>> newBuckets(newSize);
        for (const auto& bucket : buckets)
        {
            for (const auto& pair : bucket)
            {
                size_t idx = hash(pair.first) % newSize;
                newBuckets[idx].push_back(pair);
            }
        }
        buckets.swap(newBuckets);
    }

public:
    explicit THashTable(size_t initialCapacity = 16) : numElements(0)
    {
        buckets.resize(initialCapacity);
    }

    ~THashTable() = default;

    void Insert(const std::string& key, const T& value) override
    {
        size_t idx = hash(key) % buckets.size();
        auto& bucket = buckets[idx];
        for (auto& pair : bucket)
        {
            if (pair.first == key)
            {
                pair.second = value;
                return;
            }
        }
        bucket.emplace_back(key, value);
        ++numElements;

        if (static_cast<double>(numElements) / buckets.size() > MAX_LOAD_FACTOR)
            rehash();
    }

    T* Find(const std::string& key) override
    {
        size_t idx = hash(key) % buckets.size();
        auto& bucket = buckets[idx];
        for (auto& pair : bucket)
            if (pair.first == key)
                return &pair.second;
        return nullptr;
    }

    const T* Find(const std::string& key) const override
    {
        size_t idx = hash(key) % buckets.size();
        const auto& bucket = buckets[idx];
        for (const auto& pair : bucket)
            if (pair.first == key)
                return &pair.second;
        return nullptr;
    }

    bool Remove(const std::string& key) override
    {
        size_t idx = hash(key) % buckets.size();
        auto& bucket = buckets[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it)
        {
            if (it->first == key)
            {
                bucket.erase(it);
                --numElements;
                return true;
            }
        }
        return false;
    }

    bool Contains(const std::string& key) const override
    {
        return Find(key) != nullptr;
    }

    void Clear() override
    {
        for (auto& bucket : buckets)
            bucket.clear();
        numElements = 0;
    }

    size_t GetSize() const override
    {
        return numElements;
    }

    bool IsEmpty() const override
    {
        return numElements == 0;
    }

    std::vector<std::string> GetKeys() const override
    {
        std::vector<std::string> keys;
        keys.reserve(numElements);
        for (const auto& bucket : buckets)
            for (const auto& pair : bucket)
                keys.push_back(pair.first);
        return keys;
    }

    std::vector<std::pair<std::string, T>> GetAll() const override
    {
        std::vector<std::pair<std::string, T>> result;
        result.reserve(numElements);
        for (const auto& bucket : buckets)
            for (const auto& pair : bucket)
                result.push_back(pair);
        return result;
    }
};

using TPolinomHashTable = THashTable<TPolinom>;