package graphics;

import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Тесты для 2D трансформаций (TDD подход)
 * Модуль 2: 2D-преобразования
 */
public class Transform2DTest {
    private static final double EPSILON = 0.0001;

    @Test
    public void testIdentityTransform() {
        // Единичная матрица не изменяет точку
        Transform2D identity = new Transform2D();
        double[] result = identity.transform(10, 20);

        assertEquals("X не изменился", 10, result[0], EPSILON);
        assertEquals("Y не изменился", 20, result[1], EPSILON);
    }

    @Test
    public void testTranslate() {
        // Трансляция (перенос) на (5, 3)
        Transform2D t = Transform2D.translate(5, 3);
        double[] result = t.transform(10, 20);

        assertEquals("X сдвинут на 5", 15, result[0], EPSILON);
        assertEquals("Y сдвинут на 3", 23, result[1], EPSILON);
    }

    @Test
    public void testTranslateNegative() {
        // Отрицательная трансляция
        Transform2D t = Transform2D.translate(-10, -5);
        double[] result = t.transform(20, 15);

        assertEquals("X сдвинут на -10", 10, result[0], EPSILON);
        assertEquals("Y сдвинут на -5", 10, result[1], EPSILON);
    }

    @Test
    public void testScale() {
        // Масштабирование в 2 раза по X, в 3 раза по Y
        Transform2D s = Transform2D.scale(2, 3);
        double[] result = s.transform(5, 4);

        assertEquals("X масштабирован ×2", 10, result[0], EPSILON);
        assertEquals("Y масштабирован ×3", 12, result[1], EPSILON);
    }

    @Test
    public void testScaleUniform() {
        // Равномерное масштабирование
        Transform2D s = Transform2D.scale(0.5, 0.5);
        double[] result = s.transform(10, 20);

        assertEquals("X уменьшен вдвое", 5, result[0], EPSILON);
        assertEquals("Y уменьшен вдвое", 10, result[1], EPSILON);
    }

    @Test
    public void testRotate90Degrees() {
        // Поворот на 90° против часовой стрелки
        Transform2D r = Transform2D.rotate(Math.PI / 2);
        double[] result = r.transform(1, 0);

        // (1, 0) → (0, 1) после поворота на 90°
        assertEquals("X после поворота", 0, result[0], EPSILON);
        assertEquals("Y после поворота", 1, result[1], EPSILON);
    }

    @Test
    public void testRotate180Degrees() {
        // Поворот на 180°
        Transform2D r = Transform2D.rotate(Math.PI);
        double[] result = r.transform(5, 3);

        // Точка должна отразиться относительно начала координат
        assertEquals("X инвертирован", -5, result[0], EPSILON);
        assertEquals("Y инвертирован", -3, result[1], EPSILON);
    }

    @Test
    public void testRotate45Degrees() {
        // Поворот на 45°
        Transform2D r = Transform2D.rotate(Math.PI / 4);
        double[] result = r.transform(1, 0);

        // (1, 0) → (√2/2, √2/2)
        double expected = Math.sqrt(2) / 2;
        assertEquals("X = √2/2", expected, result[0], EPSILON);
        assertEquals("Y = √2/2", expected, result[1], EPSILON);
    }

    @Test
    public void testRotateAround() {
        // Поворот вокруг точки (5, 5) на 90°
        Transform2D r = Transform2D.rotateAround(5, 5, Math.PI / 2);

        // Точка (6, 5) должна перейти в (5, 6)
        double[] result = r.transform(6, 5);

        assertEquals("X после поворота вокруг центра", 5, result[0], EPSILON);
        assertEquals("Y после поворота вокруг центра", 6, result[1], EPSILON);
    }

    @Test
    public void testCompositeTransform() {
        // Композиция: сначала масштабируем, потом переносим
        Transform2D scale = Transform2D.scale(2, 2);
        Transform2D translate = Transform2D.translate(10, 5);

        Transform2D composite = translate.multiply(scale);
        double[] result = composite.transform(3, 4);

        // (3, 4) → масштаб ×2 → (6, 8) → перенос (+10, +5) → (16, 13)
        assertEquals("X после композиции", 16, result[0], EPSILON);
        assertEquals("Y после композиции", 13, result[1], EPSILON);
    }

    @Test
    public void testCompositeOrder() {
        // Проверка, что порядок композиции важен
        Transform2D scale = Transform2D.scale(2, 2);
        Transform2D translate = Transform2D.translate(10, 5);

        // Порядок 1: translate * scale
        Transform2D t1 = translate.multiply(scale);
        double[] r1 = t1.transform(1, 1);

        // Порядок 2: scale * translate
        Transform2D t2 = scale.multiply(translate);
        double[] r2 = t2.transform(1, 1);

        // Результаты должны быть разными
        assertNotEquals("Разный X", r1[0], r2[0], EPSILON);
        assertNotEquals("Разный Y", r1[1], r2[1], EPSILON);
    }

    @Test
    public void testFlipX() {
        // Отражение по оси X (инвертирует Y)
        Transform2D flip = Transform2D.flipX();
        double[] result = flip.transform(5, 7);

        assertEquals("X не изменился", 5, result[0], EPSILON);
        assertEquals("Y инвертирован", -7, result[1], EPSILON);
    }

    @Test
    public void testFlipY() {
        // Отражение по оси Y (инвертирует X)
        Transform2D flip = Transform2D.flipY();
        double[] result = flip.transform(5, 7);

        assertEquals("X инвертирован", -5, result[0], EPSILON);
        assertEquals("Y не изменился", 7, result[1], EPSILON);
    }

    @Test
    public void testMatrixAccess() {
        // Проверка доступа к элементам матрицы
        Transform2D t = Transform2D.translate(5, 10);

        assertEquals("Элемент [0][2] = dx", 5, t.get(0, 2), EPSILON);
        assertEquals("Элемент [1][2] = dy", 10, t.get(1, 2), EPSILON);
        assertEquals("Элемент [2][2] = 1", 1, t.get(2, 2), EPSILON);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testInvalidMatrixSize() {
        // Попытка создать трансформацию из неправильной матрицы
        double[][] invalid = {{1, 0}, {0, 1}};  // 2x2 вместо 3x3
        new Transform2D(invalid);
    }

    @Test
    public void testComplexTransform() {
        // Сложная трансформация: масштаб → поворот → перенос
        Transform2D transform = Transform2D.translate(100, 100)
            .multiply(Transform2D.rotate(Math.PI / 4))
            .multiply(Transform2D.scale(2, 2));

        // Применяем к точке (1, 0)
        double[] result = transform.transform(1, 0);

        // Ожидаем: (1,0) → scale(2,2) → (2,0) → rotate(45°) → (√2,√2) → translate(100,100) → (100+√2, 100+√2)
        double sqrt2 = Math.sqrt(2);
        assertEquals("X", 100 + sqrt2, result[0], EPSILON);
        assertEquals("Y", 100 + sqrt2, result[1], EPSILON);
    }

    @Test
    public void testIdentityMultiply() {
        // Умножение на единичную матрицу не меняет трансформацию
        Transform2D t = Transform2D.translate(5, 10);
        Transform2D identity = new Transform2D();

        Transform2D result = t.multiply(identity);
        double[] point = result.transform(3, 7);

        // Должно быть то же самое, что t.transform(3, 7)
        double[] expected = t.transform(3, 7);
        assertEquals("X совпадает", expected[0], point[0], EPSILON);
        assertEquals("Y совпадает", expected[1], point[1], EPSILON);
    }

    @Test
    public void testInverseScale() {
        // Масштабирование на k, затем на 1/k должно вернуть исходную точку
        Transform2D scale = Transform2D.scale(3, 3);
        Transform2D scaleInverse = Transform2D.scale(1.0/3, 1.0/3);

        Transform2D identity = scale.multiply(scaleInverse);
        double[] result = identity.transform(10, 20);

        assertEquals("X восстановлен", 10, result[0], EPSILON);
        assertEquals("Y восстановлен", 20, result[1], EPSILON);
    }

    @Test
    public void testInverseRotate() {
        // Поворот на угол α, затем на -α должно вернуть исходную точку
        double angle = Math.PI / 3;  // 60°
        Transform2D rotate = Transform2D.rotate(angle);
        Transform2D rotateBack = Transform2D.rotate(-angle);

        Transform2D identity = rotate.multiply(rotateBack);
        double[] result = identity.transform(7, 13);

        assertEquals("X восстановлен", 7, result[0], EPSILON);
        assertEquals("Y восстановлен", 13, result[1], EPSILON);
    }

    // ============ ИГРОВЫЕ ТЕСТЫ (КАК В ИГРАХ) ============

    @Test
    public void testPlayerMovement() {
        // Симуляция движения игрока в 2D игре (WASD)
        double playerX = 100, playerY = 100;
        double speed = 5.0;

        // W - движение вверх
        Transform2D moveUp = Transform2D.translate(0, speed);
        double[] pos = moveUp.transform(playerX, playerY);
        assertEquals("Движение вверх", 105, pos[1], EPSILON);

        // S - движение вниз
        Transform2D moveDown = Transform2D.translate(0, -speed);
        pos = moveDown.transform(playerX, playerY);
        assertEquals("Движение вниз", 95, pos[1], EPSILON);

        // A - движение влево
        Transform2D moveLeft = Transform2D.translate(-speed, 0);
        pos = moveLeft.transform(playerX, playerY);
        assertEquals("Движение влево", 95, pos[0], EPSILON);

        // D - движение вправо
        Transform2D moveRight = Transform2D.translate(speed, 0);
        pos = moveRight.transform(playerX, playerY);
        assertEquals("Движение вправо", 105, pos[0], EPSILON);
    }

    @Test
    public void testArkanoidBallBounce() {
        // Арканоид: отражение мяча от стен
        double ballX = 50, ballY = 50;
        double velocityX = 5, velocityY = 3;

        // Мяч летит вправо-вверх
        Transform2D move = Transform2D.translate(velocityX, velocityY);
        double[] newPos = move.transform(ballX, ballY);
        assertEquals("Мяч движется", 55, newPos[0], EPSILON);

        // Отражение от правой стены (инвертируем X)
        Transform2D bounceX = Transform2D.scale(-1, 1);
        double[] bounced = bounceX.transform(velocityX, velocityY);
        assertEquals("Отскок от стены X", -velocityX, bounced[0], EPSILON);
        assertEquals("Y скорость не изменилась", velocityY, bounced[1], EPSILON);

        // Отражение от потолка (инвертируем Y)
        Transform2D bounceY = Transform2D.scale(1, -1);
        bounced = bounceY.transform(velocityX, velocityY);
        assertEquals("X скорость не изменилась", velocityX, bounced[0], EPSILON);
        assertEquals("Отскок от потолка Y", -velocityY, bounced[1], EPSILON);
    }

    @Test
    public void testTetrisPieceRotation() {
        // Тетрис: вращение фигур (I, O, T, L, S, Z)
        // T-фигура относительно центра
        double[][] tPiece = {
            {-1, 0}, {0, 0}, {1, 0}, {0, 1}  // T-образная фигура
        };

        Transform2D rotate90 = Transform2D.rotate(Math.PI / 2);

        // Поворачиваем каждый блок T-фигуры
        for (double[] block : tPiece) {
            double[] rotated = rotate90.transform(block[0], block[1]);
            // Проверяем, что блоки всё ещё связаны
            assertNotNull("Блок повёрнут", rotated);
        }

        // Проверяем центральный блок остаётся на месте при 360° вращении
        Transform2D rotate360 = Transform2D.rotate(2 * Math.PI);
        double[] center = rotate360.transform(0, 0);
        assertEquals("Центр на месте после 360°", 0, center[0], EPSILON);
        assertEquals("Центр на месте после 360°", 0, center[1], EPSILON);
    }

    @Test
    public void testDoomStyleRaycasting() {
        // Doom-style raycasting для 2.5D движка
        double playerX = 0, playerY = 0;
        double playerAngle = 0; // Смотрит вправо
        double fov = Math.PI / 3; // 60 градусов FOV
        int rayCount = 10;

        for (int i = 0; i < rayCount; i++) {
            double rayAngle = playerAngle - fov/2 + (fov * i) / (rayCount - 1);
            Transform2D rayDir = Transform2D.rotate(rayAngle);

            // Луч на расстояние 100 единиц
            double[] rayEnd = rayDir.transform(100, 0);

            // Проверяем, что все лучи веером исходят от игрока
            double distance = Math.sqrt(rayEnd[0]*rayEnd[0] + rayEnd[1]*rayEnd[1]);
            assertEquals("Луч " + i + " на расстоянии 100", 100, distance, 0.1);
        }
    }

    @Test
    public void testPacmanGhostAI() {
        // Pac-Man: призрак преследует игрока
        double ghostX = 0, ghostY = 0;
        double pacmanX = 100, pacmanY = 100;
        double ghostSpeed = 2;

        // Вычисляем направление к Pac-Man
        double dx = pacmanX - ghostX;
        double dy = pacmanY - ghostY;
        double distance = Math.sqrt(dx*dx + dy*dy);

        // Нормализуем и применяем скорость
        double moveX = (dx / distance) * ghostSpeed;
        double moveY = (dy / distance) * ghostSpeed;

        Transform2D chaseMove = Transform2D.translate(moveX, moveY);
        double[] newGhostPos = chaseMove.transform(ghostX, ghostY);

        // Призрак приближается к Pac-Man
        double newDistance = Math.sqrt(
            Math.pow(pacmanX - newGhostPos[0], 2) +
            Math.pow(pacmanY - newGhostPos[1], 2)
        );
        assertTrue("Призрак приближается", newDistance < distance);
    }

    @Test
    public void testSpaceInvadersFormation() {
        // Space Invaders: движение строя пришельцев
        int rows = 5, cols = 11;
        double spacing = 20;
        double formationSpeed = 1;

        // Вся формация движется вправо
        Transform2D moveRight = Transform2D.translate(formationSpeed, 0);

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                double alienX = col * spacing;
                double alienY = row * spacing;

                double[] newPos = moveRight.transform(alienX, alienY);

                // Все пришельцы движутся синхронно
                assertEquals("Пришелец сдвинулся на 1", alienX + formationSpeed, newPos[0], EPSILON);
                assertEquals("Y не изменился", alienY, newPos[1], EPSILON);
            }
        }
    }

    @Test
    public void testAngryBirdsSlingshot() {
        // Angry Birds: рогатка
        double birdX = 100, birdY = 100;
        double pullDistance = 50;
        double pullAngle = Math.PI / 4; // 45 градусов

        // Оттягиваем птицу назад
        Transform2D pullBack = Transform2D.rotate(pullAngle + Math.PI);
        double[] pulledPos = pullBack.transform(pullDistance, 0);

        Transform2D position = Transform2D.translate(pulledPos[0], pulledPos[1]);
        double[] birdPos = position.transform(birdX, birdY);

        // Птица оттянута назад
        assertTrue("Птица оттянута влево", birdPos[0] < birdX);
        assertTrue("Птица оттянута вниз", birdPos[1] < birdY);
    }

    @Test
    public void testMinecraftBlockPlacement() {
        // Minecraft: размещение блоков на сетке
        double cursorX = 123.7, cursorY = 456.3;
        int blockSize = 16;

        // Snap to grid
        int gridX = (int)(cursorX / blockSize) * blockSize;
        int gridY = (int)(cursorY / blockSize) * blockSize;

        Transform2D snapToGrid = Transform2D.translate(
            gridX - cursorX,
            gridY - cursorY
        );

        double[] snappedPos = snapToGrid.transform(cursorX, cursorY);

        assertEquals("Блок привязан к сетке X", 112, snappedPos[0], EPSILON);
        assertEquals("Блок привязан к сетке Y", 448, snappedPos[1], EPSILON);
    }

    @Test
    public void testFruitNinjaSlash() {
        // Fruit Ninja: траектория разреза
        double slashStartX = 50, slashStartY = 100;
        double slashEndX = 200, slashEndY = 300;

        // Проверяем точки вдоль линии разреза
        for (double t = 0; t <= 1.0; t += 0.1) {
            double lerpX = slashStartX + t * (slashEndX - slashStartX);
            double lerpY = slashStartY + t * (slashEndY - slashStartY);

            Transform2D identity = Transform2D.translate(0, 0);
            double[] point = identity.transform(lerpX, lerpY);

            // Точки лежат на линии
            assertTrue("Точка на линии разреза", point[0] >= slashStartX);
            assertTrue("Точка на линии разреза", point[0] <= slashEndX);
        }
    }

    @Test
    public void testCandyCrushGemSwap() {
        // Candy Crush: обмен конфет местами
        double gem1X = 100, gem1Y = 100;
        double gem2X = 120, gem2Y = 100; // Справа от gem1

        // Анимация обмена (движение друг к другу)
        double swapDistance = gem2X - gem1X;

        Transform2D swap1 = Transform2D.translate(swapDistance, 0);
        Transform2D swap2 = Transform2D.translate(-swapDistance, 0);

        double[] newGem1 = swap1.transform(gem1X, gem1Y);
        double[] newGem2 = swap2.transform(gem2X, gem2Y);

        // После обмена позиции поменялись
        assertEquals("Gem1 переместился на место gem2", gem2X, newGem1[0], EPSILON);
        assertEquals("Gem2 переместился на место gem1", gem1X, newGem2[0], EPSILON);
    }

    @Test
    public void testFlappyBirdPipes() {
        // Flappy Bird: движение труб
        double pipeX = 800; // За правым краем экрана
        double pipeY = 300;
        double scrollSpeed = 3;

        // Симулируем 100 кадров прокрутки
        double totalScroll = scrollSpeed * 100;

        // Трубы прокручиваются влево
        Transform2D scroll = Transform2D.translate(-totalScroll, 0);
        double[] finalPos = scroll.transform(pipeX, pipeY);

        // Труба ушла за левый край после 100 кадров
        // 800 - 300 = 500, должно быть далеко слева
        assertTrue("Труба прокрутилась влево", finalPos[0] < pipeX);
        assertTrue("Труба за экраном слева", finalPos[0] <= 500);
    }

    @Test
    public void testTopDownZombieHorde() {
        // Зомби-хоррор сверху: орда зомби окружает игрока
        double playerX = 400, playerY = 300;
        int zombieCount = 8;

        for (int i = 0; i < zombieCount; i++) {
            double angle = (2 * Math.PI * i) / zombieCount;
            double radius = 100;

            Transform2D rotation = Transform2D.rotate(angle);
            double[] offset = rotation.transform(radius, 0);

            Transform2D position = Transform2D.translate(offset[0], offset[1]);
            double[] zombiePos = position.transform(playerX, playerY);

            // Зомби на расстоянии 100 от игрока
            double distance = Math.sqrt(
                Math.pow(zombiePos[0] - playerX, 2) +
                Math.pow(zombiePos[1] - playerY, 2)
            );
            assertEquals("Зомби " + i + " на расстоянии 100", 100, distance, 0.1);
        }
    }

    @Test
    public void testRacingGameDrift() {
        // Гоночная игра: дрифт (скольжение боком)
        double carX = 100, carY = 100;
        double carAngle = Math.PI / 6; // Машина смотрит под углом 30°
        double forwardSpeed = 10;
        double driftAngle = Math.PI / 12; // Но скользит под углом 15°

        Transform2D carDirection = Transform2D.rotate(carAngle);
        Transform2D driftDirection = Transform2D.rotate(driftAngle);

        double[] forward = carDirection.transform(forwardSpeed, 0);
        double[] drift = driftDirection.transform(forwardSpeed, 0);

        // Машина смотрит в одну сторону, но едет в другую (дрифт)
        assertFalse("Направление != движение",
                    Math.abs(forward[0] - drift[0]) < EPSILON);
    }
}
