import org.junit.Assert;
import org.junit.Test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class A_VectorTest extends Assert {
    @Test
    public void testVector() throws IOException {
        doTest("1 2 3", "3.74");
        doTest("4.6 2.7 1.1", "5.45");
    }

    private void doTest(String input, String answer) throws IOException {
        SaveToFile(input, A_Vector.ID + ".in");
        A_Vector.main(null);
        try (Scanner in = new Scanner(new File(A_Vector.ID + ".out"))) {
            String output = in.nextLine();
            assertEquals(answer, output);
        }
    }

    private void SaveToFile(String text, String fileName) throws FileNotFoundException {
        try (PrintWriter pw = new PrintWriter(fileName)) {
            pw.println(text);
        }
    }
}
