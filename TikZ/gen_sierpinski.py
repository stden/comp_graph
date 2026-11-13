#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Генератор LaTeX файла для треугольника Серпинского
Использует L-систему (Lindenmayer system) TikZ
"""

def generate_sierpinski_latex(order=8, step='2pt', angle=60):
    """Генерация LaTeX документа для треугольника Серпинского"""

    latex = []
    latex.append("% Треугольник Серпинского")
    latex.append("% Сгенерировано автоматически")
    latex.append("\\documentclass[tikz,border=10pt]{standalone}")
    latex.append("\\usepackage[utf8]{inputenc}")
    latex.append("\\usepackage[russian]{babel}")
    latex.append("\\usetikzlibrary{lindenmayersystems}")
    latex.append("\\usetikzlibrary{shadings}")
    latex.append("")
    latex.append("% Определение L-системы для треугольника Серпинского")
    latex.append("\\pgfdeclarelindenmayersystem{Sierpinski triangle}{")
    latex.append("  \\rule{F -> G-F-G}")
    latex.append("  \\rule{G -> F+G+F}}")
    latex.append("")
    latex.append("\\begin{document}")
    latex.append("\\begin{tikzpicture}")
    latex.append("  % Треугольник Серпинского с градиентом")
    latex.append(f"  \\shadedraw [top color=white, bottom color=orange!80, draw=orange!80!black]")
    latex.append(f"  [l-system={{Sierpinski triangle, step={step}, angle={angle}, axiom=F, order={order}}}]")
    latex.append("  lindenmayer system -- cycle;")
    latex.append("  ")
    latex.append("  % Заголовок")
    latex.append("  \\node[above, font=\\Large\\bfseries, text=orange!50!black] at (current bounding box.north)")
    latex.append("    {Треугольник Серпинского};")
    latex.append(f"  \\node[below, font=\\small] at (current bounding box.south)")
    latex.append(f"    {{Порядок рекурсии: {order}}};")
    latex.append("\\end{tikzpicture}")
    latex.append("\\end{document}")

    return "\n".join(latex)

if __name__ == "__main__":
    # Генерация файла
    latex_content = generate_sierpinski_latex(order=8, step='2pt', angle=60)

    with open("sierpinski-triangle.tex", "w", encoding="utf-8") as f:
        f.write(latex_content)

    print("✓ Файл sierpinski-triangle.tex создан")
    print("  Компиляция: pdflatex sierpinski-triangle.tex")
