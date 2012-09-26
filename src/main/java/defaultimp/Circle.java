package defaultimp;

/**
 * Круг
 */
public class Circle implements Shape {

    private final double x, y, radius;

    /**
     * Круг
     *
     * @param x      x-координата центра
     * @param y      y-координата центра
     * @param radius радиус
     */
    public Circle(double x, double y, double radius) {
        this.x = x;
        this.y = y;
        this.radius = radius;
    }


    @Override
    public String getName() {
        return "Круг";
    }

    @Override
    public String presentation() {
        return String.format("%s  центр: (%s; %s)  радиус = %s", getName(), x, y, radius);
    }
}
