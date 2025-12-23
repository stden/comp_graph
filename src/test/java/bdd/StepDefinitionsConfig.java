package bdd;

import io.cucumber.java.ParameterType;

/**
 * Custom parameter type mappings shared across step definitions.
 */
public class StepDefinitionsConfig {
    @ParameterType("[-+]?\\d+(?:[\\.,]\\d+)?")
    public Float floatNumber(String input) {
        return Float.parseFloat(input.replace(',', '.'));
    }

    @ParameterType("[-+]?\\d+(?:[\\.,]\\d+)?")
    public Double doubleNumber(String input) {
        return Double.parseDouble(input.replace(',', '.'));
    }

    @ParameterType("пересекает|не пересекает")
    public String intersectionResult(String input) {
        return input;
    }
}
