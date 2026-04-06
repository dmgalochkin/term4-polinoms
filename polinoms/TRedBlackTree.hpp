#pragma once
#include "TTable.hpp"
#include <string>
#include <vector>

template <typename T>
class TRedBlackTree : public TTable<T>
{
private:
  enum Color
  {
    RED,
    BLACK
  };

  struct Node
  {
    std::string key;
    T value;
    Color color;
    Node *left;
    Node *right;
    Node *parent;

    Node(const std::string &k, const T &v)
        : key(k), value(v), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
  };

  Node *root;
  size_t size;

  void rotateLeft(Node *x)
  {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
      y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
      root = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void rotateRight(Node *x)
  {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != nullptr)
      y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr)
      root = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void fixInsert(Node *z)
  {
    while (z != root && z->parent->color == RED)
    {
      if (z->parent == z->parent->parent->left)
      {
        Node *y = z->parent->parent->right;
        if (y != nullptr && y->color == RED)
        {
          z->parent->color = BLACK;
          y->color = BLACK;
          z->parent->parent->color = RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->right)
          {
            z = z->parent;
            rotateLeft(z);
          }
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          rotateRight(z->parent->parent);
        }
      }
      else
      {
        Node *y = z->parent->parent->left;
        if (y != nullptr && y->color == RED)
        {
          z->parent->color = BLACK;
          y->color = BLACK;
          z->parent->parent->color = RED;
          z = z->parent->parent;
        }
        else
        {
          if (z == z->parent->left)
          {
            z = z->parent;
            rotateRight(z);
          }
          z->parent->color = BLACK;
          z->parent->parent->color = RED;
          rotateLeft(z->parent->parent);
        }
      }
    }
    root->color = BLACK;
  }

  Node *findNode(const std::string &key) const
  {
    Node *cur = root;
    while (cur != nullptr)
    {
      if (key < cur->key)
        cur = cur->left;
      else if (key > cur->key)
        cur = cur->right;
      else
        return cur;
    }
    return nullptr;
  }

  void transplant(Node *u, Node *v)
  {
    if (u->parent == nullptr)
      root = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    if (v != nullptr)
      v->parent = u->parent;
  }

  Node *minimum(Node *x)
  {
    while (x->left != nullptr)
      x = x->left;
    return x;
  }

  void fixDelete(Node *x, Node *parent)
  {
    while (x != root && (x == nullptr || x->color == BLACK))
    {
      if (x == parent->left)
      {
        Node *w = parent->right;
        if (w->color == RED)
        {
          w->color = BLACK;
          parent->color = RED;
          rotateLeft(parent);
          w = parent->right;
        }
        if ((w->left == nullptr || w->left->color == BLACK) &&
            (w->right == nullptr || w->right->color == BLACK))
        {
          w->color = RED;
          x = parent;
          parent = x->parent;
        }
        else
        {
          if (w->right == nullptr || w->right->color == BLACK)
          {
            if (w->left != nullptr)
              w->left->color = BLACK;
            w->color = RED;
            rotateRight(w);
            w = parent->right;
          }
          w->color = parent->color;
          parent->color = BLACK;
          if (w->right != nullptr)
            w->right->color = BLACK;
          rotateLeft(parent);
          x = root;
          break;
        }
      }
      else
      {
        Node *w = parent->left;
        if (w->color == RED)
        {
          w->color = BLACK;
          parent->color = RED;
          rotateRight(parent);
          w = parent->left;
        }
        if ((w->right == nullptr || w->right->color == BLACK) &&
            (w->left == nullptr || w->left->color == BLACK))
        {
          w->color = RED;
          x = parent;
          parent = x->parent;
        }
        else
        {
          if (w->left == nullptr || w->left->color == BLACK)
          {
            if (w->right != nullptr)
              w->right->color = BLACK;
            w->color = RED;
            rotateLeft(w);
            w = parent->left;
          }
          w->color = parent->color;
          parent->color = BLACK;
          if (w->left != nullptr)
            w->left->color = BLACK;
          rotateRight(parent);
          x = root;
          break;
        }
      }
    }
    if (x != nullptr)
      x->color = BLACK;
  }

  void deleteNode(Node *z)
  {
    Node *y = z;
    Node *x = nullptr;
    Node *parent = nullptr;
    Color yOriginalColor = y->color;

    if (z->left == nullptr)
    {
      x = z->right;
      transplant(z, z->right);
      parent = z->parent;
    }
    else if (z->right == nullptr)
    {
      x = z->left;
      transplant(z, z->left);
      parent = z->parent;
    }
    else
    {
      y = minimum(z->right);
      yOriginalColor = y->color;
      x = y->right;
      if (y->parent == z)
      {
        if (x != nullptr)
          x->parent = y;
        parent = y;
      }
      else
      {
        transplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
        parent = y->parent;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }

    delete z;
    if (yOriginalColor == BLACK)
      fixDelete(x, parent);
  }

  void clearTree(Node *node)
  {
    if (node == nullptr)
      return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
  }

  void inorderCollect(Node *node, std::vector<std::pair<std::string, T>> &result) const
  {
    if (node == nullptr)
      return;
    inorderCollect(node->left, result);
    result.emplace_back(node->key, node->value);
    inorderCollect(node->right, result);
  }

  void inorderKeys(Node *node, std::vector<std::string> &keys) const
  {
    if (node == nullptr)
      return;
    inorderKeys(node->left, keys);
    keys.push_back(node->key);
    inorderKeys(node->right, keys);
  }

  Node *copyTree(Node *node)
  {
    if (node == nullptr)
      return nullptr;
    Node *newNode = new Node(node->key, node->value);
    newNode->color = node->color;
    newNode->left = copyTree(node->left);
    if (newNode->left != nullptr)
      newNode->left->parent = newNode;
    newNode->right = copyTree(node->right);
    if (newNode->right != nullptr)
      newNode->right->parent = newNode;
    return newNode;
  }

public:
  TRedBlackTree() : root(nullptr), size(0) {}
  ~TRedBlackTree() { clearTree(root); }

  TRedBlackTree(const TRedBlackTree &other) : root(nullptr), size(other.size)
  {
    root = copyTree(other.root);
  }

  TRedBlackTree &operator=(const TRedBlackTree &other)
  {
    if (this != &other)
    {
      clearTree(root);
      root = copyTree(other.root);
      size = other.size;
    }
    return *this;
  }

  TRedBlackTree(TRedBlackTree &&other) noexcept : root(other.root), size(other.size)
  {
    other.root = nullptr;
    other.size = 0;
  }

  TRedBlackTree &operator=(TRedBlackTree &&other) noexcept
  {
    if (this != &other)
    {
      clearTree(root);
      root = other.root;
      size = other.size;
      other.root = nullptr;
      other.size = 0;
    }
    return *this;
  }

  void Insert(const std::string &key, const T &value) override
  {
    Node *z = new Node(key, value);
    Node *y = nullptr;
    Node *x = root;
    while (x != nullptr)
    {
      y = x;
      if (z->key < x->key)
        x = x->left;
      else if (z->key > x->key)
        x = x->right;
      else
      {
        x->value = value;
        delete z;
        return;
      }
    }
    z->parent = y;
    if (y == nullptr)
      root = z;
    else if (z->key < y->key)
      y->left = z;
    x else y->right = z;
    ++size;
    fixInsert(z);
  }

  T *Find(const std::string &key) override
  {
    Node *node = findNode(key);
    return (node != nullptr) ? &node->value : nullptr;
  }

  const T *Find(const std::string &key) const override
  {
    Node *node = findNode(key);
    return (node != nullptr) ? &node->value : nullptr;
  }

  bool Remove(const std::string &key) override
  {
    Node *z = findNode(key);
    if (z == nullptr)
      return false;
    deleteNode(z);
    --size;
    return true;
  }

  bool Contains(const std::string &key) const override
  {
    return findNode(key) != nullptr;
  }

  void Clear() override
  {
    clearTree(root);
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
    inorderKeys(root, keys);
    return keys;
  }

  std::vector<std::pair<std::string, T>> GetAll() const override
  {
    std::vector<std::pair<std::string, T>> result;
    result.reserve(size);
    inorderCollect(root, result);
    return result;
  }
};

using TPolinomRedBlackTree = TRedBlackTree<TPolinom>;