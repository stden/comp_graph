#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Генератор LaTeX файла для множества Мандельброта
Использует TikZ для отрисовки пикселей
"""

def mandelbrot(c, max_iter=100):
    """Вычисление принадлежности точки множеству Мандельброта"""
    z = 0
    for n in range(max_iter):
        if abs(z) > 2:
            return n
        z = z*z + c
    return max_iter

def hsv_to_rgb(h, s, v):
    """Конвертация HSV в RGB"""
    h = h % 360
    c = v * s
    x = c * (1 - abs((h / 60) % 2 - 1))
    m = v - c

    if h < 60:
        r, g, b = c, x, 0
    elif h < 120:
        r, g, b = x, c, 0
    elif h < 180:
        r, g, b = 0, c, x
    elif h < 240:
        r, g, b = 0, x, c
    elif h < 300:
        r, g, b = x, 0, c
    else:
        r, g, b = c, 0, x

    return int((r + m) * 100), int((g + m) * 100), int((b + m) * 100)

def generate_mandelbrot_tikz(width=200, height=150, max_iter=100):
    """Генерация TikZ кода для множества Мандельброта"""

    # Область комплексной плоскости (центр множества)
    xmin, xmax = -2.0, 0.5
    ymin, ymax = -1.25, 1.25

    tikz_code = []
    tikz_code.append("  % Множество Мандельброта")

    # Шаг пикселя
    step_x = (xmax - xmin) / width
    step_y = (ymax - ymin) / height

    # Генерация точек
    for i in range(width):
        for j in range(height):
            real = xmin + i * step_x
            imag = ymin + j * step_y
            c = complex(real, imag)
            iterations = mandelbrot(c, max_iter)

            # Координаты в TikZ (масштаб 0-10 по x, 0-7.5 по y)
            tx = i / width * 10
            ty = j / height * 7.5
            tw = 10 / width
            th = 7.5 / height

            if iterations == max_iter:
                # Точка принадлежит множеству - черная
                tikz_code.append(f"  \\fill[black] ({tx:.3f},{ty:.3f}) rectangle +({tw:.3f},{th:.3f});")
            else:
                # Раскраска по итерациям
                t = iterations / max_iter
                hue = t * 360
                r, g, b = hsv_to_rgb(hue, 1.0, min(1.0, t * 2))
                color = f"{{rgb,100:{r},{g},{b}}}"
                tikz_code.append(f"  \\fill[{color}] ({tx:.3f},{ty:.3f}) rectangle +({tw:.3f},{th:.3f});")

    return "\n".join(tikz_code)

def generate_mandelbrot_latex(width=200, height=150, max_iter=100):
    """Генерация LaTeX документа для множества Мандельброта"""

    latex = []
    latex.append("% Множество Мандельброта")
    latex.append("% Сгенерировано автоматически")
    latex.append("\\documentclass[tikz,border=10pt]{standalone}")
    latex.append("\\usepackage[utf8]{inputenc}")
    latex.append("\\usepackage[russian]{babel}")
    latex.append("\\usepackage{xcolor}")
    latex.append("")
    latex.append("\\begin{document}")
    latex.append("\\begin{tikzpicture}[scale=0.8]")

    # Генерация фрактала
    print("Генерация множества Мандельброта...")
    tikz_fractal = generate_mandelbrot_tikz(width, height, max_iter)
    latex.append(tikz_fractal)

    # Рамка и подписи
    latex.append("  ")
    latex.append("  % Рамка и подписи")
    latex.append("  \\draw[thick] (0,0) rectangle (10,7.5);")
    latex.append("  \\node[below] at (5,0) {\\textbf{Re}};")
    latex.append("  \\node[left, rotate=90] at (0,3.75) {\\textbf{Im}};")
    latex.append("  \\node[above, font=\\Large\\bfseries] at (5,7.5) {Множество Мандельброта};")
    latex.append(f"  \\node[below, font=\\small] at (5,0) {{Итераций: {max_iter}}};")
    latex.append("\\end{tikzpicture}")
    latex.append("\\end{document}")

    return "\n".join(latex)

if __name__ == "__main__":
    # Генерация файла (небольшое разрешение для LaTeX)
    latex_content = generate_mandelbrot_latex(width=150, height=100, max_iter=80)

    with open("mandelbrot-set.tex", "w", encoding="utf-8")  as f:
        f.write(latex_content)

    print("✓ Файл mandelbrot-set.tex создан")
    print("  Компиляция: pdflatex mandelbrot-set.tex")
    print("  ВНИМАНИЕ: Компиляция может занять несколько минут!")
