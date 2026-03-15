#pragma once
#include "TTable.hpp"
#include <string>
#include <vector>

template<typename T>
class TListTable : public TTable<T>
{
private:
    struct Node
    {
        std::string key;
        T value;
        Node* next;
        
        Node(const std::string& k, const T& v) : key(k), value(v), next(nullptr) {}
    };
    
    Node* head;
    size_t size;
    
    Node* FindNode(const std::string& key)
    {
        Node* current = head;
        while (current != nullptr)
        {
            if (current->key == key)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    const Node* FindNode(const std::string& key) const
    {
        Node* current = head;
        while (current != nullptr)
        {
            if (current->key == key)
            {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    void ClearNodes()
    {
        while (head != nullptr)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
    
    void CopyNodes(const TListTable& other)
    {
        if (other.head == nullptr)
        {
            head = nullptr;
            size = 0;
            return;
        }
        
        head = new Node(other.head->key, other.head->value);
        Node* current = head;
        Node* otherCurrent = other.head->next;
        
        while (otherCurrent != nullptr)
        {
            current->next = new Node(otherCurrent->key, otherCurrent->value);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
        
        size = other.size;
    }
    
public:
    TListTable() : head(nullptr), size(0) {}
    
    ~TListTable()
    {
        ClearNodes();
    }
    
    TListTable(const TListTable& other) : head(nullptr), size(0)
    {
        CopyNodes(other);
    }
    
    TListTable& operator=(const TListTable& other)
    {
        if (this != &other)
        {
            ClearNodes();
            CopyNodes(other);
        }
        return *this;
    }
    
    TListTable(TListTable&& other) noexcept : head(other.head), size(other.size)
    {
        other.head = nullptr;
        other.size = 0;
    }
    
    TListTable& operator=(TListTable&& other) noexcept
    {
        if (this != &other)
        {
            ClearNodes();
            head = other.head;
            size = other.size;
            other.head = nullptr;
            other.size = 0;
        }
        return *this;
    }
    
    void Insert(const std::string& key, const T& value) override
    {
        Node* existing = FindNode(key);
        if (existing != nullptr)
        {
            existing->value = value;
        }
        else
        {
            Node* newNode = new Node(key, value);
            newNode->next = head;
            head = newNode;
            size++;
        }
    }
    
    T* Find(const std::string& key) override
    {
        Node* node = FindNode(key);
        return (node != nullptr) ? &(node->value) : nullptr;
    }
    
    const T* Find(const std::string& key) const override
    {
        const Node* node = FindNode(key);
        return (node != nullptr) ? &(node->value) : nullptr;
    }
    
    bool Remove(const std::string& key) override
    {
        if (head == nullptr)
        {
            return false;
        }
        
        if (head->key == key)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }
        
        Node* current = head;
        while (current->next != nullptr)
        {
            if (current->next->key == key)
            {
                Node* temp = current->next;
                current->next = temp->next;
                delete temp;
                size--;
                return true;
            }
            current = current->next;
        }
        
        return false;
    }
    
    bool Contains(const std::string& key) const override
    {
        return FindNode(key) != nullptr;
    }
    
    void Clear() override
    {
        ClearNodes();
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
        
        Node* current = head;
        while (current != nullptr)
        {
            keys.push_back(current->key);
            current = current->next;
        }
        
        return keys;
    }
    
    std::vector<std::pair<std::string, T>> GetAll() const override
    {
        std::vector<std::pair<std::string, T>> result;
        result.reserve(size);
        
        Node* current = head;
        while (current != nullptr)
        {
            result.emplace_back(current->key, current->value);
            current = current->next;
        }
        
        return result;
    }
};

using TPolinomListTable = TListTable<TPolinom>;