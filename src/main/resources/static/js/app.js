/**
 * Основное приложение для управления фракталами
 */

// Глобальные переменные
let currentFractal = 'mandelbrot';
let engine = null;
let viewParams = {};

// Конфигурация фракталов
const fractalConfigs = {
    mandelbrot: {
        name: 'Множество Мандельброта',
        info: 'Множество Мандельброта — это множество комплексных чисел c, для которых итеративная функция f(z) = z² + c не расходится при z₀ = 0.',
        defaultParams: {
            xMin: -2.5,
            xMax: 1.0,
            yMin: -1.2,
            yMax: 1.2,
            maxIterations: 100
        },
        controls: [
            { id: 'maxIterations', label: 'Максимум итераций', type: 'number', min: 10, max: 500, value: 100 }
        ]
    },
    julia: {
        name: 'Множество Жюлиа',
        info: 'Множество Жюлиа определяется итерацией f(z) = z² + c, где c — константа. Различные значения c дают разные формы фрактала.',
        defaultParams: {
            xMin: -2.0,
            xMax: 2.0,
            yMin: -1.5,
            yMax: 1.5,
            cReal: -0.7,
            cImag: 0.27015,
            maxIterations: 100
        },
        controls: [
            { id: 'cReal', label: 'C (действительная часть)', type: 'number', min: -2, max: 2, step: 0.01, value: -0.7 },
            { id: 'cImag', label: 'C (мнимая часть)', type: 'number', min: -2, max: 2, step: 0.01, value: 0.27015 },
            { id: 'maxIterations', label: 'Максимум итераций', type: 'number', min: 10, max: 500, value: 100 }
        ]
    },
    sierpinski: {
        name: 'Треугольник Серпинского',
        info: 'Треугольник Серпинского — это фрактал, получаемый рекурсивным делением треугольника на 4 меньших треугольника и удалением центрального.',
        defaultParams: {
            depth: 6
        },
        controls: [
            { id: 'depth', label: 'Глубина рекурсии', type: 'number', min: 1, max: 8, value: 6 }
        ]
    },
    koch: {
        name: 'Снежинка Коха',
        info: 'Снежинка Коха — это фрактальная кривая, которая создается итеративным добавлением треугольных выступов к сторонам равностороннего треугольника.',
        defaultParams: {
            order: 4
        },
        controls: [
            { id: 'order', label: 'Порядок рекурсии', type: 'number', min: 0, max: 6, value: 4 }
        ]
    },
    pythagoras: {
        name: 'Дерево Пифагора',
        info: 'Фрактальное дерево, создаваемое рекурсивным построением квадратов и треугольников, напоминающее теорему Пифагора.',
        defaultParams: {
            order: 10,
            angle: 0.5
        },
        controls: [
            { id: 'order', label: 'Глубина рекурсии', type: 'number', min: 1, max: 15, value: 10 },
            { id: 'angle', label: 'Угол ветвления', type: 'range', min: 0.1, max: 1.5, step: 0.1, value: 0.5 }
        ]
    },
    landscape: {
        name: 'Фрактальный ландшафт',
        info: 'Процедурно генерируемый ландшафт с использованием алгоритма Midpoint Displacement для создания реалистичного рельефа.',
        defaultParams: {
            seed: 12345,
            roughness: 0.7
        },
        controls: [
            { id: 'seed', label: 'Seed (начальное число)', type: 'number', min: 1, max: 99999, value: 12345 },
            { id: 'roughness', label: 'Шероховатость', type: 'range', min: 0.1, max: 1.5, step: 0.1, value: 0.7 }
        ]
    },
    lyapunov: {
        name: 'Фрактал Ляпунова',
        info: 'Фрактал на основе показателя Ляпунова для логистического отображения. Визуализирует области стабильности (синий) и хаоса (красный).',
        defaultParams: {
            rAMin: 2.0,
            rAMax: 4.0,
            rBMin: 2.0,
            rBMax: 4.0,
            sequence: 'AABAB'
        },
        controls: [
            { id: 'sequence', label: 'Последовательность', type: 'text', value: 'AABAB' }
        ]
    }
};

// Инициализация при загрузке страницы
document.addEventListener('DOMContentLoaded', () => {
    const canvas = document.getElementById('fractal-canvas');
    engine = new FractalEngine(canvas);

    setupEventListeners();
    selectFractal('mandelbrot');
    renderCurrentFractal();

    // Отслеживание координат мыши
    canvas.addEventListener('mousemove', (e) => {
        const rect = canvas.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;

        if (viewParams.xMin !== undefined) {
            const realX = viewParams.xMin + (viewParams.xMax - viewParams.xMin) * x / canvas.width;
            const realY = viewParams.yMin + (viewParams.yMax - viewParams.yMin) * y / canvas.height;
            document.getElementById('coords').textContent = `x: ${realX.toFixed(4)}, y: ${realY.toFixed(4)}`;
        }
    });

    // Zoom на клик
    canvas.addEventListener('click', (e) => {
        if (currentFractal === 'mandelbrot' || currentFractal === 'julia') {
            const rect = canvas.getBoundingClientRect();
            const x = e.clientX - rect.left;
            const y = e.clientY - rect.top;

            const centerX = viewParams.xMin + (viewParams.xMax - viewParams.xMin) * x / canvas.width;
            const centerY = viewParams.yMin + (viewParams.yMax - viewParams.yMin) * y / canvas.height;

            const rangeX = (viewParams.xMax - viewParams.xMin) / 3;
            const rangeY = (viewParams.yMax - viewParams.yMin) / 3;

            viewParams.xMin = centerX - rangeX / 2;
            viewParams.xMax = centerX + rangeX / 2;
            viewParams.yMin = centerY - rangeY / 2;
            viewParams.yMax = centerY + rangeY / 2;

            renderCurrentFractal();
        }
    });
});

// Настройка обработчиков событий
function setupEventListeners() {
    // Выбор фрактала
    document.querySelectorAll('.fractal-btn').forEach(btn => {
        btn.addEventListener('click', () => {
            const fractal = btn.getAttribute('data-fractal');
            selectFractal(fractal);
        });
    });

    // Кнопка рендеринга
    document.getElementById('render-btn').addEventListener('click', () => {
        renderCurrentFractal();
    });

    // Кнопка сброса
    document.getElementById('reset-btn').addEventListener('click', () => {
        resetParams();
        renderCurrentFractal();
    });
}

// Выбор фрактала
function selectFractal(fractal) {
    currentFractal = fractal;

    // Обновить активную кнопку
    document.querySelectorAll('.fractal-btn').forEach(btn => {
        btn.classList.remove('active');
    });
    document.querySelector(`[data-fractal="${fractal}"]`).classList.add('active');

    // Обновить информацию
    const config = fractalConfigs[fractal];
    document.getElementById('fractal-info').innerHTML = `
        <h4>${config.name}</h4>
        <p>${config.info}</p>
    `;

    // Создать контролы
    createControls(config.controls);

    // Сбросить параметры вида
    resetParams();
}

// Создание элементов управления
function createControls(controls) {
    const container = document.getElementById('controls-container');
    container.innerHTML = '';

    controls.forEach(control => {
        const group = document.createElement('div');
        group.className = 'control-group';

        const label = document.createElement('label');
        label.textContent = control.label;
        label.setAttribute('for', control.id);
        group.appendChild(label);

        if (control.type === 'range') {
            const rangeContainer = document.createElement('div');
            const input = document.createElement('input');
            input.type = 'range';
            input.id = control.id;
            input.min = control.min;
            input.max = control.max;
            input.step = control.step || 0.1;
            input.value = control.value;

            const valueSpan = document.createElement('span');
            valueSpan.className = 'range-value';
            valueSpan.textContent = control.value;

            input.addEventListener('input', (e) => {
                valueSpan.textContent = e.target.value;
            });

            rangeContainer.appendChild(input);
            rangeContainer.appendChild(valueSpan);
            group.appendChild(rangeContainer);
        } else {
            const input = document.createElement('input');
            input.type = control.type || 'number';
            input.id = control.id;
            if (control.min !== undefined) input.min = control.min;
            if (control.max !== undefined) input.max = control.max;
            if (control.step !== undefined) input.step = control.step;
            input.value = control.value;
            group.appendChild(input);
        }

        container.appendChild(group);
    });
}

// Сброс параметров
function resetParams() {
    viewParams = { ...fractalConfigs[currentFractal].defaultParams };
}

// Получить параметры из контролов
function getControlValues() {
    const params = {};
    const controls = fractalConfigs[currentFractal].controls;

    controls.forEach(control => {
        const element = document.getElementById(control.id);
        if (element) {
            if (control.type === 'number' || control.type === 'range') {
                params[control.id] = parseFloat(element.value);
            } else {
                params[control.id] = element.value;
            }
        }
    });

    return params;
}

// Рендеринг текущего фрактала
async function renderCurrentFractal() {
    const loading = document.getElementById('loading');
    loading.classList.remove('hidden');

    const params = getControlValues();

    try {
        switch (currentFractal) {
            case 'mandelbrot':
                await renderMandelbrot(params);
                break;
            case 'julia':
                await renderJulia(params);
                break;
            case 'sierpinski':
                await renderSierpinski(params);
                break;
            case 'koch':
                await renderKoch(params);
                break;
            case 'pythagoras':
                await renderPythagoras(params);
                break;
            case 'landscape':
                await renderLandscape(params);
                break;
            case 'lyapunov':
                await renderLyapunov(params);
                break;
        }
    } catch (error) {
        console.error('Ошибка рендеринга:', error);
        alert('Ошибка при генерации фрактала: ' + error.message);
    } finally {
        loading.classList.add('hidden');
    }
}

// Рендеринг Мандельброта
async function renderMandelbrot(params) {
    const response = await fetch('/api/fractals/mandelbrot', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            width: engine.width,
            height: engine.height,
            xMin: viewParams.xMin,
            xMax: viewParams.xMax,
            yMin: viewParams.yMin,
            yMax: viewParams.yMax,
            maxIterations: params.maxIterations
        })
    });

    const data = await response.json();
    engine.renderPixelData(data.pixels, data.maxIterations);
}

// Рендеринг Жюлиа
async function renderJulia(params) {
    const response = await fetch('/api/fractals/julia', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            width: engine.width,
            height: engine.height,
            xMin: viewParams.xMin,
            xMax: viewParams.xMax,
            yMin: viewParams.yMin,
            yMax: viewParams.yMax,
            cReal: params.cReal,
            cImag: params.cImag,
            maxIterations: params.maxIterations
        })
    });

    const data = await response.json();
    engine.renderPixelData(data.pixels, data.maxIterations);
}

// Рендеринг Серпинского
async function renderSierpinski(params) {
    const response = await fetch('/api/fractals/sierpinski', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            depth: params.depth
        })
    });

    const data = await response.json();
    engine.renderSierpinski(data.triangles);
}

// Рендеринг Коха (клиентский рендеринг)
async function renderKoch(params) {
    // Небольшая задержка для отображения индикатора загрузки
    await new Promise(resolve => setTimeout(resolve, 100));
    engine.renderKoch(params.order);
}

// Рендеринг дерева Пифагора (клиентский рендеринг)
async function renderPythagoras(params) {
    await new Promise(resolve => setTimeout(resolve, 100));
    engine.renderPythagorasTree(params.order, params.angle);
}

// Рендеринг ландшафта (клиентский рендеринг)
async function renderLandscape(params) {
    await new Promise(resolve => setTimeout(resolve, 100));
    engine.renderLandscape(params.seed, params.roughness);
}

// Рендеринг Ляпунова
async function renderLyapunov(params) {
    const response = await fetch('/api/fractals/lyapunov', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            width: engine.width,
            height: engine.height,
            rAMin: viewParams.rAMin,
            rAMax: viewParams.rAMax,
            rBMin: viewParams.rBMin,
            rBMax: viewParams.rBMax,
            sequence: params.sequence
        })
    });

    const data = await response.json();
    engine.renderLyapunovData(data.values);
}
