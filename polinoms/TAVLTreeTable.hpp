#pragma once
#include "TTable.hpp"
#include <string>
#include <vector>

template<typename T>
class TAVLTreeTable : public TTable<T>
{
private:
    struct Node
    {
        std::string key;
        T value;
        Node* left;
        Node* right;
        int height;
        
        Node(const std::string& k, const T& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };
    
    Node* root;
    size_t size;
    
    int GetHeight(Node* node)
    {
        return (node == nullptr) ? 0 : node->height;
    }
    
    void UpdateHeight(Node* node)
    {
        if (node != nullptr)
        {
            int leftHeight = GetHeight(node->left);
            int rightHeight = GetHeight(node->right);
            node->height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
        }
    }
    
    int GetBalance(Node* node)
    {
        return (node == nullptr) ? 0 : GetHeight(node->left) - GetHeight(node->right);
    }
    
    Node* RotateRight(Node* y)
    {
        Node* x = y->left;
        Node* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        UpdateHeight(y);
        UpdateHeight(x);
        
        return x;
    }
    
    Node* RotateLeft(Node* x)
    {
        Node* y = x->right;
        Node* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        UpdateHeight(x);
        UpdateHeight(y);
        
        return y;
    }
    
    Node* InsertNode(Node* node, const std::string& key, const T& value, bool& inserted)
    {
        if (node == nullptr)
        {
            inserted = true;
            size++;
            return new Node(key, value);
        }
        
        if (key < node->key)
        {
            node->left = InsertNode(node->left, key, value, inserted);
        }
        else if (key > node->key)
        {
            node->right = InsertNode(node->right, key, value, inserted);
        }
        else
        {
            node->value = value;
            inserted = false;
            return node;
        }
        
        UpdateHeight(node);
        
        int balance = GetBalance(node);
        
        if (balance > 1 && key < node->left->key)
        {
            return RotateRight(node);
        }
        
        if (balance < -1 && key > node->right->key)
        {
            return RotateLeft(node);
        }
        
        if (balance > 1 && key > node->left->key)
        {
            node->left = RotateLeft(node->left);
            return RotateRight(node);
        }
        
        if (balance < -1 && key < node->right->key)
        {
            node->right = RotateRight(node->right);
            return RotateLeft(node);
        }
        
        return node;
    }
    
    Node* FindMin(Node* node)
    {
        while (node->left != nullptr)
        {
            node = node->left;
        }
        return node;
    }
    
    Node* RemoveNode(Node* node, const std::string& key, bool& removed)
    {
        if (node == nullptr)
        {
            removed = false;
            return node;
        }
        
        if (key < node->key)
        {
            node->left = RemoveNode(node->left, key, removed);
        }
        else if (key > node->key)
        {
            node->right = RemoveNode(node->right, key, removed);
        }
        else
        {
            removed = true;
            size--;
            
            if (node->left == nullptr || node->right == nullptr)
            {
                Node* temp = (node->left != nullptr) ? node->left : node->right;
                
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    *node = *temp;
                }
                delete temp;
            }
            else
            {
                Node* temp = FindMin(node->right);
                
                node->key = temp->key;
                node->value = temp->value;
                
                bool tempRemoved;
                node->right = RemoveNode(node->right, temp->key, tempRemoved);
            }
        }
        
        if (node == nullptr)
        {
            return node;
        }
        
        UpdateHeight(node);
        
        int balance = GetBalance(node);
        
        if (balance > 1 && GetBalance(node->left) >= 0)
        {
            return RotateRight(node);
        }
        
        if (balance > 1 && GetBalance(node->left) < 0)
        {
            node->left = RotateLeft(node->left);
            return RotateRight(node);
        }
        
        if (balance < -1 && GetBalance(node->right) <= 0)
        {
            return RotateLeft(node);
        }
        
        if (balance < -1 && GetBalance(node->right) > 0)
        {
            node->right = RotateRight(node->right);
            return RotateLeft(node);
        }
        
        return node;
    }
    
    Node* FindNode(Node* node, const std::string& key)
    {
        if (node == nullptr || node->key == key)
        {
            return node;
        }
        
        if (key < node->key)
        {
            return FindNode(node->left, key);
        }
        else
        {
            return FindNode(node->right, key);
        }
    }
    
    const Node* FindNode(Node* node, const std::string& key) const
    {
        if (node == nullptr || node->key == key)
        {
            return node;
        }
        
        if (key < node->key)
        {
            return FindNode(node->left, key);
        }
        else
        {
            return FindNode(node->right, key);
        }
    }
    
    void ClearTree(Node* node)
    {
        if (node != nullptr)
        {
            ClearTree(node->left);
            ClearTree(node->right);
            delete node;
        }
    }
    
    Node* CopyTree(Node* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }
        
        Node* newNode = new Node(node->key, node->value);
        newNode->height = node->height;
        newNode->left = CopyTree(node->left);
        newNode->right = CopyTree(node->right);
        
        return newNode;
    }
    
    void InOrderTraversal(Node* node, std::vector<std::pair<std::string, T>>& result) const
    {
        if (node != nullptr)
        {
            InOrderTraversal(node->left, result);
            result.emplace_back(node->key, node->value);
            InOrderTraversal(node->right, result);
        }
    }
    
    void InOrderKeys(Node* node, std::vector<std::string>& keys) const
    {
        if (node != nullptr)
        {
            InOrderKeys(node->left, keys);
            keys.push_back(node->key);
            InOrderKeys(node->right, keys);
        }
    }
    
public:
    TAVLTreeTable() : root(nullptr), size(0) {}
    
    ~TAVLTreeTable()
    {
        ClearTree(root);
    }
    
    TAVLTreeTable(const TAVLTreeTable& other) : root(nullptr), size(other.size)
    {
        root = CopyTree(other.root);
    }
    
    TAVLTreeTable& operator=(const TAVLTreeTable& other)
    {
        if (this != &other)
        {
            ClearTree(root);
            root = CopyTree(other.root);
            size = other.size;
        }
        return *this;
    }
    
    TAVLTreeTable(TAVLTreeTable&& other) noexcept : root(other.root), size(other.size)
    {
        other.root = nullptr;
        other.size = 0;
    }
    
    TAVLTreeTable& operator=(TAVLTreeTable&& other) noexcept
    {
        if (this != &other)
        {
            ClearTree(root);
            root = other.root;
            size = other.size;
            other.root = nullptr;
            other.size = 0;
        }
        return *this;
    }
    
    void Insert(const std::string& key, const T& value) override
    {
        bool inserted;
        root = InsertNode(root, key, value, inserted);
    }
    
    T* Find(const std::string& key) override
    {
        Node* node = FindNode(root, key);
        return (node != nullptr) ? &(node->value) : nullptr;
    }
    
    const T* Find(const std::string& key) const override
    {
        const Node* node = FindNode(root, key);
        return (node != nullptr) ? &(node->value) : nullptr;
    }
    
    bool Remove(const std::string& key) override
    {
        bool removed;
        root = RemoveNode(root, key, removed);
        return removed;
    }
    
    bool Contains(const std::string& key) const override
    {
        return FindNode(root, key) != nullptr;
    }
    
    void Clear() override
    {
        ClearTree(root);
        root = nullptr;
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
        InOrderKeys(root, keys);
        return keys;
    }
    
    std::vector<std::pair<std::string, T>> GetAll() const override
    {
        std::vector<std::pair<std::string, T>> result;
        result.reserve(size);
        InOrderTraversal(root, result);
        return result;
    }
};

using TPolinomAVLTreeTable = TAVLTreeTable<TPolinom>;