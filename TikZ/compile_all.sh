#!/bin/bash
# Скрипт для компиляции всех LaTeX файлов фракталов

echo "╔══════════════════════════════════════════════════════════╗"
echo "║           КОМПИЛЯЦИЯ LATEX ФАЙЛОВ ФРАКТАЛОВ              ║"
echo "╚══════════════════════════════════════════════════════════╝"
echo ""

# Массив файлов для компиляции
TEX_FILES=(
    "sierpinski-triangle.tex"
    "koch-snowflake.tex"
    "pythagoras-tree-generated.tex"
    "mandelbrot-set.tex"
    "julia-set.tex"
    "fractal-landscape.tex"
    "lyapunov-fractal.tex"
)

SUCCESS=0
FAILED=0
TOTAL=${#TEX_FILES[@]}

# Компиляция каждого файла
for tex_file in "${TEX_FILES[@]}"; do
    if [ -f "$tex_file" ]; then
        echo "──────────────────────────────────────────────────────────"
        echo "Компиляция: $tex_file"
        echo "──────────────────────────────────────────────────────────"

        # Компиляция с подавлением избыточного вывода
        if pdflatex -interaction=nonstopmode -halt-on-error "$tex_file" > /dev/null 2>&1; then
            echo "✓ $tex_file успешно скомпилирован"
            ((SUCCESS++))

            # Очистка вспомогательных файлов
            rm -f "${tex_file%.tex}.aux" "${tex_file%.tex}.log"
        else
            echo "✗ Ошибка при компиляции $tex_file"
            ((FAILED++))
            echo "  Подробности в файле: ${tex_file%.tex}.log"
        fi
        echo ""
    else
        echo "✗ Файл не найден: $tex_file"
        ((FAILED++))
        echo ""
    fi
done

# Итоги
echo "══════════════════════════════════════════════════════════"
echo "РЕЗУЛЬТАТЫ КОМПИЛЯЦИИ"
echo "══════════════════════════════════════════════════════════"
echo "Всего файлов:     $TOTAL"
echo "Успешно:          $SUCCESS"
echo "Ошибок:           $FAILED"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "✓ Все файлы успешно скомпилированы!"
else
    echo "✗ Некоторые файлы не скомпилированы"
fi

echo ""
echo "Созданные PDF файлы:"
ls -lh *.pdf 2>/dev/null | awk '{print "  " $9 " (" $5 ")"}'
