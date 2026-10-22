<p align="center">
  <img src="engine-logo.png" alt="Nexora Engine" width="320">
</p>

# Nexora Engine

Игровой движок на C++23.

## Требования

- Windows 10/11
- Visual Studio 2026 с компонентом «Разработка классических приложений на C++»
- CMake 3.31+
- Python 3
- Conan 2 (`pip install conan`)
- clang-format — только для форматирования кода

## Сборка

Все скрипты лежат в папке `Automation`. Конфигурация передаётся аргументом: `Debug` (по умолчанию) или `Release`.

| Скрипт | Что делает |
|---|---|
| `Automation\clean_rebuild_test.bat` | удаляет `Build`, пересобирает проект и запускает тесты |
| `Automation\rebuild_test.bat` | пересобирает проект и запускает тесты |
| `Automation\rebuild.bat` | пересобирает проект |
| `Automation\format_code.bat` | форматирует код по `Engine\.clang-format` |

Пример:

```bat
Automation\clean_rebuild_test.bat Release
```

Первая сборка дольше обычной: Conan скачивает и собирает зависимости (spdlog, GLFW, EnTT, GoogleTest).

Результат сборки лежит в `Build\bin\<Debug|Release>\`:

- `NexoraEngine.exe` — движок
- `NexoraEngineTestRunner.exe` — тесты

Решение для Visual Studio: `Build\Nexora.slnx`.
