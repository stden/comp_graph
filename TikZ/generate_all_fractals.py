#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Главный скрипт для генерации всех LaTeX файлов фракталов
Запускает все отдельные генераторы
"""

import subprocess
import sys
import os

def run_generator(script_name):
    """Запуск отдельного генератора"""
    print(f"\n{'='*60}")
    print(f"Запуск: {script_name}")
    print('='*60)

    try:
        result = subprocess.run(
            [sys.executable, script_name],
            capture_output=True,
            text=True,
            check=True
        )
        print(result.stdout)
        if result.stderr:
            print("Warnings:", result.stderr, file=sys.stderr)
        return True
    except subprocess.CalledProcessError as e:
        print(f"✗ Ошибка при запуске {script_name}:", file=sys.stderr)
        print(e.stderr, file=sys.stderr)
        return False

def main():
    """Запуск всех генераторов"""

    print("╔" + "="*58 + "╗")
    print("║" + " "*15 + "ГЕНЕРАЦИЯ ФРАКТАЛОВ" + " "*24 + "║")
    print("╚" + "="*58 + "╝")

    generators = [
        ("gen_sierpinski.py", "Треугольник Серпинского"),
        ("gen_koch.py", "Снежинка Коха"),
        ("gen_pythagoras_tree.py", "Дерево Пифагора"),
        ("gen_mandelbrot.py", "Множество Мандельброта"),
        ("gen_julia.py", "Множество Жюлиа"),
        ("gen_landscape.py", "Фрактальный ландшафт"),
        ("gen_lyapunov.py", "Фрактал Ляпунова"),
    ]

    success_count = 0
    failed = []

    for script, name in generators:
        if run_generator(script):
            success_count += 1
        else:
            failed.append(name)

    # Итоги
    print(f"\n{'='*60}")
    print(f"РЕЗУЛЬТАТЫ")
    print('='*60)
    print(f"Успешно сгенерировано: {success_count}/{len(generators)}")

    if failed:
        print(f"\nНе удалось сгенерировать:")
        for name in failed:
            print(f"  ✗ {name}")
    else:
        print("\n✓ Все фракталы успешно сгенерированы!")

    print(f"\n{'='*60}")
    print("СОЗДАННЫЕ ФАЙЛЫ:")
    print('='*60)

    tex_files = [
        "sierpinski-triangle.tex",
        "koch-snowflake.tex",
        "pythagoras-tree-generated.tex",
        "mandelbrot-set.tex",
        "julia-set.tex",
        "fractal-landscape.tex",
        "lyapunov-fractal.tex",
    ]

    for tex_file in tex_files:
        if os.path.exists(tex_file):
            print(f"  ✓ {tex_file}")
        else:
            print(f"  ✗ {tex_file} (не найден)")

    print(f"\n{'='*60}")
    print("КОМПИЛЯЦИЯ:")
    print('='*60)
    print("Для компиляции всех файлов выполните:")
    print("  ./compile_all.sh")
    print("\nИли компилируйте отдельные файлы:")
    print("  pdflatex <filename>.tex")
    print("\nВНИМАНИЕ: Файлы с пиксельной графикой (Mandelbrot, Julia, Lyapunov)")
    print("          могут компилироваться несколько минут!")

if __name__ == "__main__":
    main()
