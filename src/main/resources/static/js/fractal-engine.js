/**
 * Fractal Engine - движок для рендеринга фракталов на Canvas
 */

class FractalEngine {
    constructor(canvas) {
        this.canvas = canvas;
        this.ctx = canvas.getContext('2d');
        this.width = canvas.width;
        this.height = canvas.height;
        this.imageData = this.ctx.createImageData(this.width, this.height);
    }

    /**
     * Очистка canvas
     */
    clear() {
        this.ctx.fillStyle = '#000';
        this.ctx.fillRect(0, 0, this.width, this.height);
    }

    /**
     * Преобразование HSV в RGB
     */
    hsvToRgb(h, s, v) {
        let r, g, b;
        let i = Math.floor(h * 6);
        let f = h * 6 - i;
        let p = v * (1 - s);
        let q = v * (1 - f * s);
        let t = v * (1 - (1 - f) * s);

        switch (i % 6) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }

        return [
            Math.floor(r * 255),
            Math.floor(g * 255),
            Math.floor(b * 255)
        ];
    }

    /**
     * Цветовая схема для итераций
     */
    getColor(iterations, maxIterations) {
        if (iterations === maxIterations) {
            return [0, 0, 0];
        }

        const hue = iterations / maxIterations;
        const saturation = 1.0;
        const value = iterations < maxIterations ? 1.0 : 0.0;

        return this.hsvToRgb(hue, saturation, value);
    }

    /**
     * Отрисовка данных пикселей на canvas
     */
    renderPixelData(pixels, maxIterations) {
        const data = this.imageData.data;

        for (let i = 0; i < pixels.length; i++) {
            const iterations = pixels[i];
            const [r, g, b] = this.getColor(iterations, maxIterations);

            const idx = i * 4;
            data[idx] = r;
            data[idx + 1] = g;
            data[idx + 2] = b;
            data[idx + 3] = 255;
        }

        this.ctx.putImageData(this.imageData, 0, 0);
    }

    /**
     * Отрисовка данных Ляпунова
     */
    renderLyapunovData(values) {
        const data = this.imageData.data;

        for (let i = 0; i < values.length; i++) {
            const lyapunov = values[i];
            let r, g, b;

            if (!isFinite(lyapunov) || lyapunov < 0) {
                // Отрицательный - устойчивость (синий)
                const normalized = Math.max(-1.0, lyapunov);
                const t = Math.abs(normalized);
                r = 0;
                g = Math.floor(t * 76);
                b = Math.floor(127 + t * 128);
            } else {
                // Положительный - хаос (красный)
                const normalized = Math.min(1.0, lyapunov);
                const t = normalized;
                r = Math.floor(178 + t * 77);
                g = Math.floor(t * 127);
                b = 0;
            }

            const idx = i * 4;
            data[idx] = r;
            data[idx + 1] = g;
            data[idx + 2] = b;
            data[idx + 3] = 255;
        }

        this.ctx.putImageData(this.imageData, 0, 0);
    }

    /**
     * Отрисовка треугольника Серпинского
     */
    renderSierpinski(triangles) {
        this.clear();
        this.ctx.fillStyle = '#00FF00';
        this.ctx.strokeStyle = '#00AA00';
        this.ctx.lineWidth = 0.5;

        const scale = Math.min(this.width, this.height) * 0.9;
        const offsetX = this.width / 2 - scale / 2;
        const offsetY = this.height / 2 + scale * Math.sqrt(3) / 4;

        triangles.forEach(triangle => {
            this.ctx.beginPath();
            this.ctx.moveTo(
                offsetX + triangle.x1 * scale,
                offsetY - triangle.y1 * scale
            );
            this.ctx.lineTo(
                offsetX + triangle.x2 * scale,
                offsetY - triangle.y2 * scale
            );
            this.ctx.lineTo(
                offsetX + triangle.x3 * scale,
                offsetY - triangle.y3 * scale
            );
            this.ctx.closePath();
            this.ctx.fill();
            this.ctx.stroke();
        });
    }

    /**
     * Отрисовка снежинки Коха
     */
    renderKoch(order) {
        this.clear();
        this.ctx.strokeStyle = '#00AAFF';
        this.ctx.lineWidth = 2;

        const size = Math.min(this.width, this.height) * 0.8;
        const centerX = this.width / 2;
        const centerY = this.height / 2;

        // Три стороны снежинки
        const points = [
            [centerX - size / 2, centerY + size * Math.sqrt(3) / 6],
            [centerX + size / 2, centerY + size * Math.sqrt(3) / 6],
            [centerX, centerY - size * Math.sqrt(3) / 3]
        ];

        for (let i = 0; i < 3; i++) {
            const start = points[i];
            const end = points[(i + 1) % 3];
            this.drawKochCurve(start[0], start[1], end[0], end[1], order);
        }
    }

    drawKochCurve(x1, y1, x2, y2, order) {
        if (order === 0) {
            this.ctx.beginPath();
            this.ctx.moveTo(x1, y1);
            this.ctx.lineTo(x2, y2);
            this.ctx.stroke();
            return;
        }

        const dx = x2 - x1;
        const dy = y2 - y1;

        const x3 = x1 + dx / 3;
        const y3 = y1 + dy / 3;

        const x5 = x1 + 2 * dx / 3;
        const y5 = y1 + 2 * dy / 3;

        const x4 = x3 + (x5 - x3) * Math.cos(Math.PI / 3) - (y5 - y3) * Math.sin(Math.PI / 3);
        const y4 = y3 + (x5 - x3) * Math.sin(Math.PI / 3) + (y5 - y3) * Math.cos(Math.PI / 3);

        this.drawKochCurve(x1, y1, x3, y3, order - 1);
        this.drawKochCurve(x3, y3, x4, y4, order - 1);
        this.drawKochCurve(x4, y4, x5, y5, order - 1);
        this.drawKochCurve(x5, y5, x2, y2, order - 1);
    }

    /**
     * Отрисовка дерева Пифагора
     */
    renderPythagorasTree(order, angle) {
        this.clear();
        this.ctx.strokeStyle = '#8B4513';
        this.ctx.fillStyle = '#228B22';

        const size = Math.min(this.width, this.height) * 0.15;
        const startX = this.width / 2;
        const startY = this.height - 50;

        this.drawPythagorasTree(startX, startY, size, -Math.PI / 2, order, angle);
    }

    drawPythagorasTree(x, y, size, angle, order, branchAngle) {
        if (order === 0) return;

        const x2 = x + size * Math.cos(angle);
        const y2 = y + size * Math.sin(angle);

        // Рисуем ствол
        this.ctx.lineWidth = order;
        this.ctx.beginPath();
        this.ctx.moveTo(x, y);
        this.ctx.lineTo(x2, y2);
        this.ctx.stroke();

        // Рекурсивно рисуем ветви
        const newSize = size * 0.7;
        this.drawPythagorasTree(x2, y2, newSize, angle - branchAngle, order - 1, branchAngle);
        this.drawPythagorasTree(x2, y2, newSize, angle + branchAngle, order - 1, branchAngle);
    }

    /**
     * Отрисовка фрактального ландшафта
     */
    renderLandscape(seed, roughness) {
        this.clear();

        const size = 65; // 2^6 + 1
        const heightmap = this.generateMidpointDisplacement(size, roughness, seed);

        this.drawLandscape(heightmap);
    }

    generateMidpointDisplacement(size, roughness, seed) {
        // Простой генератор случайных чисел с seed
        let seedValue = seed;
        const random = () => {
            seedValue = (seedValue * 9301 + 49297) % 233280;
            return seedValue / 233280;
        };

        const heightmap = Array(size).fill(0).map(() => Array(size).fill(0));

        heightmap[0][0] = 0.5;
        heightmap[0][size - 1] = 0.5;
        heightmap[size - 1][0] = 0.5;
        heightmap[size - 1][size - 1] = 0.5;

        let stepSize = size - 1;
        let scale = roughness;

        while (stepSize > 1) {
            const halfStep = Math.floor(stepSize / 2);

            // Diamond step
            for (let x = halfStep; x < size; x += stepSize) {
                for (let y = halfStep; y < size; y += stepSize) {
                    const avg = (heightmap[x - halfStep][y - halfStep] +
                                heightmap[x - halfStep][y + halfStep] +
                                heightmap[x + halfStep][y - halfStep] +
                                heightmap[x + halfStep][y + halfStep]) / 4.0;
                    heightmap[x][y] = avg + (random() - 0.5) * scale;
                }
            }

            // Square step
            for (let x = 0; x < size; x += halfStep) {
                for (let y = (x + halfStep) % stepSize; y < size; y += stepSize) {
                    let avg = 0;
                    let count = 0;

                    if (x >= halfStep) { avg += heightmap[x - halfStep][y]; count++; }
                    if (x + halfStep < size) { avg += heightmap[x + halfStep][y]; count++; }
                    if (y >= halfStep) { avg += heightmap[x][y - halfStep]; count++; }
                    if (y + halfStep < size) { avg += heightmap[x][y + halfStep]; count++; }

                    heightmap[x][y] = avg / count + (random() - 0.5) * scale;
                }
            }

            stepSize = Math.floor(stepSize / 2);
            scale *= Math.pow(2.0, -roughness);
        }

        // Нормализация
        let minH = Math.min(...heightmap.map(row => Math.min(...row)));
        let maxH = Math.max(...heightmap.map(row => Math.max(...row)));
        const range = maxH - minH;

        if (range > 0) {
            for (let x = 0; x < size; x++) {
                for (let y = 0; y < size; y++) {
                    heightmap[x][y] = (heightmap[x][y] - minH) / range;
                }
            }
        }

        return heightmap;
    }

    drawLandscape(heightmap) {
        const size = heightmap.length;
        const scale = Math.min(this.width, this.height) / size;

        for (let x = 0; x < size - 1; x++) {
            for (let z = 0; z < size - 1; z++) {
                const h1 = heightmap[x][z];
                const h2 = heightmap[x + 1][z];
                const h3 = heightmap[x + 1][z + 1];
                const h4 = heightmap[x][z + 1];

                const avgH = (h1 + h2 + h3 + h4) / 4.0;

                // Цвет на основе высоты
                let color;
                if (avgH < 0.3) color = 'rgb(70, 130, 180)';      // Вода
                else if (avgH < 0.5) color = 'rgb(238, 214, 175)'; // Песок
                else if (avgH < 0.7) color = 'rgb(34, 139, 34)';   // Трава
                else if (avgH < 0.85) color = 'rgb(139, 90, 43)';  // Горы
                else color = 'rgb(255, 250, 250)';                 // Снег

                // Изометрическая проекция
                const isoX1 = (x - z) * scale * 0.5 + this.width / 2;
                const isoY1 = (x + z) * scale * 0.25 + h1 * 100 + 100;

                const isoX2 = (x + 1 - z) * scale * 0.5 + this.width / 2;
                const isoY2 = (x + 1 + z) * scale * 0.25 + h2 * 100 + 100;

                const isoX3 = (x + 1 - z - 1) * scale * 0.5 + this.width / 2;
                const isoY3 = (x + 1 + z + 1) * scale * 0.25 + h3 * 100 + 100;

                const isoX4 = (x - z - 1) * scale * 0.5 + this.width / 2;
                const isoY4 = (x + z + 1) * scale * 0.25 + h4 * 100 + 100;

                this.ctx.fillStyle = color;
                this.ctx.strokeStyle = 'rgba(0, 0, 0, 0.2)';
                this.ctx.lineWidth = 0.5;

                this.ctx.beginPath();
                this.ctx.moveTo(isoX1, isoY1);
                this.ctx.lineTo(isoX2, isoY2);
                this.ctx.lineTo(isoX3, isoY3);
                this.ctx.lineTo(isoX4, isoY4);
                this.ctx.closePath();
                this.ctx.fill();
                this.ctx.stroke();
            }
        }
    }
}
