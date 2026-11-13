package graphics;

/**
 * Класс для работы с цветовыми моделями RGB и HSV
 *
 * Модуль 1: Основы компьютерной графики
 * - Цветовая модель RGB: Red (0-255), Green (0-255), Blue (0-255)
 * - Цветовая модель HSV: Hue (0-360°), Saturation (0-100%), Value (0-100%)
 */
public class ColorModel {

    /**
     * RGB цвет: красный, зелёный, синий (0-255)
     */
    public static class RGB {
        public final int r, g, b;

        public RGB(int r, int g, int b) {
            this.r = clamp(r, 0, 255);
            this.g = clamp(g, 0, 255);
            this.b = clamp(b, 0, 255);
        }

        /**
         * Преобразование RGB в HSV
         * Алгоритм из компьютерной графики
         */
        public HSV toHSV() {
            float r = this.r / 255.0f;
            float g = this.g / 255.0f;
            float b = this.b / 255.0f;

            float max = Math.max(r, Math.max(g, b));
            float min = Math.min(r, Math.min(g, b));
            float delta = max - min;

            // Вычисление Hue (оттенок)
            float h = 0;
            if (delta != 0) {
                if (max == r) {
                    h = 60 * (((g - b) / delta) % 6);
                } else if (max == g) {
                    h = 60 * (((b - r) / delta) + 2);
                } else {
                    h = 60 * (((r - g) / delta) + 4);
                }
            }
            if (h < 0) h += 360;

            // Вычисление Saturation (насыщенность)
            float s = (max == 0) ? 0 : (delta / max) * 100;

            // Вычисление Value (яркость)
            float v = max * 100;

            return new HSV(h, s, v);
        }

        /**
         * Создание серого цвета (grayscale)
         */
        public RGB toGrayscale() {
            // Формула яркости по стандарту ITU-R BT.709
            int gray = (int)(0.2126 * r + 0.7152 * g + 0.0722 * b);
            return new RGB(gray, gray, gray);
        }

        @Override
        public String toString() {
            return String.format("RGB(%d, %d, %d)", r, g, b);
        }

        @Override
        public boolean equals(Object obj) {
            if (!(obj instanceof RGB)) return false;
            RGB other = (RGB) obj;
            return r == other.r && g == other.g && b == other.b;
        }

        @Override
        public int hashCode() {
            return (r << 16) | (g << 8) | b;
        }
    }

    /**
     * HSV цвет: оттенок, насыщенность, яркость
     */
    public static class HSV {
        public final float h, s, v;

        public HSV(float h, float s, float v) {
            this.h = h % 360;
            this.s = clamp(s, 0, 100);
            this.v = clamp(v, 0, 100);
        }

        /**
         * Преобразование HSV в RGB
         * Алгоритм из компьютерной графики
         */
        public RGB toRGB() {
            float s = this.s / 100.0f;
            float v = this.v / 100.0f;

            float c = v * s;
            float x = c * (1 - Math.abs(((h / 60) % 2) - 1));
            float m = v - c;

            float r1 = 0, g1 = 0, b1 = 0;

            if (h >= 0 && h < 60) {
                r1 = c; g1 = x; b1 = 0;
            } else if (h >= 60 && h < 120) {
                r1 = x; g1 = c; b1 = 0;
            } else if (h >= 120 && h < 180) {
                r1 = 0; g1 = c; b1 = x;
            } else if (h >= 180 && h < 240) {
                r1 = 0; g1 = x; b1 = c;
            } else if (h >= 240 && h < 300) {
                r1 = x; g1 = 0; b1 = c;
            } else {
                r1 = c; g1 = 0; b1 = x;
            }

            int r = (int)((r1 + m) * 255);
            int g = (int)((g1 + m) * 255);
            int b = (int)((b1 + m) * 255);

            return new RGB(r, g, b);
        }

        @Override
        public String toString() {
            return String.format("HSV(%.1f°, %.1f%%, %.1f%%)", h, s, v);
        }

        @Override
        public boolean equals(Object obj) {
            if (!(obj instanceof HSV)) return false;
            HSV other = (HSV) obj;
            return Math.abs(h - other.h) < 0.01f &&
                   Math.abs(s - other.s) < 0.01f &&
                   Math.abs(v - other.v) < 0.01f;
        }
    }

    private static int clamp(int value, int min, int max) {
        return Math.max(min, Math.min(max, value));
    }

    private static float clamp(float value, float min, float max) {
        return Math.max(min, Math.min(max, value));
    }
}
