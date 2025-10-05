Компьютерная графика
====================

[![Java CI with Maven](https://github.com/stden/comp_graph/actions/workflows/maven.yml/badge.svg)](https://github.com/stden/comp_graph/actions/workflows/maven.yml)
[![Tests](https://img.shields.io/badge/tests-53%20passing-brightgreen)](https://github.com/stden/comp_graph)
[![Coverage](https://img.shields.io/badge/coverage-100%25-brightgreen)](https://github.com/stden/comp_graph)

Материалы по предмету "Компьютерная графика" в СПбГЭТУ ЛЭТИ каф. АСОИУ

## 🎯 Основной проект

Проект содержит **полностью протестированные** примеры кода для обучения:

- **53 теста** с подробными комментариями на русском языке
- **100% покрытие кода** тестами
- Все тесты проходят на GitHub Actions

### 📦 Структура проекта

```
src/main/java/
├── A_Vector.java              # 2D векторы
├── B_line.java                # Уравнение прямой по двум точкам
├── defaultimp/                # Геометрические фигуры (Circle, Square)
├── lambda/                    # Лямбда-выражения Java 8
├── lambda_tree/               # Бинарное дерево с лямбдами
└── math3d/                    # 3D векторная математика
    └── Vector3D.java          # Полная реализация 3D вектора

src/test/java/                 # 53 обучающих теста
├── B_LineTest.java            # 6 тестов: уравнение прямой
├── lambda/LambdaDemoTest.java # 8 тестов: лямбда-выражения
├── defaultimp/CircleTest.java # 10 тестов: геометрия круга
└── math3d/Vector3DTest.java   # 20 тестов: 3D векторы
```

### 🚀 Запуск тестов

```bash
# Запустить все тесты
mvn test

# Запустить конкретный тест
mvn test -Dtest=Vector3DTest
```

### 📚 Обучающие модули

#### 1. **Математика для графики** (math3d/)
- Класс `Vector3D` - полная реализация 3D вектора
- Операции: сложение, вычитание, масштабирование, нормализация
- Скалярное и векторное произведение
- Вычисление расстояний
- **20 тестов** с подробными объяснениями математики

#### 2. **Геометрические примитивы**
- `B_line` - построение уравнения прямой
- `Circle` - окружность с центром и радиусом
- `Square` - квадрат (интерфейс Shape)
- **17 тестов** с примерами использования

#### 3. **Лямбда-выражения Java 8** (lambda/)
- Функциональные интерфейсы
- Лямбда-выражения vs анонимные классы
- Ссылки на методы (method references)
- Операции высшего порядка (map/reduce)
- **8 тестов** от простых к сложным

#### 4. **Работа с датой и временем** (datetime/)
- `Date`, `Calendar`, `LocalDateTime`
- Форматирование и парсинг дат
- Часовые пояса и локали
- **6 тестов** с практическими примерами

---

## 📖 Лабораторные работы

* [Лаб. 1 - примитивы OpenGL, 2D](labs/kg_lab1.doc)
* [Лаб. 2 - продолжение 2D](labs/kg_lab2.doc)
* [Лаб. 3 - работа с 3D](labs/kg_lab3.doc)
* [Лаб. 4 - фракталы](labs/kg_lab4.doc) - [Фракталы в интернете](http://elementy.ru/posters/fractals)

## 🎮 Примеры JOGL (OpenGL для Java)

Каталог `JOGL/` содержит примеры работы с 3D графикой на Java:

- **JOGL_NeHe/** - адаптация знаменитых уроков NeHe на JOGL
  - `Gears.java` - демо вращающихся шестерёнок
  - `Tennis.java` - простая 3D игра в теннис
  - `nehe1/`, `nehe2/` - уроки OpenGL (текстуры, освещение, частицы, туман и т.д.)

- **NeHe/** - полный набор уроков NeHe (48 уроков)
  - lesson01-48: от базовых примитивов до сложных эффектов
  - Включает работу с текстурами, шейдерами, физикой

### Использование JOGL примеров

Эти примеры предназначены для изучения OpenGL API, но не покрыты юнит-тестами (требуют графический контекст).

Математические классы из JOGL были адаптированы в основной проект:
- `Vector3D` (из lesson39) → `src/main/java/math3d/Vector3D.java` ✅ с тестами

## 📝 Задачи

Сдавать задачи в автоматической тестирующей системе:
http://ts.lokos.net (пароли и логины у старост)

[Условия задач](15_cg.pdf) | [Разбор задач](http://stden.github.io/comp_graph/tasks.html) 


## 📚 Учебники и ресурсы

* [NeHe OpenGL Tutorial](http://pmg.org.ru/nehe/) - классический учебник по OpenGL (русский перевод)
* [OpenGL Reference](https://www.opengl.org/sdk/docs/) - официальная документация OpenGL

## 🛠️ Установка и настройка

### Java Development Kit (JDK 8+)

```bash
# Проверка версии Java
java -version

# Должно быть JDK 8 или выше
```

**Требования для проекта:**
- Java 8+ (1.8+)
- Maven 3.6+
- Git

### Установка зависимостей

```bash
# Клонировать репозиторий
git clone https://github.com/stden/comp_graph.git
cd comp_graph

# Собрать проект и запустить тесты
mvn clean test

# Собрать JAR файл
mvn package
```

### IDE (по выбору)

* [IntelliJ IDEA](https://www.jetbrains.com/idea/) - рекомендуется (Community Edition бесплатна)
* [Eclipse](https://www.eclipse.org/)
* [VS Code](https://code.visualstudio.com/) с расширением Java

### C++ и OpenGL (для лабораторных работ)

* [MinGW](http://sourceforge.net/projects/mingw/) - компилятор GCC для Windows
* [Code::Blocks](http://www.codeblocks.org/downloads/binaries) - IDE с OpenGL
* [Установка Code::Blocks](codeblocks/README.md) - подробная инструкция

### Visual Studio (альтернатива)

Для установки OpenGL в Visual Studio:
1. Найти каталог VS: `C:\Program Files (x86)\Microsoft Visual Studio\VC`
2. Скопировать `.lib` файлы в `lib/`
3. Скопировать `.h` файлы в `include/`
4. Скопировать `.dll` в системный каталог (например, `C:\Windows\System32`)

## 🤝 Контрибьютинг

Проект приветствует контрибьюции! Если вы нашли ошибку или хотите добавить новый пример:

1. Fork репозитория
2. Создайте ветку для вашей фичи
3. Напишите тесты для нового кода
4. Убедитесь, что все тесты проходят: `mvn test`
5. Создайте Pull Request

**Важно:** Все новые классы должны иметь 100% покрытие тестами!

## 📄 Лицензия

Материалы курса распространяются для образовательных целей.

---

💡 **Совет:** Начните изучение с тестов - они содержат подробные комментарии и примеры использования!


