package fractals.web;

import org.springframework.web.bind.annotation.*;

import java.util.HashMap;
import java.util.Map;

/**
 * REST API контроллер для генерации фрактальных данных
 * REST API controller for fractal data generation
 */
@RestController
@RequestMapping("/api/fractals")
@CrossOrigin(origins = "*")
public class FractalController {

    /**
     * Получить список доступных фракталов
     */
    @GetMapping("/list")
    public Map<String, Object> getFractalList() {
        Map<String, Object> response = new HashMap<>();
        String[] fractals = {
            "mandelbrot",
            "julia",
            "sierpinski",
            "koch",
            "pythagoras",
            "landscape",
            "lyapunov"
        };

        Map<String, String> descriptions = new HashMap<>();
        descriptions.put("mandelbrot", "Множество Мандельброта");
        descriptions.put("julia", "Множество Жюлиа");
        descriptions.put("sierpinski", "Треугольник Серпинского");
        descriptions.put("koch", "Снежинка Коха");
        descriptions.put("pythagoras", "Дерево Пифагора");
        descriptions.put("landscape", "Фрактальный ландшафт");
        descriptions.put("lyapunov", "Фрактал Ляпунова");

        response.put("fractals", fractals);
        response.put("descriptions", descriptions);
        return response;
    }

    /**
     * Генерация данных множества Мандельброта
     */
    @PostMapping("/mandelbrot")
    public Map<String, Object> generateMandelbrot(@RequestBody MandelbrotRequest request) {
        int width = request.width;
        int height = request.height;
        double xMin = request.xMin;
        double xMax = request.xMax;
        double yMin = request.yMin;
        double yMax = request.yMax;
        int maxIterations = request.maxIterations;

        int[] pixels = new int[width * height];

        for (int py = 0; py < height; py++) {
            for (int px = 0; px < width; px++) {
                double x0 = xMin + (xMax - xMin) * px / width;
                double y0 = yMin + (yMax - yMin) * py / height;

                double x = 0.0;
                double y = 0.0;
                int iteration = 0;

                while (x * x + y * y <= 4.0 && iteration < maxIterations) {
                    double xTemp = x * x - y * y + x0;
                    y = 2.0 * x * y + y0;
                    x = xTemp;
                    iteration++;
                }

                pixels[py * width + px] = iteration;
            }
        }

        Map<String, Object> response = new HashMap<>();
        response.put("width", width);
        response.put("height", height);
        response.put("pixels", pixels);
        response.put("maxIterations", maxIterations);
        return response;
    }

    /**
     * Генерация данных множества Жюлиа
     */
    @PostMapping("/julia")
    public Map<String, Object> generateJulia(@RequestBody JuliaRequest request) {
        int width = request.width;
        int height = request.height;
        double xMin = request.xMin;
        double xMax = request.xMax;
        double yMin = request.yMin;
        double yMax = request.yMax;
        double cReal = request.cReal;
        double cImag = request.cImag;
        int maxIterations = request.maxIterations;

        int[] pixels = new int[width * height];

        for (int py = 0; py < height; py++) {
            for (int px = 0; px < width; px++) {
                double x = xMin + (xMax - xMin) * px / width;
                double y = yMin + (yMax - yMin) * py / height;

                int iteration = 0;

                while (x * x + y * y <= 4.0 && iteration < maxIterations) {
                    double xTemp = x * x - y * y + cReal;
                    y = 2.0 * x * y + cImag;
                    x = xTemp;
                    iteration++;
                }

                pixels[py * width + px] = iteration;
            }
        }

        Map<String, Object> response = new HashMap<>();
        response.put("width", width);
        response.put("height", height);
        response.put("pixels", pixels);
        response.put("maxIterations", maxIterations);
        return response;
    }

    /**
     * Генерация треугольника Серпинского
     */
    @PostMapping("/sierpinski")
    public Map<String, Object> generateSierpinski(@RequestBody SierpinskiRequest request) {
        int depth = request.depth;

        Map<String, Object> response = new HashMap<>();
        response.put("depth", depth);
        response.put("triangles", generateSierpinskiTriangles(0, 0, 1, 0, 0.5, Math.sqrt(3) / 2, depth));
        return response;
    }

    private Object generateSierpinskiTriangles(double x1, double y1, double x2, double y2, double x3, double y3, int depth) {
        java.util.List<Map<String, Double>> triangles = new java.util.ArrayList<>();

        if (depth == 0) {
            Map<String, Double> triangle = new HashMap<>();
            triangle.put("x1", x1);
            triangle.put("y1", y1);
            triangle.put("x2", x2);
            triangle.put("y2", y2);
            triangle.put("x3", x3);
            triangle.put("y3", y3);
            triangles.add(triangle);
        } else {
            double mid1x = (x1 + x2) / 2.0;
            double mid1y = (y1 + y2) / 2.0;
            double mid2x = (x2 + x3) / 2.0;
            double mid2y = (y2 + y3) / 2.0;
            double mid3x = (x3 + x1) / 2.0;
            double mid3y = (y3 + y1) / 2.0;

            triangles.addAll((java.util.Collection<? extends Map<String, Double>>) generateSierpinskiTriangles(x1, y1, mid1x, mid1y, mid3x, mid3y, depth - 1));
            triangles.addAll((java.util.Collection<? extends Map<String, Double>>) generateSierpinskiTriangles(mid1x, mid1y, x2, y2, mid2x, mid2y, depth - 1));
            triangles.addAll((java.util.Collection<? extends Map<String, Double>>) generateSierpinskiTriangles(mid3x, mid3y, mid2x, mid2y, x3, y3, depth - 1));
        }

        return triangles;
    }

    /**
     * Генерация показателей Ляпунова
     */
    @PostMapping("/lyapunov")
    public Map<String, Object> generateLyapunov(@RequestBody LyapunovRequest request) {
        int width = request.width;
        int height = request.height;
        double rAMin = request.rAMin;
        double rAMax = request.rAMax;
        double rBMin = request.rBMin;
        double rBMax = request.rBMax;
        String sequence = request.sequence;

        double[] values = new double[width * height];

        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                double rA = rAMin + (rAMax - rAMin) * i / width;
                double rB = rBMin + (rBMax - rBMin) * j / height;

                values[j * width + i] = computeLyapunov(rA, rB, sequence);
            }
        }

        Map<String, Object> response = new HashMap<>();
        response.put("width", width);
        response.put("height", height);
        response.put("values", values);
        return response;
    }

    private double computeLyapunov(double rA, double rB, String sequence) {
        double x = 0.5;
        double lyapunovSum = 0.0;
        int warmup = 50;
        int compute = 50;

        for (int i = 0; i < warmup; i++) {
            char c = sequence.charAt(i % sequence.length());
            double r = (c == 'A') ? rA : rB;
            x = r * x * (1.0 - x);
            if (x <= 0.0 || x >= 1.0 || Double.isNaN(x)) {
                return Double.NEGATIVE_INFINITY;
            }
        }

        for (int i = 0; i < compute; i++) {
            char c = sequence.charAt(i % sequence.length());
            double r = (c == 'A') ? rA : rB;
            x = r * x * (1.0 - x);
            if (x <= 0.0 || x >= 1.0 || Double.isNaN(x)) {
                return Double.NEGATIVE_INFINITY;
            }
            double derivative = Math.abs(r * (1.0 - 2.0 * x));
            if (derivative > 0) {
                lyapunovSum += Math.log(derivative);
            }
        }

        return lyapunovSum / compute;
    }

    // Request classes
    static class MandelbrotRequest {
        public int width;
        public int height;
        public double xMin;
        public double xMax;
        public double yMin;
        public double yMax;
        public int maxIterations;
    }

    static class JuliaRequest {
        public int width;
        public int height;
        public double xMin;
        public double xMax;
        public double yMin;
        public double yMax;
        public double cReal;
        public double cImag;
        public int maxIterations;
    }

    static class SierpinskiRequest {
        public int depth;
    }

    static class LyapunovRequest {
        public int width;
        public int height;
        public double rAMin;
        public double rAMax;
        public double rBMin;
        public double rBMax;
        public String sequence;
    }
}
