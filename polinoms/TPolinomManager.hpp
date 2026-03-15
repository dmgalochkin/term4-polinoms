#pragma once
#include "TTable.hpp"
#include "TArrayTable.hpp"
#include "TListTable.hpp"
#include "TOrderedArrayTable.hpp"
#include "TAVLTreeTable.hpp"
#include "TPostfixCalculator.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

enum class TableType
{
  ARRAY_LINEAR,
  LIST_LINEAR,
  ARRAY_ORDERED,
  TREE_AVL
};

class TPolinomManager
{
private:
  std::vector<TPolinomTable*> tables;
  std::vector<std::string> tableNames;
  size_t activeTableIndex;
  TPostfixCalculator calculator;
  
  TPolinomTable* CreateTable(TableType type);
  std::string GetTableTypeName(TableType type) const;

public:
  TPolinomManager();
  ~TPolinomManager();
  
  void InitializeTables();
  
  void SetActiveTable(size_t index);
  size_t GetActiveTableIndex() const;
  std::string GetActiveTableName() const;
  TPolinomTable* GetActiveTable();
  const TPolinomTable* GetActiveTable() const;
  
  bool AddPolinom(const std::string& name, const TPolinom& polinom);
  bool RemovePolinom(const std::string& name);
  
  TPolinom* FindPolinom(const std::string& name);
  const TPolinom* FindPolinom(const std::string& name) const;
  
  bool ContainsPolinom(const std::string& name) const;
  
  std::vector<std::pair<std::string, TPolinom>> GetAllPolinoms() const;
  std::vector<std::string> GetPolinomNames() const;
  
  TPolinom EvaluateExpression(const std::string& expression);
  bool EvaluateAndStore(const std::string& name, const std::string& expression);
  
  TPolinom EvaluateAt(const std::string& name, const std::vector<double>& point);
  TPolinom MultiplyByConstant(const std::string& name, double constant);
  TPolinom Differentiate(const std::string& name, int variable);
  TPolinom Integrate(const std::string& name, int variable);
  
  size_t GetTableCount() const;
  std::string GetTableName(size_t index) const;
  size_t GetPolinomCount() const;
  
  void PrintActiveTable(std::ostream& os = std::cout) const;
  void PrintTableInfo(std::ostream& os = std::cout) const;
  
  void ClearAll();
  
  void CreateSamplePolinoms();
};


inline TPolinomManager::TPolinomManager() : activeTableIndex(0)
{
  InitializeTables();
  calculator.SetActiveTable(GetActiveTable());
}

inline TPolinomManager::~TPolinomManager()
{
  for (TPolinomTable* table : tables)
  {
    delete table;
  }
}

inline TPolinomTable* TPolinomManager::CreateTable(TableType type)
{
  switch (type)
  {
    case TableType::ARRAY_LINEAR:
      return new TPolinomArrayTable();
    case TableType::LIST_LINEAR:
      return new TPolinomListTable();
    case TableType::ARRAY_ORDERED:
      return new TPolinomOrderedArrayTable();
    case TableType::TREE_AVL:
      return new TPolinomAVLTreeTable();
    default:
      return new TPolinomArrayTable();
  }
}

inline std::string TPolinomManager::GetTableTypeName(TableType type) const
{
  switch (type)
  {
    case TableType::ARRAY_LINEAR: return "Linear Array Table";
    case TableType::LIST_LINEAR: return "Linear List Table";
    case TableType::ARRAY_ORDERED: return "Ordered Array Table";
    case TableType::TREE_AVL: return "AVL Tree Table";
    default: return "Unknown Table";
  }
}

inline void TPolinomManager::InitializeTables()
{
  std::vector<TableType> types = {
    TableType::ARRAY_LINEAR,
    TableType::LIST_LINEAR,
    TableType::ARRAY_ORDERED,
    TableType::TREE_AVL
  };
  
  for (TPolinomTable* table : tables)
  {
    delete table;
  }
  tables.clear();
  tableNames.clear();
  
  for (TableType type : types)
  {
    tables.push_back(CreateTable(type));
    tableNames.push_back(GetTableTypeName(type));
  }
  
  activeTableIndex = 0;
}

inline void TPolinomManager::SetActiveTable(size_t index)
{
  if (index >= tables.size())
  {
    throw std::out_of_range("Table index out of range");
  }
  activeTableIndex = index;
  calculator.SetActiveTable(GetActiveTable());
}

inline size_t TPolinomManager::GetActiveTableIndex() const
{
  return activeTableIndex;
}

inline std::string TPolinomManager::GetActiveTableName() const
{
  return tableNames[activeTableIndex];
}

inline TPolinomTable* TPolinomManager::GetActiveTable()
{
  return tables[activeTableIndex];
}

inline const TPolinomTable* TPolinomManager::GetActiveTable() const
{
  return tables[activeTableIndex];
}

inline bool TPolinomManager::AddPolinom(const std::string& name, const TPolinom& polinom)
{
  try
  {
    for (auto& table : tables)
    {
      table->Insert(name, polinom);
    }
    return true;
  }
  catch (const std::exception&)
  {
    return false;
  }
}

inline bool TPolinomManager::RemovePolinom(const std::string& name)
{
  bool success = true;
  for (auto& table : tables)
  {
    if (!table->Remove(name))
    {
      success = false;
    }
  }
  return success;
}

inline TPolinom* TPolinomManager::FindPolinom(const std::string& name)
{
  return GetActiveTable()->Find(name);
}

inline const TPolinom* TPolinomManager::FindPolinom(const std::string& name) const
{
  return tables[activeTableIndex]->Find(name);
}

inline bool TPolinomManager::ContainsPolinom(const std::string& name) const
{
  return tables[activeTableIndex]->Contains(name);
}

inline std::vector<std::pair<std::string, TPolinom>> TPolinomManager::GetAllPolinoms() const
{
  return tables[activeTableIndex]->GetAll();
}

inline std::vector<std::string> TPolinomManager::GetPolinomNames() const
{
  return tables[activeTableIndex]->GetKeys();
}

inline TPolinom TPolinomManager::EvaluateExpression(const std::string& expression)
{
  return calculator.Evaluate(expression);
}

inline bool TPolinomManager::EvaluateAndStore(const std::string& name, const std::string& expression)
{
  try
  {
    TPolinom result = EvaluateExpression(expression);
    return AddPolinom(name, result);
  }
  catch (const std::exception&)
  {
    return false;
  }
}

inline TPolinom TPolinomManager::EvaluateAt(const std::string& name, const std::vector<double>& point)
{
  const TPolinom* polinom = FindPolinom(name);
  if (!polinom)
  {
    throw std::runtime_error("Polynomial not found: " + name);
  }
  
  double value = polinom->Evaluate(point);
  return TPolinom(TMonom(value, {0.0, 0.0, 0.0}));
}

inline TPolinom TPolinomManager::MultiplyByConstant(const std::string& name, double constant)
{
  const TPolinom* polinom = FindPolinom(name);
  if (!polinom)
  {
    throw std::runtime_error("Polynomial not found: " + name);
  }
  
  return (*polinom) * constant;
}

inline TPolinom TPolinomManager::Differentiate(const std::string& name, int variable)
{
  const TPolinom* polinom = FindPolinom(name);
  if (!polinom)
  {
    throw std::runtime_error("Polynomial not found: " + name);
  }
  
  return polinom->Differentiate(variable);
}

inline TPolinom TPolinomManager::Integrate(const std::string& name, int variable)
{
  const TPolinom* polinom = FindPolinom(name);
  if (!polinom)
  {
    throw std::runtime_error("Polynomial not found: " + name);
  }
  
  return polinom->Integrate(variable);
}

inline size_t TPolinomManager::GetTableCount() const
{
  return tables.size();
}

inline std::string TPolinomManager::GetTableName(size_t index) const
{
  if (index >= tableNames.size())
  {
    return "Unknown";
  }
  return tableNames[index];
}

inline size_t TPolinomManager::GetPolinomCount() const
{
  return tables[activeTableIndex]->GetSize();
}

inline void TPolinomManager::PrintActiveTable(std::ostream& os) const
{
  os << "\n=== " << GetActiveTableName() << " ===\n";
  os << std::left << std::setw(15) << "Name" << " | " << "Polynomial\n";
  os << std::string(50, '-') << "\n";
  
  auto polinoms = GetAllPolinoms();
  if (polinoms.empty())
  {
    os << "Table is empty.\n";
  }
  else
  {
    for (const auto& pair : polinoms)
    {
      os << std::left << std::setw(15) << pair.first << " | " << pair.second << "\n";
    }
  }
  os << "\nTotal polynomials: " << polinoms.size() << "\n\n";
}

inline void TPolinomManager::PrintTableInfo(std::ostream& os) const
{
  os << "\n=== Table Information ===\n";
  for (size_t i = 0; i < tables.size(); ++i)
  {
    std::string marker = (i == activeTableIndex) ? " [ACTIVE]" : "";
    os << i << ". " << tableNames[i] << marker 
       << " (Size: " << tables[i]->GetSize() << ")\n";
  }
  os << "\n";
}

inline void TPolinomManager::ClearAll()
{
  for (auto& table : tables)
  {
    table->Clear();
  }
}

inline void TPolinomManager::CreateSamplePolinoms()
{
  TMonom m1(3.2, {2.0, 3.0, 1.0});
  TMonom m2(-1.3, {1.0, 0.0, 4.0});
  std::vector<TMonom> monoms1 = {m1, m2};
  TPolinom pol1(monoms1);
  AddPolinom("pol1", pol1);
  
  TMonom m3(-3.2, {2.0, 3.0, 1.0});
  TMonom m4(1.3, {1.0, 0.0, 4.0});
  std::vector<TMonom> monoms2 = {m3, m4};
  TPolinom pol2(monoms2);
  AddPolinom("pol2", pol2);
  
  TMonom m5(6.0, {0.0, 0.0, 0.0});
  TPolinom const6(m5);
  AddPolinom("const6", const6);
  
  TMonom m6(4.0, {2.0, 0.0, 0.0});
  TPolinom q(m6);
  AddPolinom("q", q);
}