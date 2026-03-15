#pragma once
#include "TPolinom.hpp"
#include "TTable.hpp"
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>

class TPostfixCalculator
{
private:
  TPolinomTable* table;
  
  bool IsNumber(const std::string& str) const;
  bool IsOperator(const std::string& str) const;
  double StringToNumber(const std::string& str) const;
  TPolinom CreateConstantPolinom(double value) const;
  TPolinom ExecuteOperation(const TPolinom& left, const TPolinom& right, const std::string& op) const;
  TPolinom ExecuteUnaryOperation(const TPolinom& operand, const std::string& op) const;

public:
  explicit TPostfixCalculator(TPolinomTable* activeTable = nullptr);
  
  void SetActiveTable(TPolinomTable* activeTable);
  std::vector<std::string> Tokenize(const std::string& expression) const;
  std::vector<std::string> InfixToPostfix(const std::vector<std::string>& tokens) const;
  TPolinom EvaluatePostfix(const std::vector<std::string>& postfixTokens) const;
  TPolinom Evaluate(const std::string& expression) const;
  int GetOperatorPriority(const std::string& op) const;
  bool IsLeftAssociative(const std::string& op) const;
};


inline TPostfixCalculator::TPostfixCalculator(TPolinomTable* activeTable)
  : table(activeTable)
{
}

inline void TPostfixCalculator::SetActiveTable(TPolinomTable* activeTable)
{
  table = activeTable;
}

inline bool TPostfixCalculator::IsNumber(const std::string& str) const
{
  if (str.empty()) return false;
  
  size_t start = 0;
  if (str[0] == '-' || str[0] == '+') start = 1;
  if (start >= str.length()) return false;
  
  bool hasDot = false;
  for (size_t i = start; i < str.length(); ++i)
  {
    if (str[i] == '.')
    {
      if (hasDot) return false;
      hasDot = true;
    }
    else if (!std::isdigit(str[i]))
    {
      return false;
    }
  }
  return true;
}

inline bool TPostfixCalculator::IsOperator(const std::string& str) const
{
  return str == "+" || str == "-" || str == "*" || str == "/" || 
         str == "^" || str == "diff" || str == "int";
}

inline double TPostfixCalculator::StringToNumber(const std::string& str) const
{
  std::istringstream iss(str);
  double value;
  iss >> value;
  return value;
}

inline TPolinom TPostfixCalculator::CreateConstantPolinom(double value) const
{
  TMonom constantMonom(value, {0.0, 0.0, 0.0});
  return TPolinom(constantMonom);
}

inline TPolinom TPostfixCalculator::ExecuteOperation(const TPolinom& left, const TPolinom& right, const std::string& op) const
{
  if (op == "+")
  {
    return left + right;
  }
  else if (op == "-")
  {
    return left - right;
  }
  else if (op == "*")
  {
    return left * right;
  }
  else if (op == "/")
  {
    if (right.GetMonomCount() == 1 &&
        right.GetMonoms()[0].GetPowers()[0] == 0.0 &&
        right.GetMonoms()[0].GetPowers()[1] == 0.0 &&
        right.GetMonoms()[0].GetPowers()[2] == 0.0)
    {
      double divisor = right.GetMonoms()[0].GetCoef();
      if (std::abs(divisor) < EPS)
      {
        throw std::runtime_error("Division by zero");
      }
      return left * (1.0 / divisor);
    }
    else
    {
      throw std::runtime_error("Polynomial division not implemented for non-constant divisors");
    }
  }
  else
  {
    throw std::runtime_error("Unknown binary operator: " + op);
  }
}

inline TPolinom TPostfixCalculator::ExecuteUnaryOperation(const TPolinom& operand, const std::string& op) const
{
  if (op == "diff")
  {
    return operand.Differentiate(0);
  }
  else if (op == "int")
  {
    return operand.Integrate(0);
  }
  else
  {
    throw std::runtime_error("Unknown unary operator: " + op);
  }
}

inline std::vector<std::string> TPostfixCalculator::Tokenize(const std::string& expression) const
{
  std::vector<std::string> tokens;
  std::string current;
  
  for (size_t i = 0; i < expression.length(); ++i)
  {
    char c = expression[i];
    
    if (std::isspace(c))
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')')
    {
      if (!current.empty())
      {
        tokens.push_back(current);
        current.clear();
      }
      tokens.push_back(std::string(1, c));
    }
    else
    {
      current += c;
    }
  }
  
  if (!current.empty())
  {
    tokens.push_back(current);
  }
  
  return tokens;
}

inline int TPostfixCalculator::GetOperatorPriority(const std::string& op) const
{
  if (op == "+" || op == "-") return 1;
  if (op == "*" || op == "/") return 2;
  if (op == "^") return 3;
  if (op == "diff" || op == "int") return 4;
  return 0;
}

inline bool TPostfixCalculator::IsLeftAssociative(const std::string& op) const
{
  return op != "^"; // Все операторы левоассоциативны, кроме возведения в степень
}

inline std::vector<std::string> TPostfixCalculator::InfixToPostfix(const std::vector<std::string>& tokens) const
{
  std::vector<std::string> output;
  std::stack<std::string> operators;
  
  for (const std::string& token : tokens)
  {
    if (IsNumber(token) || (!IsOperator(token) && token != "(" && token != ")"))
    {
      output.push_back(token);
    }
    else if (IsOperator(token))
    {
      while (!operators.empty() && 
             operators.top() != "(" &&
             ((IsLeftAssociative(token) && GetOperatorPriority(token) <= GetOperatorPriority(operators.top())) ||
              (!IsLeftAssociative(token) && GetOperatorPriority(token) < GetOperatorPriority(operators.top()))))
      {
        output.push_back(operators.top());
        operators.pop();
      }
      operators.push(token);
    }
    else if (token == "(")
    {
      operators.push(token);
    }
    else if (token == ")")
    {
      while (!operators.empty() && operators.top() != "(")
      {
        output.push_back(operators.top());
        operators.pop();
      }
      if (!operators.empty())
      {
        operators.pop(); // Удаляем "("
      }
    }
  }
  
  while (!operators.empty())
  {
    output.push_back(operators.top());
    operators.pop();
  }
  
  return output;
}

inline TPolinom TPostfixCalculator::EvaluatePostfix(const std::vector<std::string>& postfixTokens) const
{
  std::stack<TPolinom> stack;
  
  for (const std::string& token : postfixTokens)
  {
    if (IsNumber(token))
    {
      double value = StringToNumber(token);
      stack.push(CreateConstantPolinom(value));
    }
    else if (IsOperator(token))
    {
      if (token == "diff" || token == "int")
      {
        if (stack.empty())
        {
          throw std::runtime_error("Not enough operands for unary operator: " + token);
        }
        TPolinom operand = stack.top();
        stack.pop();
        stack.push(ExecuteUnaryOperation(operand, token));
      }
      else
      {
        if (stack.size() < 2)
        {
          throw std::runtime_error("Not enough operands for binary operator: " + token);
        }
        TPolinom right = stack.top(); stack.pop();
        TPolinom left = stack.top(); stack.pop();
        stack.push(ExecuteOperation(left, right, token));
      }
    }
    else
    {
      if (!table)
      {
        throw std::runtime_error("No active table set for polynomial lookup");
      }
      
      const TPolinom* found = table->Find(token);
      if (!found)
      {
        throw std::runtime_error("Polynomial not found: " + token);
      }
      stack.push(*found);
    }
  }
  
  if (stack.size() != 1)
  {
    throw std::runtime_error("Invalid expression: stack size is " + std::to_string(stack.size()));
  }
  
  return stack.top();
}

inline TPolinom TPostfixCalculator::Evaluate(const std::string& expression) const
{
  std::vector<std::string> tokens = Tokenize(expression);
  std::vector<std::string> postfix = InfixToPostfix(tokens);
  return EvaluatePostfix(postfix);
}