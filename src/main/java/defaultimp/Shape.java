package defaultimp;

/**
 * Java8 - реализация по-умолчанию (default для интерфейсов)
 */
public interface Shape {

    /**
     * @return Имя фигуры
     */
    String getName();

    /**
     * Метод presentation() c реализацией по-умолчанию
     *
     * @return строчку об объекте для вывода пользователю
     */
    default String presentation() {
        return "Реализуйте метод presentation() для класса "
                + getClass().getName() + ": " + getName();
    }
}