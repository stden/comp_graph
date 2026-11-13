package bdd;

import io.cucumber.java.en.Given;
import io.cucumber.java.en.Then;
import math3d.Vector3D;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

public class SmokeSteps {

    private Vector3D vector;

    @Given("a 3D vector with coordinates {double}, {double}, {double}")
    public void a3DVectorWithCoordinates(double x, double y, double z) {
        vector = new Vector3D((float) x, (float) y, (float) z);
        assertNotNull("Vector should be created", vector);
    }

    @Then("the vector length should be approximately {double}")
    public void theVectorLengthShouldBeApproximately(double expectedLength) {
        double length = Math.sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
        assertTrue("Vector length should be close to " + expectedLength,
                Math.abs(length - expectedLength) < 0.01);
    }

    @Given("the fractal renderer registry")
    public void theFractalRendererRegistry() {
        try {
            Class.forName("fractals.core.Fractal");
        } catch (ClassNotFoundException e) {
            throw new AssertionError("Fractal core class should be loadable", e);
        }
    }

    @Then("at least one fractal renderer should be available")
    public void atLeastOneFractalRendererShouldBeAvailable() {
        try {
            Class.forName("fractals.renderers.MandelbrotSet");
        } catch (ClassNotFoundException e) {
            throw new AssertionError("MandelbrotSet renderer should be available", e);
        }
    }
}
