#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Генератор LaTeX файла для снежинки Коха (Koch Snowflake)
Использует L-систему TikZ
"""

def generate_koch_latex(order=4, step='2pt', angle=60):
    """Генерация LaTeX документа для снежинки Коха"""

    latex = []
    latex.append("% Снежинка Коха")
    latex.append("% Сгенерировано автоматически")
    latex.append("\\documentclass[tikz,border=10pt]{standalone}")
    latex.append("\\usepackage[utf8]{inputenc}")
    latex.append("\\usepackage[russian]{babel}")
    latex.append("\\usetikzlibrary{lindenmayersystems}")
    latex.append("\\usetikzlibrary{shadings}")
    latex.append("")
    latex.append("% Определение L-системы для кривой Коха")
    latex.append("\\pgfdeclarelindenmayersystem{Koch curve}{")
    latex.append("  \\rule{F -> F-F++F-F}}")
    latex.append("")
    latex.append("\\begin{document}")
    latex.append("\\begin{tikzpicture}")
    latex.append("  % Снежинка Коха (три кривых Коха образуют снежинку)")
    latex.append(f"  \\shadedraw[shading=color wheel]")
    latex.append(f"  [l-system={{Koch curve, step={step}, angle={angle}, axiom=F++F++F, order={order}}}]")
    latex.append("  lindenmayer system -- cycle;")
    latex.append("  ")
    latex.append("  % Заголовок")
    latex.append("  \\node[above, font=\\Large\\bfseries, text=blue!70!black] at (current bounding box.north)")
    latex.append("    {Снежинка Коха};")
    latex.append(f"  \\node[below, font=\\small] at (current bounding box.south)")
    latex.append(f"    {{Порядок рекурсии: {order}}};")
    latex.append("\\end{tikzpicture}")
    latex.append("\\end{document}")

    return "\n".join(latex)

if __name__ == "__main__":
    # Генерация файла
    latex_content = generate_koch_latex(order=4, step='2pt', angle=60)

    with open("koch-snowflake.tex", "w", encoding="utf-8") as f:
        f.write(latex_content)

    print("✓ Файл koch-snowflake.tex создан")
    print("  Компиляция: pdflatex koch-snowflake.tex")
