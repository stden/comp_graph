package bdd;

import io.cucumber.junit.Cucumber;
import io.cucumber.junit.CucumberOptions;
import org.junit.runner.RunWith;

@RunWith(Cucumber.class)
@CucumberOptions(
        features = "src/test/resources/features",
        glue = "bdd",
        plugin = {"pretty", "html:target/cucumber-reports/cucumber.html"}
)
public class RunCucumberTest {
}
