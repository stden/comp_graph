#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Генератор LaTeX файла для дерева Пифагора
Использует рекурсивные макросы TikZ
"""

import math

def generate_square(x, y, size, angle, depth, max_depth):
    """Генерация TikZ кода для квадрата дерева Пифагора"""

    if depth > max_depth or size < 0.01:
        return []

    lines = []

    # Цвет зависит от глубины
    t = depth / max_depth
    r = int((0.2 + t * 0.5) * 100)
    g = int((0.6 - t * 0.3) * 100)
    b = 20

    color = f"{{rgb,100:{r},{g},{b}}}"

    # Вычисляем вершины квадрата
    cos_a = math.cos(angle)
    sin_a = math.sin(angle)

    p1 = (x, y)
    p2 = (x + size * cos_a, y + size * sin_a)
    p3 = (x + size * cos_a - size * sin_a, y + size * sin_a + size * cos_a)
    p4 = (x - size * sin_a, y + size * cos_a)

    # Рисуем квадрат
    lines.append(f"  \\filldraw[fill={color}, draw=brown!50!black, line width=0.2pt]")
    lines.append(f"    ({p1[0]:.3f},{p1[1]:.3f}) --")
    lines.append(f"    ({p2[0]:.3f},{p2[1]:.3f}) --")
    lines.append(f"    ({p3[0]:.3f},{p3[1]:.3f}) --")
    lines.append(f"    ({p4[0]:.3f},{p4[1]:.3f}) -- cycle;")

    # Рекурсивно рисуем два дочерних квадрата
    branch_angle = math.pi / 6  # 30 градусов
    length_ratio = math.cos(branch_angle)
    new_size = size * length_ratio

    # Левая ветвь
    left_x = p4[0]
    left_y = p4[1]
    left_angle = angle + branch_angle

    # Правая ветвь
    right_x = p3[0]
    right_y = p3[1]
    right_angle = angle - branch_angle

    lines.extend(generate_square(left_x, left_y, new_size, left_angle, depth + 1, max_depth))
    lines.extend(generate_square(right_x, right_y, new_size, right_angle, depth + 1, max_depth))

    return lines

def generate_pythagoras_tree_latex(max_depth=10, base_size=2.0):
    """Генерация LaTeX документа для дерева Пифагора"""

    latex = []
    latex.append("% Дерево Пифагора")
    latex.append("% Сгенерировано автоматически")
    latex.append("\\documentclass[tikz,border=10pt]{standalone}")
    latex.append("\\usepackage[utf8]{inputenc}")
    latex.append("\\usepackage[russian]{babel}")
    latex.append("\\usepackage{xcolor}")
    latex.append("")
    latex.append("\\begin{document}")
    latex.append("\\begin{tikzpicture}[scale=1.0]")
    latex.append("  % Дерево Пифагора - рекурсивная структура из квадратов")

    # Генерация дерева
    tree_lines = generate_square(0, 0, base_size, 0, 0, max_depth)
    latex.extend(tree_lines)

    latex.append("  ")
    latex.append("  % Заголовок")
    latex.append("  \\node[below, font=\\Large\\bfseries, text=brown!70!black] at (current bounding box.south)")
    latex.append("    {Дерево Пифагора};")
    latex.append(f"  \\node[above, font=\\small] at (current bounding box.north)")
    latex.append(f"    {{Глубина рекурсии: {max_depth}}};")
    latex.append("\\end{tikzpicture}")
    latex.append("\\end{document}")

    return "\n".join(latex)

if __name__ == "__main__":
    # Генерация файла
    latex_content = generate_pythagoras_tree_latex(max_depth=10, base_size=2.0)

    with open("pythagoras-tree-generated.tex", "w", encoding="utf-8") as f:
        f.write(latex_content)

    print("✓ Файл pythagoras-tree-generated.tex создан")
    print("  Компиляция: pdflatex pythagoras-tree-generated.tex")
