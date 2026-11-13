@smoke
Feature: Smoke test for Computer Graphics project
  Verify that core classes are loadable and basic operations work

  Scenario: Vector3D basic arithmetic works
    Given a 3D vector with coordinates 1.0, 2.0, 3.0
    Then the vector length should be approximately 3.742

  Scenario: Fractal renderers are available
    Given the fractal renderer registry
    Then at least one fractal renderer should be available
