cmake_minimum_required(VERSION 3.16)

# Найти Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Включить автоматическую обработку MOC, UIC и RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Исходные файлы Qt приложения
set(QT_SOURCES
    main.cpp
    MainWindow.cpp
    PolynomialWidget.cpp
    TableWidget.cpp
    ExpressionWidget.cpp
)

# Заголовочные файлы Qt приложения
set(QT_HEADERS
    MainWindow.h
    PolynomialWidget.h
    TableWidget.h
    ExpressionWidget.h
)

# Создать исполняемый файл Qt приложения
add_executable(polynomial_qt_gui
    ${QT_SOURCES}
    ${QT_HEADERS}
)

# Связать с Qt библиотеками
target_link_libraries(polynomial_qt_gui
    Qt6::Core
    Qt6::Widgets
    polinoms  # Наша библиотека полиномов
)

# Включить заголовочные файлы
target_include_directories(polynomial_qt_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/polinoms
)

# Установить свойства C++
set_target_properties(polynomial_qt_gui PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)