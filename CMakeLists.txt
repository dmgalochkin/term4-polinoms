cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME term4-polinoms)
project(${PROJECT_NAME})

set(application main)
set(library polinoms)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Config" FORCE)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/polinoms)

# Сборка компонентов
add_subdirectory(polinoms)
add_subdirectory(qt_gui)
add_subdirectory(gtest)
add_subdirectory(test)