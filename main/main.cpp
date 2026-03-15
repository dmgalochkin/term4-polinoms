#include <iostream>
#include <string>
#include <vector>
#include "TPolinomManager.hpp"

void PrintMenu()
{
  std::cout << "\n=== Лабораторная работа: Алгебра полиномов ===\n";
  std::cout << "1. Показать активную таблицу\n";
  std::cout << "2. Показать информацию о таблицах\n";
  std::cout << "3. Сменить активную таблицу\n";
  std::cout << "4. Добавить полином\n";
  std::cout << "5. Удалить полином\n";
  std::cout << "6. Найти полином\n";
  std::cout << "7. Вычислить выражение\n";
  std::cout << "8. Вычислить и сохранить выражение\n";
  std::cout << "9. Операции над полиномом\n";
  std::cout << "10. Создать примеры полиномов\n";
  std::cout << "11. Очистить все таблицы\n";
  std::cout << "12. Демонстрация из задания\n";
  std::cout << "0. Выход\n";
  std::cout << "Выберите действие: ";
}

void ShowPolynomialOperationsMenu()
{
  std::cout << "\n=== Операции над полиномом ===\n";
  std::cout << "1. Вычислить в точке\n";
  std::cout << "2. Умножить на константу\n";
  std::cout << "3. Дифференцировать\n";
  std::cout << "4. Интегрировать\n";
  std::cout << "Выберите операцию: ";
}

void DemoFromAssignment(TPolinomManager& manager)
{
  std::cout << "\n=== Демонстрация из задания ===\n";
  
  // Очищаем таблицы и создаем полиномы из примера
  manager.ClearAll();
  
  std::cout << "Создаем полиномы:\n";
  
  // pol1 = 3.2x^2*y^3*z^1 - 1.3x^1*z^4
  TMonom m1(3.2, {2.0, 3.0, 1.0});
  TMonom m2(-1.3, {1.0, 0.0, 4.0});
  std::vector<TMonom> monoms1 = {m1, m2};
  TPolinom pol1(monoms1);
  manager.AddPolinom("pol1", pol1);
  std::cout << "pol1 = " << pol1 << "\n";
  
  // pol2 = -3.2x^2*y^3*z^1 + 1.3x^1*z^4
  TMonom m3(-3.2, {2.0, 3.0, 1.0});
  TMonom m4(1.3, {1.0, 0.0, 4.0});
  std::vector<TMonom> monoms2 = {m3, m4};
  TPolinom pol2(monoms2);
  manager.AddPolinom("pol2", pol2);
  std::cout << "pol2 = " << pol2 << "\n";
  
  // const6 = 6.0
  TMonom m5(6.0, {0.0, 0.0, 0.0});
  TPolinom const6(m5);
  manager.AddPolinom("const6", const6);
  std::cout << "const6 = " << const6 << "\n";
  
  // q = 4.0x^2
  TMonom m6(4.0, {2.0, 0.0, 0.0});
  TPolinom q(m6);
  manager.AddPolinom("q", q);
  std::cout << "q = " << q << "\n";
  
  std::cout << "\nВычисляем выражение: new_pol = 2 * pol1 + 2 * pol2 + 3.6 * q - const6\n";
  
  try
  {
    // Вычисляем выражение
    std::string expression = "2 * pol1 + 2 * pol2 + 3.6 * q - const6";
    TPolinom result = manager.EvaluateExpression(expression);
    
    std::cout << "Результат: " << result << "\n";
    
    // Сохраняем результат
    manager.AddPolinom("new_pol", result);
    std::cout << "Полином new_pol добавлен во все таблицы.\n";
    
    std::cout << "\nПроверка: 2*pol1 + 2*pol2 должно дать 0 (pol1 и pol2 противоположны)\n";
    std::cout << "3.6 * q = 3.6 * 4.0 * x^2 = 14.4 * x^2\n";
    std::cout << "14.4 * x^2 - 6.0 = результат\n";
    
  }
  catch (const std::exception& e)
  {
    std::cout << "Ошибка при вычислении: " << e.what() << "\n";
  }
  
  std::cout << "\nТекущее состояние активной таблицы:\n";
  manager.PrintActiveTable();
}

int main()
{
  TPolinomManager manager;
  int choice;
  
  std::cout << "Добро пожаловать в систему работы с полиномами!\n";
  std::cout << "Система поддерживает полиномы от 3 переменных (x, y, z).\n";
  
  while (true)
  {
    PrintMenu();
    std::cin >> choice;
    
    try
    {
      switch (choice)
      {
        case 0:
          std::cout << "До свидания!\n";
          return 0;
          
        case 1:
          manager.PrintActiveTable();
          break;
          
        case 2:
          manager.PrintTableInfo();
          break;
          
        case 3:
        {
          std::cout << "Доступные таблицы:\n";
          manager.PrintTableInfo();
          std::cout << "Введите номер таблицы: ";
          size_t index;
          std::cin >> index;
          manager.SetActiveTable(index);
          std::cout << "Активная таблица изменена на: " << manager.GetActiveTableName() << "\n";
          break;
        }
        
        case 4:
        {
          std::cout << "Введите имя полинома: ";
          std::string name;
          std::cin >> name;
          
          std::cout << "Введите количество мономов: ";
          size_t count;
          std::cin >> count;
          
          std::vector<TMonom> monoms;
          for (size_t i = 0; i < count; ++i)
          {
            std::cout << "Моном " << (i+1) << ":\n";
            std::cout << "  Коэффициент: ";
            double coef;
            std::cin >> coef;
            
            std::cout << "  Степени (x y z): ";
            double px, py, pz;
            std::cin >> px >> py >> pz;
            
            monoms.emplace_back(coef, std::vector<double>{px, py, pz});
          }
          
          TPolinom polinom(monoms);
          if (manager.AddPolinom(name, polinom))
          {
            std::cout << "Полином " << name << " = " << polinom << " добавлен.\n";
          }
          else
          {
            std::cout << "Ошибка при добавлении полинома.\n";
          }
          break;
        }
        
        case 5:
        {
          std::cout << "Введите имя полинома для удаления: ";
          std::string name;
          std::cin >> name;
          
          if (manager.RemovePolinom(name))
          {
            std::cout << "Полином " << name << " удален из всех таблиц.\n";
          }
          else
          {
            std::cout << "Полином не найден или ошибка при удалении.\n";
          }
          break;
        }
        
        case 6:
        {
          std::cout << "Введите имя полинома для поиска: ";
          std::string name;
          std::cin >> name;
          
          const TPolinom* found = manager.FindPolinom(name);
          if (found)
          {
            std::cout << "Найден: " << name << " = " << *found << "\n";
          }
          else
          {
            std::cout << "Полином " << name << " не найден в активной таблице.\n";
          }
          break;
        }
        
        case 7:
        {
          std::cout << "Введите выражение (например: pol1 + pol2 * 2): ";
          std::cin.ignore();
          std::string expression;
          std::getline(std::cin, expression);
          
          TPolinom result = manager.EvaluateExpression(expression);
          std::cout << "Результат: " << result << "\n";
          break;
        }
        
        case 8:
        {
          std::cout << "Введите имя для результата: ";
          std::string name;
          std::cin >> name;
          
          std::cout << "Введите выражение: ";
          std::cin.ignore();
          std::string expression;
          std::getline(std::cin, expression);
          
          if (manager.EvaluateAndStore(name, expression))
          {
            std::cout << "Результат сохранен как " << name << "\n";
          }
          else
          {
            std::cout << "Ошибка при вычислении или сохранении.\n";
          }
          break;
        }
        
        case 9:
        {
          ShowPolynomialOperationsMenu();
          int opChoice;
          std::cin >> opChoice;
          
          std::cout << "Введите имя полинома: ";
          std::string name;
          std::cin >> name;
          
          switch (opChoice)
          {
            case 1:
            {
              std::cout << "Введите точку (x y z): ";
              double x, y, z;
              std::cin >> x >> y >> z;
              TPolinom result = manager.EvaluateAt(name, {x, y, z});
              std::cout << "Значение в точке (" << x << ", " << y << ", " << z << "): " << result << "\n";
              break;
            }
            case 2:
            {
              std::cout << "Введите константу: ";
              double constant;
              std::cin >> constant;
              TPolinom result = manager.MultiplyByConstant(name, constant);
              std::cout << "Результат умножения на " << constant << ": " << result << "\n";
              break;
            }
            case 3:
            {
              std::cout << "Введите переменную (0=x, 1=y, 2=z): ";
              int var;
              std::cin >> var;
              TPolinom result = manager.Differentiate(name, var);
              char varName = (var == 0) ? 'x' : (var == 1) ? 'y' : 'z';
              std::cout << "Производная по " << varName << ": " << result << "\n";
              break;
            }
            case 4:
            {
              std::cout << "Введите переменную (0=x, 1=y, 2=z): ";
              int var;
              std::cin >> var;
              TPolinom result = manager.Integrate(name, var);
              char varName = (var == 0) ? 'x' : (var == 1) ? 'y' : 'z';
              std::cout << "Интеграл по " << varName << ": " << result << " + C\n";
              break;
            }
            default:
              std::cout << "Неверный выбор операции.\n";
          }
          break;
        }
        
        case 10:
          manager.CreateSamplePolinoms();
          std::cout << "Примеры полиномов созданы.\n";
          manager.PrintActiveTable();
          break;
          
        case 11:
          manager.ClearAll();
          std::cout << "Все таблицы очищены.\n";
          break;
          
        case 12:
          DemoFromAssignment(manager);
          break;
          
        default:
          std::cout << "Неверный выбор. Попробуйте снова.\n";
      }
    }
    catch (const std::exception& e)
    {
      std::cout << "Ошибка: " << e.what() << "\n";
    }
    
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
  }
  
  return 0;
}