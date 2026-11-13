# Project Instructions

This is a Java project. Below are the key operational instructions for development and maintenance.

## TL;DR (Most Common)

```bash
# Build project
mvn clean install

# Run tests
mvn test

# Run application
mvn spring-boot:run  # or mvn quarkus:dev for Quarkus
```

## 1. Project Description

Brief overview of this project's purpose and scope.

## 2. Development Setup

### Prerequisites
- Java 11+ (or Java version specified in pom.xml)
- Maven 3.8+

### Installation
```bash
# Build project
mvn clean install

# Skip tests during build
mvn clean install -DskipTests
```

### Running Tests
```bash
# Run all tests
mvn test

# Run specific test
mvn test -Dtest=ClassName
```

## 3. Code Quality

### Run checks
```bash
mvn clean verify
```

### Format code
```bash
mvn spotless:apply
```

### Checkstyle
```bash
mvn checkstyle:check
```

## 4. Common Tasks

- **Add dependency**: Edit `pom.xml` and run `mvn dependency:resolve`
- **Run application**: `mvn spring-boot:run` or `mvn quarkus:dev`
- **Build JAR**: `mvn clean package`
- **View dependency tree**: `mvn dependency:tree`

## 5. CI/CD

This project uses GitHub Actions for CI/CD. Workflows are defined in `.github/workflows/`.

## 6. Repository Rules

- Use conventional commits: `feat:`, `fix:`, `docs:`, `refactor:`, `test:`, `chore:`
- Keep commits focused and descriptive
- Ensure tests pass before pushing
- Update pom.xml when adding dependencies

## Questions?

Refer to the project README.md for more details.
