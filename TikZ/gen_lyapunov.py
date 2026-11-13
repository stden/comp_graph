#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Генератор LaTeX файла для фрактала Ляпунова
Использует TikZ для отрисовки пикселей
"""


import math

def compute_lyapunov(rA, rB, sequence, warmup=100, compute_iter=100):
    """Вычисление показателя Ляпунова для логистического отображения"""

    x = 0.5
    lyapunov_sum = 0.0

    # Warmup - стабилизация орбиты
    for i in range(warmup):
        c = sequence[i % len(sequence)]
        r = rA if c == 'A' else rB
        x = r * x * (1.0 - x)

        if x <= 0.0 or x >= 1.0 or math.isnan(x):
            return float('-inf')

    # Вычисление показателя
    for i in range(compute_iter):
        c = sequence[i % len(sequence)]
        r = rA if c == 'A' else rB

        x = r * x * (1.0 - x)

        if x <= 0.0 or x >= 1.0 or math.isnan(x):
            return float('-inf')

        derivative = abs(r * (1.0 - 2.0 * x))

        if derivative > 0:
            lyapunov_sum += math.log(derivative)

    return lyapunov_sum / compute_iter

def lyapunov_to_color(lyapunov):
    """Цветовая схема для показателя Ляпунова"""

    if math.isinf(lyapunov) and lyapunov < 0:
        return "black"

    # Нормализация
    normalized = max(-1.0, min(1.0, lyapunov))

    if normalized < 0:
        # Отрицательный - устойчивость (синий)
        t = abs(normalized)
        blue = int(50 + t * 50)
        green = int(t * 30)
        return f"{{rgb,100:0,{green},{blue}}}"
    else:
        # Положительный - хаос (красный)
        t = normalized
        red = int(70 + t * 30)
        green = int(t * 50)
        return f"{{rgb,100:{red},{green},0}}"

def generate_lyapunov_tikz(sequence="AB", width=100, height=100,
                          rA_min=2.0, rA_max=4.0, rB_min=2.0, rB_max=4.0):
    """Генерация TikZ кода для фрактала Ляпунова"""

    tikz_code = []
    tikz_code.append(f"  % Фрактал Ляпунова для последовательности {sequence}")

    step_rA = (rA_max - rA_min) / width
    step_rB = (rB_max - rB_min) / height

    # Генерация точек
    for i in range(width):
        for j in range(height):
            rA = rA_min + i * step_rA
            rB = rB_min + j * step_rB

            lyapunov = compute_lyapunov(rA, rB, sequence, warmup=50, compute_iter=50)

            # Координаты в TikZ
            tx = i / width * 10
            ty = j / height * 10
            tw = 10 / width
            th = 10 / height

            color = lyapunov_to_color(lyapunov)
            tikz_code.append(f"  \\fill[{color}] ({tx:.3f},{ty:.3f}) rectangle +({tw:.3f},{th:.3f});")

    return "\n".join(tikz_code)

def generate_lyapunov_latex(sequence="AB", width=80, height=80):
    """Генерация LaTeX документа для фрактала Ляпунова"""

    latex = []
    latex.append("% Фрактал Ляпунова")
    latex.append("% Сгенерировано автоматически")
    latex.append("\\documentclass[tikz,border=10pt]{standalone}")
    latex.append("\\usepackage[utf8]{inputenc}")
    latex.append("\\usepackage[russian]{babel}")
    latex.append("\\usepackage{xcolor}")
    latex.append("")
    latex.append("\\begin{document}")
    latex.append("\\begin{tikzpicture}[scale=0.8]")

    # Генерация фрактала
    print(f"Генерация фрактала Ляпунова для последовательности '{sequence}'...")
    tikz_fractal = generate_lyapunov_tikz(sequence, width, height)
    latex.append(tikz_fractal)

    # Рамка и подписи
    latex.append("  ")
    latex.append("  % Рамка и подписи")
    latex.append("  \\draw[thick] (0,0) rectangle (10,10);")
    latex.append("  \\node[below] at (5,0) {\\textbf{$r_A$}};")
    latex.append("  \\node[left, rotate=90] at (0,5) {\\textbf{$r_B$}};")
    latex.append("  \\node[above, font=\\Large\\bfseries] at (5,10) {Фрактал Ляпунова};")
    latex.append(f"  \\node[below, font=\\small] at (5,0) {{Последовательность: {sequence}}};")
    latex.append("")
    latex.append("  % Легенда")
    latex.append("  \\node[right, font=\\scriptsize, align=left] at (10.2, 8)")
    latex.append("    {Синий:\\\\устойчивость};")
    latex.append("  \\node[right, font=\\scriptsize, align=left] at (10.2, 6)")
    latex.append("    {Красный:\\\\хаос};")
    latex.append("\\end{tikzpicture}")
    latex.append("\\end{document}")

    return "\n".join(latex)

if __name__ == "__main__":
    # Генерация файла
    sequence = "AABAB"  # Интересная последовательность
    latex_content = generate_lyapunov_latex(sequence=sequence, width=60, height=60)

    with open("lyapunov-fractal.tex", "w", encoding="utf-8") as f:
        f.write(latex_content)

    print("✓ Файл lyapunov-fractal.tex создан")
    print("  Компиляция: pdflatex lyapunov-fractal.tex")
    print("  ВНИМАНИЕ: Компиляция может занять несколько минут!")
