#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Генератор LaTeX файла для фрактального ландшафта
Использует алгоритм Midpoint Displacement и TikZ для 3D изометрической проекции
"""

import random
import math

def midpoint_displacement(size, roughness=0.7, seed=None):
    """Генерация фрактального ландшафта методом Midpoint Displacement"""

    if seed is not None:
        random.seed(seed)

    # Размер должен быть 2^n + 1
    n = int(math.log2(size - 1))
    size = 2**n + 1

    # Создаем пустую карту высот
    heightmap = [[0.0 for _ in range(size)] for _ in range(size)]

    # Инициализация углов
    heightmap[0][0] = 0.5
    heightmap[0][size-1] = 0.5
    heightmap[size-1][0] = 0.5
    heightmap[size-1][size-1] = 0.5

    step_size = size - 1
    scale = roughness

    while step_size > 1:
        half_step = step_size // 2

        # Diamond step
        for x in range(half_step, size, step_size):
            for y in range(half_step, size, step_size):
                avg = (heightmap[x-half_step][y-half_step] +
                      heightmap[x-half_step][y+half_step] +
                      heightmap[x+half_step][y-half_step] +
                      heightmap[x+half_step][y+half_step]) / 4.0

                heightmap[x][y] = avg + (random.random() - 0.5) * scale

        # Square step
        for x in range(0, size, half_step):
            for y in range((x + half_step) % step_size, size, step_size):
                avg = 0
                count = 0

                if x >= half_step:
                    avg += heightmap[x-half_step][y]
                    count += 1
                if x + half_step < size:
                    avg += heightmap[x+half_step][y]
                    count += 1
                if y >= half_step:
                    avg += heightmap[x][y-half_step]
                    count += 1
                if y + half_step < size:
                    avg += heightmap[x][y+half_step]
                    count += 1

                heightmap[x][y] = avg / count + (random.random() - 0.5) * scale

        step_size //= 2
        scale *= 2.0**(-roughness)

    # Нормализация
    min_h = min(min(row) for row in heightmap)
    max_h = max(max(row) for row in heightmap)
    range_h = max_h - min_h

    if range_h > 0:
        for x in range(size):
            for y in range(size):
                heightmap[x][y] = (heightmap[x][y] - min_h) / range_h

    return heightmap

def height_to_color(h):
    """Получить цвет в зависимости от высоты"""
    if h < 0.3:
        # Вода: синий
        return "blue!70"
    elif h < 0.5:
        # Песок: жёлтый
        return "yellow!80!orange"
    elif h < 0.7:
        # Трава: зелёный
        return "green!60!black"
    elif h < 0.85:
        # Горы: коричневый
        return "brown!70"
    else:
        # Снег: белый
        return "white!90!blue"

def generate_landscape_tikz(heightmap, sample_step=2):
    """Генерация TikZ кода для фрактального ландшафта"""

    size = len(heightmap)
    tikz_code = []
    tikz_code.append("  % Фрактальный ландшафт (изометрическая проекция)")

    scale = 8.0 / size  # Масштаб для вписывания в область

    # Отрисовка полигонов (с шагом для уменьшения количества)
    for x in range(0, size - sample_step, sample_step):
        for z in range(0, size - sample_step, sample_step):
            # Высоты вершин квада
            h1 = heightmap[x][z] * 2.0
            h2 = heightmap[x + sample_step][z] * 2.0
            h3 = heightmap[x + sample_step][z + sample_step] * 2.0
            h4 = heightmap[x][z + sample_step] * 2.0

            # Изометрическая проекция
            iso_x1 = (x - z) * scale * 0.5
            iso_y1 = (x + z) * scale * 0.25 + h1

            iso_x2 = (x + sample_step - z) * scale * 0.5
            iso_y2 = (x + sample_step + z) * scale * 0.25 + h2

            iso_x3 = (x + sample_step - z - sample_step) * scale * 0.5
            iso_y3 = (x + sample_step + z + sample_step) * scale * 0.25 + h3

            iso_x4 = (x - z - sample_step) * scale * 0.5
            iso_y4 = (x + z + sample_step) * scale * 0.25 + h4

            # Средняя высота для цвета
            avg_h = (h1 + h2 + h3 + h4) / (4.0 * 2.0)  # Нормализация обратно к [0,1]
            color = height_to_color(avg_h)

            # Рисуем полигон
            tikz_code.append(f"  \\filldraw[fill={color}, draw=black!30, line width=0.1pt]")
            tikz_code.append(f"    ({iso_x1:.2f},{iso_y1:.2f}) --")
            tikz_code.append(f"    ({iso_x2:.2f},{iso_y2:.2f}) --")
            tikz_code.append(f"    ({iso_x3:.2f},{iso_y3:.2f}) --")
            tikz_code.append(f"    ({iso_x4:.2f},{iso_y4:.2f}) -- cycle;")

    return "\n".join(tikz_code)

def generate_landscape_latex(grid_size=33, roughness=0.7, seed=12345, sample_step=2):
    """Генерация LaTeX документа для фрактального ландшафта"""

    latex = []
    latex.append("% Фрактальный ландшафт")
    latex.append("% Сгенерировано автоматически")
    latex.append("\\documentclass[tikz,border=10pt]{standalone}")
    latex.append("\\usepackage[utf8]{inputenc}")
    latex.append("\\usepackage[russian]{babel}")
    latex.append("\\usepackage{xcolor}")
    latex.append("")
    latex.append("\\begin{document}")
    latex.append("\\begin{tikzpicture}[scale=1.0]")

    # Генерация ландшафта
    print(f"Генерация ландшафта {grid_size}x{grid_size}...")
    heightmap = midpoint_displacement(grid_size, roughness, seed)

    print("Создание TikZ кода...")
    tikz_fractal = generate_landscape_tikz(heightmap, sample_step)
    latex.append(tikz_fractal)

    # Заголовок
    latex.append("  ")
    latex.append("  % Заголовок")
    latex.append("  \\node[above, font=\\Large\\bfseries] at (current bounding box.north)")
    latex.append("    {Фрактальный ландшафт};")
    latex.append(f"  \\node[below, font=\\small] at (current bounding box.south)")
    latex.append(f"    {{Алгоритм Midpoint Displacement, roughness={roughness}}};")
    latex.append("\\end{tikzpicture}")
    latex.append("\\end{document}")

    return "\n".join(latex)

if __name__ == "__main__":
    # Генерация файла (небольшая сетка для LaTeX)
    latex_content = generate_landscape_latex(grid_size=33, roughness=0.7, seed=12345, sample_step=2)

    with open("fractal-landscape.tex", "w", encoding="utf-8") as f:
        f.write(latex_content)

    print("✓ Файл fractal-landscape.tex создан")
    print("  Компиляция: pdflatex fractal-landscape.tex")
