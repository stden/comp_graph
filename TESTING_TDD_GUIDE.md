# TESTING_TDD_GUIDE.md

## Purpose

This repository should follow a simple rule:

**Ship behavior with tests, not hope.**

The goal is not to maximize test count. The goal is to maximize confidence while keeping feedback fast.

---

## Default Testing Strategy

Use the testing pyramid:

- **70% unit tests** — pure logic, fast feedback, no network/DB/filesystem
- **20% integration tests** — module boundaries, DB, queues, APIs, CLI boundaries
- **10% end-to-end tests** — real user flows, critical paths only

If E2E tests start to outnumber unit tests, the pyramid is upside down.

---

## TDD Policy

Use **Red → Green → Refactor** for all new business logic and bug fixes.

### Red
Write a failing test that describes the expected behavior.

### Green
Write the minimum code needed to make the test pass.

### Refactor
Clean up names, structure, duplication, and complexity while keeping tests green.

### Rule for bug fixes
Every bug fix should include:
1. a regression test that fails before the fix
2. the implementation change
3. all tests green after the fix

---

## What to Test

### Unit tests should cover
- pure functions
- parsers and validators
- business rules
- edge cases and boundary values
- error handling branches
- serialization / transformation logic

### Integration tests should cover
- DB repositories and migrations
- HTTP handlers and API contracts
- message queues / cron jobs / workers
- file IO boundaries
- third-party integrations using fakes, mocks, or test environments

### E2E tests should cover only critical flows
- login / auth
- checkout / payment / subscription
- create / update / delete core entities
- import / export / sync workflows
- deployment smoke checks

---

## Best Practices

### 1. Prefer AAA
Structure tests as:
- **Arrange**
- **Act**
- **Assert**

### 2. Test behavior, not implementation
Do not assert private/internal implementation details unless absolutely necessary.

### 3. Keep tests deterministic
A test must give the same result every run.
Avoid:
- random state without a seed
- real wall-clock timing assumptions
- network calls in unit tests
- shared mutable state between tests

### 4. Mock boundaries, not your own logic
Mock:
- external APIs
- DB in unit tests
- filesystem
- time / clock
- environment variables when needed

Do **not** mock core domain logic just to make tests pass.

### 5. No `sleep()` in tests
Use retries, polling, explicit waits, or event-based assertions.

### 6. One logical behavior per test
Small focused tests fail more clearly and are easier to maintain.

### 7. Use parameterized tests for variants
If the same behavior must be checked across many inputs, use table-driven / parameterized tests.

### 8. Keep CI strict on the important parts
At minimum CI should block merges on:
- test failures
- syntax / compile failures
- formatting drift if formatter is enforced
- linter failures for agreed rules

---

## Coverage Guidance

Coverage is a signal, not the goal.

Recommended targets:
- **80%+ line coverage** for core business logic
- **90%+ branch coverage** for critical logic (auth, payments, permissions, pricing)
- **100% coverage is not required** if it creates low-value tests

Never write pointless tests just to raise the number.

---

## Repository Rules

1. New features should include tests.
2. Bug fixes should include regression tests.
3. Flaky tests must be fixed or removed quickly.
4. Skipped tests must have a linked issue or clear reason.
5. Test data should be explicit and readable.
6. Tests should run locally and in CI.
7. Prefer fast tests close to the code.

---

## Suggested Test Layout

Use the closest idiomatic version for the stack in this repo.

```text
src/
  module_x/
    ...
    module_x.test.*      # unit tests close to source when idiomatic

tests/
  integration/
  e2e/
  fixtures/
  helpers/
```

---

## Language / Stack Hints

### Python
- Prefer `pytest`
- Use `@pytest.mark.parametrize` for variants
- Use fixtures for setup, not giant inline setup blocks
- Keep unit tests away from live external services

### Rust
- Use `cargo test`
- Put small unit tests near the code with `#[cfg(test)]`
- Put integration tests in `tests/`
- Prefer small deterministic tests over giant golden-path suites

### JavaScript / TypeScript
- Prefer Vitest or Jest for unit/integration
- Prefer Playwright for E2E
- Mock HTTP/network boundaries, not internal functions

### PHP
- Prefer PHPUnit
- Test services and domain classes directly
- Keep controller tests thin and integration-oriented

### Java / Kotlin
- Prefer JUnit 5
- Use table-driven patterns where possible
- Keep Spring / framework-heavy tests for integration level only

---

## CI Minimum Standard

Every repository should aim to have these jobs where relevant:

1. **format / lint**
2. **unit tests**
3. **integration tests**
4. **build / package**
5. **smoke or E2E** for critical apps only

Recommended order:
- fast checks first
- expensive checks later
- fail fast on syntax / compile / formatter issues

---

## Anti-Patterns to Avoid

- testing implementation instead of behavior
- giant brittle E2E suites
- hidden magic test setup
- copy-pasted tests
- asserting logs instead of outcomes
- real credentials in tests
- non-isolated tests
- ignored flaky failures in CI
- merging broken test suites

---

## Definition of Done

A change is not done until:
- code works
- tests prove it
- CI is green
- the next person can understand the intent

---

## Sources Behind This Guide

This guide is distilled from established testing practice:
- testing pyramid
- TDD / red-green-refactor
- unit / integration / E2E separation
- deterministic and maintainable CI-friendly tests

It was added as a repository-wide baseline so future changes converge on one testing standard.
