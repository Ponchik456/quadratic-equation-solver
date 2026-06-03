# Решение квадратных уравнений на C++

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Программа для решения квадратных уравнений с поддержкой **длинной арифметики** и валидацией ввода через паттерн **«Интерпретатор»**.

## Возможности

- ✅ Решение полных и неполных квадратных уравнений
- ✅ Обработка вырожденных случаев (INF, NO SOLUTION, WRONG)
- ✅ Паттерн «Интерпретатор» для валидации входных данных
- ✅ Длинная арифметика (поддержка больших чисел)
- ✅ Модульные тесты (покрытие >80%)
- ✅ CMake для сборки
- ✅ Русскоязычный вывод и документация

## Формат вывода (по спецификации)

| Вывод | Описание |
|-------|----------|
| `OK` | Есть решение(я) |
| `INF` | Бесконечно много решений |
| `WRONG` | Неверный ввод коэффициентов |
| `NO SOLUTION` | Нет решений |

При `OK` следующими строками выводятся:
- Количество корней
- Корни (каждый на новой строке)

## Требования

- Компилятор с поддержкой C++17
- CMake 3.10+


## Сборка проекта

### Linux
```bash
mkdir build && cd build
cmake ..
make
./quadratic_solver
```

### Windows
```bash
cd C:\quadratic_solver
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Запуск тестов

# Windows
```bash
.\Release\tests.exe
```

# Linux/macOS
```bash
./tests
```

## Запуск программы
# Windows
```bash
.\Release\quadratic_solver.exe
```

# Linux/macOS
```bash
./quadratic_solver
```
