cmake_minimum_required(VERSION 3.10)
project(QuadraticSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа
add_executable(quadratic_solver
    main.cpp
    src/big_number.cpp
    src/expression.cpp
    src/quadratic_solver.cpp
)

target_include_directories(quadratic_solver PRIVATE include)

# Тесты (без Google Test, свои простые тесты)
add_executable(tests
    tests/test_quadratic.cpp
    src/big_number.cpp
    src/expression.cpp
    src/quadratic_solver.cpp
)

target_include_directories(tests PRIVATE include)

enable_testing()
add_test(NAME QuadraticTests COMMAND tests)
