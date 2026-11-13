# TESTING_CHECKLIST_JAVA.md

## Before merge
- [ ] `mvn test` / `gradle test` passes
- [ ] Build passes
- [ ] New logic has tests
- [ ] Bug fixes include regression tests
- [ ] Critical service / persistence boundaries covered by integration tests

## Java-specific best practices
- [ ] Prefer JUnit 5
- [ ] Use parameterized tests for variants
- [ ] Keep Spring / framework-heavy tests at integration level
- [ ] Unit-test business logic without full application context
- [ ] Mock external services, not domain logic
- [ ] Avoid slow context boot unless needed
- [ ] Test validation and error responses explicitly
- [ ] Keep tests deterministic and independent

## CI minimum
- [ ] build
- [ ] unit tests
- [ ] integration tests
- [ ] static analysis if configured
