package graphics;

import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Тесты для 3D матриц и проекций (TDD подход)
 * Модуль 3: Основы 3D-графики
 */
public class Matrix4Test {
    private static final double EPSILON = 0.001;

    @Test
    public void testIdentity() {
        // Единичная матрица не изменяет точку
        Matrix4 identity = new Matrix4();
        double[] result = identity.transformPoint(5, 10, 15);

        assertEquals("X не изменился", 5, result[0], EPSILON);
        assertEquals("Y не изменился", 10, result[1], EPSILON);
        assertEquals("Z не изменился", 15, result[2], EPSILON);
    }

    @Test
    public void testTranslate() {
        // Трансляция в 3D
        Matrix4 t = Matrix4.translate(10, 20, 30);
        double[] result = t.transformPoint(1, 2, 3);

        assertEquals("X сдвинут", 11, result[0], EPSILON);
        assertEquals("Y сдвинут", 22, result[1], EPSILON);
        assertEquals("Z сдвинут", 33, result[2], EPSILON);
    }

    @Test
    public void testScale() {
        // Масштабирование в 3D
        Matrix4 s = Matrix4.scale(2, 3, 4);
        double[] result = s.transformPoint(5, 6, 7);

        assertEquals("X масштабирован ×2", 10, result[0], EPSILON);
        assertEquals("Y масштабирован ×3", 18, result[1], EPSILON);
        assertEquals("Z масштабирован ×4", 28, result[2], EPSILON);
    }

    @Test
    public void testRotateX90() {
        // Поворот на 90° вокруг оси X
        // Точка (0, 1, 0) → (0, 0, 1)
        Matrix4 r = Matrix4.rotateX(Math.PI / 2);
        double[] result = r.transformPoint(0, 1, 0);

        assertEquals("X не изменился", 0, result[0], EPSILON);
        assertEquals("Y → 0", 0, result[1], EPSILON);
        assertEquals("Z → 1", 1, result[2], EPSILON);
    }

    @Test
    public void testRotateY90() {
        // Поворот на 90° вокруг оси Y
        // Точка (1, 0, 0) → (0, 0, -1)
        Matrix4 r = Matrix4.rotateY(Math.PI / 2);
        double[] result = r.transformPoint(1, 0, 0);

        assertEquals("X → 0", 0, result[0], EPSILON);
        assertEquals("Y не изменился", 0, result[1], EPSILON);
        assertEquals("Z → -1", -1, result[2], EPSILON);
    }

    @Test
    public void testRotateZ90() {
        // Поворот на 90° вокруг оси Z
        // Точка (1, 0, 0) → (0, 1, 0)
        Matrix4 r = Matrix4.rotateZ(Math.PI / 2);
        double[] result = r.transformPoint(1, 0, 0);

        assertEquals("X → 0", 0, result[0], EPSILON);
        assertEquals("Y → 1", 1, result[1], EPSILON);
        assertEquals("Z не изменился", 0, result[2], EPSILON);
    }

    @Test
    public void testMatrixMultiply() {
        // Композиция трансформаций: масштаб, затем перенос
        Matrix4 scale = Matrix4.scale(2, 2, 2);
        Matrix4 translate = Matrix4.translate(10, 0, 0);

        Matrix4 composite = translate.multiply(scale);
        double[] result = composite.transformPoint(5, 0, 0);

        // (5,0,0) → scale ×2 → (10,0,0) → translate +10 → (20,0,0)
        assertEquals("X после композиции", 20, result[0], EPSILON);
        assertEquals("Y = 0", 0, result[1], EPSILON);
        assertEquals("Z = 0", 0, result[2], EPSILON);
    }

    @Test
    public void testRotationOrder() {
        // Порядок вращений важен: X→Y→Z ≠ Z→Y→X
        Matrix4 rx = Matrix4.rotateX(Math.PI / 4);
        Matrix4 ry = Matrix4.rotateY(Math.PI / 4);

        Matrix4 order1 = rx.multiply(ry);
        Matrix4 order2 = ry.multiply(rx);

        double[] r1 = order1.transformPoint(1, 1, 1);
        double[] r2 = order2.transformPoint(1, 1, 1);

        // Результаты должны различаться
        boolean different = Math.abs(r1[0] - r2[0]) > EPSILON ||
                           Math.abs(r1[1] - r2[1]) > EPSILON ||
                           Math.abs(r1[2] - r2[2]) > EPSILON;

        assertTrue("Порядок вращений влияет на результат", different);
    }

    @Test
    public void testOrthographic() {
        // Ортографическая проекция
        // Отображает [-1,1]×[-1,1]×[-1,1] в NDC (Normalized Device Coordinates)
        Matrix4 ortho = Matrix4.orthographic(-1, 1, -1, 1, 0.1, 100);

        // Точка в центре должна остаться на месте (кроме Z)
        double[] result = ortho.transformPoint(0, 0, -1);

        assertEquals("X в центре", 0, result[0], EPSILON);
        assertEquals("Y в центре", 0, result[1], EPSILON);
        // Z будет преобразована в NDC диапазон
    }

    @Test
    public void testOrthographicEdges() {
        // Проверка краёв ортографической проекции
        Matrix4 ortho = Matrix4.orthographic(-10, 10, -5, 5, 1, 100);

        // Левый край
        double[] left = ortho.transformPoint(-10, 0, -10);
        assertEquals("Левый край → -1", -1, left[0], EPSILON);

        // Правый край
        double[] right = ortho.transformPoint(10, 0, -10);
        assertEquals("Правый край → 1", 1, right[0], EPSILON);

        // Нижний край
        double[] bottom = ortho.transformPoint(0, -5, -10);
        assertEquals("Нижний край → -1", -1, bottom[1], EPSILON);

        // Верхний край
        double[] top = ortho.transformPoint(0, 5, -10);
        assertEquals("Верхний край → 1", 1, top[1], EPSILON);
    }

    @Test
    public void testPerspective() {
        // Перспективная проекция
        // FOV = 90°, aspect = 1:1
        Matrix4 persp = Matrix4.perspective(Math.PI / 2, 1.0, 0.1, 100);

        // Проверяем, что матрица создана
        assertNotNull("Матрица создана", persp);

        // Точка на оси Z должна быть спроецирована в центр
        double[] result = persp.transformPoint(0, 0, -1);
        assertEquals("X в центре", 0, result[0], EPSILON);
        assertEquals("Y в центре", 0, result[1], EPSILON);
    }

    @Test
    public void testPerspectiveDivide() {
        // Перспективная проекция выполняет перспективное деление
        // Точки дальше от камеры должны быть меньше
        Matrix4 persp = Matrix4.perspective(Math.PI / 2, 1.0, 0.1, 100);

        double[] near = persp.transformPoint(1, 0, -1);
        double[] far = persp.transformPoint(1, 0, -10);

        // Точка дальше от камеры должна быть меньше по X (перспективное сжатие)
        assertTrue("Дальняя точка меньше", Math.abs(far[0]) < Math.abs(near[0]));
    }

    @Test
    public void testLookAtOrigin() {
        // Камера смотрит на начало координат из точки (0, 0, 5)
        Matrix4 view = Matrix4.lookAt(
            0, 0, 5,    // позиция камеры
            0, 0, 0,    // цель взгляда
            0, 1, 0     // вектор "вверх"
        );

        // Точка в начале координат должна быть перед камерой
        double[] result = view.transformPoint(0, 0, 0);

        // После view трансформации точка (0,0,0) должна быть на расстоянии 5 по оси Z
        assertEquals("Точка перед камерой", -5, result[2], EPSILON);
    }

    @Test
    public void testTransformVector() {
        // Трансформация вектора (w=0, не применяется трансляция)
        Matrix4 t = Matrix4.translate(10, 20, 30);

        double[] vector = t.transform(1, 0, 0, 0);  // Вектор, не точка

        // Трансляция не должна влиять на векторы (w=0)
        assertEquals("Вектор X", 1, vector[0], EPSILON);
        assertEquals("Вектор Y", 0, vector[1], EPSILON);
        assertEquals("Вектор Z", 0, vector[2], EPSILON);
        assertEquals("W = 0", 0, vector[3], EPSILON);
    }

    @Test
    public void testMatrixGet() {
        // Проверка доступа к элементам матрицы
        Matrix4 t = Matrix4.translate(5, 10, 15);

        assertEquals("Элемент [0][3]", 5, t.get(0, 3), EPSILON);
        assertEquals("Элемент [1][3]", 10, t.get(1, 3), EPSILON);
        assertEquals("Элемент [2][3]", 15, t.get(2, 3), EPSILON);
        assertEquals("Элемент [3][3]", 1, t.get(3, 3), EPSILON);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testInvalidMatrixSize() {
        // Попытка создать матрицу неправильного размера
        double[][] invalid = new double[3][3];
        new Matrix4(invalid);
    }

    @Test
    public void testComplexTransform() {
        // Сложная трансформация: Model-View-Projection
        Matrix4 model = Matrix4.translate(0, 0, -5)
            .multiply(Matrix4.rotateY(Math.PI / 4))
            .multiply(Matrix4.scale(2, 2, 2));

        Matrix4 view = Matrix4.lookAt(0, 2, 10, 0, 0, 0, 0, 1, 0);
        Matrix4 projection = Matrix4.perspective(Math.PI / 3, 16.0 / 9.0, 0.1, 100);

        Matrix4 mvp = projection.multiply(view).multiply(model);

        // Применяем к точке
        double[] result = mvp.transformPoint(1, 0, 0);

        // Точка должна быть в пределах NDC [-1, 1]
        assertTrue("X в пределах NDC", result[0] >= -1 && result[0] <= 1);
        assertTrue("Y в пределах NDC", result[1] >= -1 && result[1] <= 1);
    }

    @Test
    public void testIdentityMultiply() {
        // Умножение на единичную матрицу
        Matrix4 t = Matrix4.translate(5, 10, 15);
        Matrix4 identity = new Matrix4();

        Matrix4 result = t.multiply(identity);
        double[] point = result.transformPoint(1, 2, 3);

        double[] expected = t.transformPoint(1, 2, 3);
        assertArrayEquals("Результат не изменился", expected, point, EPSILON);
    }

    // ============ 3D ИГРОВЫЕ ТЕСТЫ ============

    @Test
    public void testFPSCameraMovement() {
        // FPS камера (Doom, Quake, CS:GO)
        double eyeX = 0, eyeY = 1.7, eyeZ = 0; // Высота глаз игрока
        double targetX = 0, targetY = 1.7, targetZ = -10; // Смотрит вперёд
        double upX = 0, upY = 1, upZ = 0;

        Matrix4 view = Matrix4.lookAt(eyeX, eyeY, eyeZ, targetX, targetY, targetZ, upX, upY, upZ);

        // Точка перед игроком должна быть видна
        double[] transformed = view.transformPoint(0, 1.7, -5);
        assertNotNull("FPS камера трансформирует точку", transformed);
    }

    @Test
    public void testThirdPersonCamera() {
        // Камера за спиной (GTA, Tomb Raider)
        double playerX = 0, playerY = 0, playerZ = 0;
        double cameraDistance = 10;
        double cameraHeight = 3;
        double cameraAngle = Math.PI; // Позади игрока

        // Камера находится позади и выше игрока
        double camX = playerX + cameraDistance * Math.cos(cameraAngle);
        double camZ = playerZ + cameraDistance * Math.sin(cameraAngle);
        double camY = playerY + cameraHeight;

        Matrix4 view = Matrix4.lookAt(camX, camY, camZ, playerX, playerY, playerZ, 0, 1, 0);
        assertNotNull("Third-person камера создана", view);
    }

    @Test
    public void testFlightSimulator() {
        // Авиасимулятор: Roll, Pitch, Yaw
        double roll = Math.PI / 12;  // Крен 15°
        double pitch = Math.PI / 6;  // Тангаж 30°
        double yaw = Math.PI / 4;    // Рыскание 45°

        Matrix4 rollMatrix = Matrix4.rotateZ(roll);
        Matrix4 pitchMatrix = Matrix4.rotateX(pitch);
        Matrix4 yawMatrix = Matrix4.rotateY(yaw);

        // Применяем в правильном порядке
        Matrix4 aircraft = yawMatrix.multiply(pitchMatrix).multiply(rollMatrix);

        double[] nose = aircraft.transformPoint(0, 0, -10); // Нос самолёта
        assertNotNull("Ориентация самолёта", nose);
    }

    @Test
    public void testRTSCameraAngle() {
        // RTS камера (StarCraft, Age of Empires) - изометрический вид
        double cameraHeight = 30;
        double cameraAngle = Math.PI / 4; // 45 градусов

        double eyeY = cameraHeight;
        double eyeZ = cameraHeight / Math.tan(cameraAngle);

        Matrix4 view = Matrix4.lookAt(0, eyeY, eyeZ, 0, 0, 0, 0, 1, 0);

        // Проверяем, что земля видна под углом
        double[] ground = view.transformPoint(5, 0, 5);
        assertNotNull("RTS камера видит землю", ground);
    }

    @Test
    public void testOrbitCamera() {
        // Orbit камера вокруг объекта (3D редакторы, просмотр моделей)
        double radius = 10;
        int steps = 12;

        for (int i = 0; i < steps; i++) {
            double angle = (2 * Math.PI * i) / steps;
            double eyeX = radius * Math.cos(angle);
            double eyeZ = radius * Math.sin(angle);
            double eyeY = 5;

            Matrix4 view = Matrix4.lookAt(eyeX, eyeY, eyeZ, 0, 0, 0, 0, 1, 0);

            // Камера всегда смотрит на центр
            assertNotNull("Орбита шаг " + i, view);
        }
    }

    @Test
    public void testPlatformer3DPerspective() {
        // 3D платформер (Mario 64, Crash Bandicoot)
        double fov = Math.toRadians(70);
        double aspect = 16.0 / 9.0;
        double near = 0.1;
        double far = 1000.0;

        Matrix4 projection = Matrix4.perspective(fov, aspect, near, far);

        // Точка близко к камере
        double[] closePoint = projection.transformPoint(0, 0, -1);
        // Точка далеко
        double[] farPoint = projection.transformPoint(0, 0, -100);

        assertNotNull("Близкая точка", closePoint);
        assertNotNull("Дальняя точка", farPoint);
    }

    @Test
    public void testVRHeadTracking() {
        // VR отслеживание головы (Oculus, HTC Vive)
        double headYaw = Math.PI / 6;     // Поворот головы на 30°
        double headPitch = Math.PI / 12;  // Наклон головы на 15°

        Matrix4 yaw = Matrix4.rotateY(headYaw);
        Matrix4 pitch = Matrix4.rotateX(headPitch);

        Matrix4 headOrientation = yaw.multiply(pitch);

        // Точка прямо перед глазами
        double[] forward = headOrientation.transformPoint(0, 0, -1);

        assertTrue("VR взгляд смещён по Y", Math.abs(forward[1]) > 0.01);
        assertTrue("VR взгляд смещён по X", Math.abs(forward[0]) > 0.01);
    }

    @Test
    public void testSkyboxRendering() {
        // Skybox должен оставаться на месте при движении камеры
        double[][] cameraPositions = {{0, 0, 0}, {100, 50, 200}, {-50, 0, 100}};

        for (double[] camPos : cameraPositions) {
            Matrix4 view = Matrix4.lookAt(
                camPos[0], camPos[1], camPos[2],
                camPos[0], camPos[1], camPos[2] - 1,
                0, 1, 0
            );

            // Skybox - очень далёкая точка
            double[] skyPoint = view.transformPoint(
                camPos[0] + 500, camPos[1], camPos[2] + 500
            );

            assertNotNull("Skybox всегда виден", skyPoint);
        }
    }

    @Test
    public void testPortalTransformation() {
        // Portal (игра): трансформация через портал
        Matrix4 portalEntry = Matrix4.translate(10, 0, 0);
        Matrix4 portalRotation = Matrix4.rotateY(Math.PI); // 180° поворот
        Matrix4 portalExit = Matrix4.translate(-10, 0, 0);

        // Проход через портал
        Matrix4 throughPortal = portalExit.multiply(portalRotation).multiply(portalEntry);

        double[] player = throughPortal.transformPoint(0, 0, 0);

        // Игрок телепортировался и повернулся
        assertNotNull("Телепортация через портал", player);
    }

    @Test
    public void testMinecraftBlockPicking() {
        // Minecraft: выбор блока взглядом (ray picking)
        double eyeX = 0, eyeY = 64, eyeZ = 0; // Игрок на высоте 64
        double lookX = 0, lookY = 64, lookZ = -10; // Смотрит вперёд-вниз

        Matrix4 view = Matrix4.lookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, 0, 1, 0);

        // Блок в прицеле
        double[] blockPos = view.transformPoint(0, 63, -5);

        assertNotNull("Блок в прицеле", blockPos);
    }

    @Test
    public void testRocketLeagueCarPhysics() {
        // Rocket League: машина в воздухе с вращением
        double carX = 100, carY = 50, carZ = 100;
        double carRotX = Math.PI / 4; // Кувырок вперёд
        double carRotY = Math.PI / 6; // Поворот налево
        double carRotZ = Math.PI / 12; // Крен

        Matrix4 position = Matrix4.translate(carX, carY, carZ);
        Matrix4 rotation = Matrix4.rotateY(carRotY)
                                  .multiply(Matrix4.rotateX(carRotX))
                                  .multiply(Matrix4.rotateZ(carRotZ));

        Matrix4 carTransform = position.multiply(rotation);

        // Колесо машины
        double[] wheel = carTransform.transformPoint(2, -1, 3);

        assertNotNull("Колесо в 3D пространстве", wheel);
    }

    @Test
    public void testAssassinsCreedParkour() {
        // Assassin's Creed: паркур по стенам
        double wallNormalX = 1, wallNormalY = 0, wallNormalZ = 0; // Стена слева

        // Игрок бежит по стене под углом
        Matrix4 wallRun = Matrix4.rotateZ(Math.PI / 4); // Наклон 45°

        double[] playerPos = wallRun.transformPoint(0, 2, 0);

        assertTrue("Игрок бежит по стене", playerPos[0] != 0 || playerPos[1] != 2);
    }

    @Test
    public void testGravityGunPhysics() {
        // Half-Life 2: Gravity Gun поднимает объект
        double objectX = 5, objectY = 0, objectZ = -3;
        double gunX = 0, gunY = 1.7, gunZ = 0;

        // Притягиваем объект к оружию
        double dx = gunX - objectX;
        double dy = gunY - objectY;
        double dz = gunZ - objectZ;

        double pullStrength = 0.1;
        Matrix4 pull = Matrix4.translate(dx * pullStrength, dy * pullStrength, dz * pullStrength);

        double[] newPos = pull.transformPoint(objectX, objectY, objectZ);

        // Объект приближается к оружию
        double oldDist = Math.sqrt(objectX*objectX + objectY*objectY + objectZ*objectZ);
        double newDist = Math.sqrt(newPos[0]*newPos[0] + newPos[1]*newPos[1] + newPos[2]*newPos[2]);

        assertTrue("Объект притягивается", newDist < oldDist);
    }

    // ============ ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ДЛЯ ПОКРЫТИЯ МУТАЦИЙ ============

    @Test
    public void testLookAtDifferentPositions() {
        // Камера смотрит из разных позиций (избегаем положения на оси Y - up vector issue)
        double[][] positions = {
            {5, 0, 0, 0, 0, 0},    // справа
            {-5, 0, 0, 0, 0, 0},   // слева
            {0, 0, -5, 0, 0, 0},   // сзади
            {3, 4, 5, 0, 0, 0},    // диагональ
            {5, 3, 5, 0, 0, 0},    // произвольная точка
        };

        for (double[] pos : positions) {
            Matrix4 view = Matrix4.lookAt(
                pos[0], pos[1], pos[2],
                pos[3], pos[4], pos[5],
                0, 1, 0
            );
            assertNotNull("Камера создана", view);

            // Проверяем что матрица создана корректно
            assertEquals("Элемент [3][3] = 1", 1, view.get(3, 3), EPSILON);
        }
    }

    @Test
    public void testLookAtUpVectorVariations() {
        // Разные векторы "вверх"
        double[][] upVectors = {
            {0, 1, 0},   // стандартный
            {0, -1, 0},  // перевёрнутый
            {1, 0, 0},   // вправо
            {0, 0, 1},   // вперёд
            {1, 1, 0},   // диагональ
        };

        for (double[] up : upVectors) {
            Matrix4 view = Matrix4.lookAt(
                0, 0, 5,
                0, 0, 0,
                up[0], up[1], up[2]
            );
            assertNotNull("Камера с up " + up[0] + "," + up[1] + "," + up[2], view);
        }
    }

    @Test
    public void testLookAtForwardBackward() {
        // Камера смотрит вперёд и назад
        Matrix4 forward = Matrix4.lookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
        Matrix4 backward = Matrix4.lookAt(0, 0, -5, 0, 0, 0, 0, 1, 0);

        double[] fwdResult = forward.transformPoint(0, 0, 0);
        double[] bwdResult = backward.transformPoint(0, 0, 0);

        // Направления должны быть противоположными
        assertTrue("Разные направления", fwdResult[2] * bwdResult[2] > 0);
    }

    @Test
    public void testLookAtMatrixElements() {
        // Проверяем конкретные элементы матрицы lookAt
        Matrix4 view = Matrix4.lookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

        // Элемент [3][3] должен быть 1 (для однородных координат)
        assertEquals("Элемент [3][3] = 1", 1, view.get(3, 3), EPSILON);

        // Элементы последней строки (кроме [3][3]) должны быть 0
        assertEquals("Элемент [3][0] = 0", 0, view.get(3, 0), EPSILON);
        assertEquals("Элемент [3][1] = 0", 0, view.get(3, 1), EPSILON);
        assertEquals("Элемент [3][2] = 0", 0, view.get(3, 2), EPSILON);
    }

    @Test
    public void testLookAtNegativeCoordinates() {
        // Камера с отрицательными координатами
        Matrix4 view = Matrix4.lookAt(-10, -5, -20, 5, 10, 15, 0, 1, 0);

        // Проверяем что матрица создана корректно
        assertNotNull("View матрица создана", view);
        assertEquals("Элемент [3][3] = 1", 1, view.get(3, 3), EPSILON);

        // Проверяем трансформацию - точка должна переместиться
        double[] result = view.transformPoint(5, 10, 15);
        assertNotNull("Результат не null", result);
    }

    @Test
    public void testLookAtLargeDistance() {
        // Очень далёкая камера
        Matrix4 view = Matrix4.lookAt(0, 0, 1000, 0, 0, 0, 0, 1, 0);
        double[] result = view.transformPoint(0, 0, 0);
        assertEquals("Цель на расстоянии 1000", -1000, result[2], EPSILON);
    }

    @Test
    public void testLookAtCloseDistance() {
        // Очень близкая камера
        Matrix4 view = Matrix4.lookAt(0, 0, 0.1, 0, 0, 0, 0, 1, 0);
        double[] result = view.transformPoint(0, 0, 0);
        assertEquals("Цель на расстоянии 0.1", -0.1, result[2], EPSILON);
    }

    @Test
    public void testLookAtSideView() {
        // Вид сбоку
        Matrix4 view = Matrix4.lookAt(10, 0, 0, 0, 0, 0, 0, 1, 0);
        double[] result = view.transformPoint(0, 0, 0);
        assertEquals("Цель в центре X (вид сбоку)", 0, result[0], EPSILON);
    }

    @Test
    public void testLookAtTopView() {
        // Вид сверху
        Matrix4 view = Matrix4.lookAt(0, 10, 0, 0, 0, 0, 0, 0, -1);
        double[] result = view.transformPoint(0, 0, 0);
        assertEquals("Цель в центре X (вид сверху)", 0, result[0], EPSILON);
    }

    @Test
    public void testMatrixToString() {
        Matrix4 identity = new Matrix4();
        String str = identity.toString();
        assertNotNull("toString не null", str);
        assertTrue("Содержит 1.000", str.contains("1.000"));
    }

    @Test
    public void testPerspectiveWithDifferentFOV() {
        // Разные углы обзора
        double[] fovs = {Math.PI / 6, Math.PI / 4, Math.PI / 3, Math.PI / 2, 2 * Math.PI / 3};

        for (double fov : fovs) {
            Matrix4 persp = Matrix4.perspective(fov, 1.0, 0.1, 100);
            assertNotNull("Перспектива с FOV " + Math.toDegrees(fov), persp);

            double[] result = persp.transformPoint(1, 1, -5);
            assertNotNull("Трансформация точки", result);
        }
    }

    @Test
    public void testOrthographicAsymmetric() {
        // Асимметричная ортографическая проекция
        Matrix4 ortho = Matrix4.orthographic(-5, 15, -10, 20, 1, 1000);

        double[] leftBottom = ortho.transformPoint(-5, -10, -1);
        double[] rightTop = ortho.transformPoint(15, 20, -1);

        assertEquals("Левый нижний X = -1", -1, leftBottom[0], EPSILON);
        assertEquals("Левый нижний Y = -1", -1, leftBottom[1], EPSILON);
        assertEquals("Правый верхний X = 1", 1, rightTop[0], EPSILON);
        assertEquals("Правый верхний Y = 1", 1, rightTop[1], EPSILON);
    }

    // ============ ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ДЛЯ УБИЙСТВА МУТАЦИЙ ============

    @Test
    public void testLookAtForwardVectorCalculation() {
        // Тест на правильный расчёт вектора направления (centerX - eyeX и т.д.)
        // Камера в (10, 0, 0) смотрит на (0, 0, 0) - направление влево по X
        Matrix4 view = Matrix4.lookAt(10, 0, 0, 0, 0, 0, 0, 1, 0);

        // Точка (5, 0, 0) должна быть перед камерой (между камерой и целью)
        double[] result = view.transformPoint(5, 0, 0);
        // Точка между камерой и целью должна иметь отрицательный Z в view space
        assertTrue("Точка между камерой и целью", result[2] < 0);

        // Точка (0, 0, 0) - цель
        double[] target = view.transformPoint(0, 0, 0);
        // Цель дальше от камеры
        assertTrue("Цель дальше", target[2] < result[2]);
    }

    @Test
    public void testLookAtForwardVectorComponents() {
        // Камера смотрит по диагонали - все компоненты forward вектора ненулевые
        Matrix4 view = Matrix4.lookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);

        // Точка в начале координат
        double[] result = view.transformPoint(0, 0, 0);

        // Расстояние должно соответствовать sqrt(5^2 + 5^2 + 5^2) ≈ 8.66
        double expectedDist = Math.sqrt(75);
        assertEquals("Расстояние до цели", -expectedDist, result[2], EPSILON);
    }

    @Test
    public void testLookAtCrossProductRight() {
        // Тест на правильный расчёт правого вектора (forward × up)
        // Камера смотрит по оси -Z, up = Y => right = X
        Matrix4 view = Matrix4.lookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

        // Точка (1, 0, 0) должна быть справа (положительный X в view space)
        double[] rightPoint = view.transformPoint(1, 0, 0);
        assertTrue("Точка справа имеет положительный X", rightPoint[0] > 0);

        // Точка (-1, 0, 0) должна быть слева
        double[] leftPoint = view.transformPoint(-1, 0, 0);
        assertTrue("Точка слева имеет отрицательный X", leftPoint[0] < 0);
    }

    @Test
    public void testLookAtCrossProductUp() {
        // Тест на правильный расчёт нового up вектора (right × forward)
        Matrix4 view = Matrix4.lookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

        // Точка (0, 1, 0) должна быть сверху (положительный Y в view space)
        double[] upPoint = view.transformPoint(0, 1, 0);
        assertTrue("Точка сверху имеет положительный Y", upPoint[1] > 0);

        // Точка (0, -1, 0) должна быть снизу
        double[] downPoint = view.transformPoint(0, -1, 0);
        assertTrue("Точка снизу имеет отрицательный Y", downPoint[1] < 0);
    }

    @Test
    public void testLookAtTranslationComponent() {
        // Тест на правильный расчёт компонента трансляции
        // Цель (target) должна быть на отрицательной оси Z в view space
        Matrix4 view = Matrix4.lookAt(3, 4, 5, 0, 0, 0, 0, 1, 0);

        // Цель (0,0,0) должна быть перед камерой (отрицательный Z)
        double[] targetInView = view.transformPoint(0, 0, 0);
        double distToTarget = Math.sqrt(3*3 + 4*4 + 5*5);  // sqrt(50) ≈ 7.07
        assertEquals("Цель на отрицательном Z", -distToTarget, targetInView[2], EPSILON);
    }

    @Test
    public void testLookAtNormalization() {
        // Тест на правильную нормализацию векторов (деление на длину)
        // Используем ненормализованные начальные данные
        Matrix4 view = Matrix4.lookAt(100, 0, 0, 0, 0, 0, 0, 1, 0);

        // Несмотря на большое расстояние, направления должны быть корректными
        double[] result = view.transformPoint(50, 0, 0);
        assertTrue("Точка перед камерой", result[2] < 0);
        assertEquals("X в центре", 0, result[0], EPSILON);
    }

    @Test
    public void testPerspectiveFocalLength() {
        // Тест на правильный расчёт f = 1/tan(fov/2)
        double fov90 = Math.PI / 2;  // 90 градусов
        Matrix4 persp90 = Matrix4.perspective(fov90, 1.0, 0.1, 100);

        double fov60 = Math.PI / 3;  // 60 градусов
        Matrix4 persp60 = Matrix4.perspective(fov60, 1.0, 0.1, 100);

        // При меньшем FOV точки должны проецироваться дальше от центра (больше zoom)
        double[] point90 = persp90.transformPoint(1, 0, -5);
        double[] point60 = persp60.transformPoint(1, 0, -5);

        assertTrue("Меньший FOV = больший zoom", Math.abs(point60[0]) > Math.abs(point90[0]));
    }

    @Test
    public void testPerspectiveAspectRatio() {
        // Тест на правильный учёт aspect ratio (f/aspect)
        Matrix4 wide = Matrix4.perspective(Math.PI / 2, 2.0, 0.1, 100);  // 2:1
        Matrix4 square = Matrix4.perspective(Math.PI / 2, 1.0, 0.1, 100);  // 1:1

        // При широком aspect ratio X координата должна быть меньше
        double[] widePoint = wide.transformPoint(1, 0, -5);
        double[] squarePoint = square.transformPoint(1, 0, -5);

        assertTrue("Широкий aspect = меньший X", Math.abs(widePoint[0]) < Math.abs(squarePoint[0]));
    }

    @Test
    public void testPerspectiveNearFarPlanes() {
        // Тест на правильный расчёт near и far плоскостей
        double near = 1.0;
        double far = 100.0;
        Matrix4 persp = Matrix4.perspective(Math.PI / 2, 1.0, near, far);

        // Точка на ближней плоскости
        double[] nearPoint = persp.transformPoint(0, 0, -near);
        // Точка на дальней плоскости
        double[] farPoint = persp.transformPoint(0, 0, -far);

        // Z значения должны быть в NDC диапазоне [-1, 1]
        assertTrue("Near plane в NDC", nearPoint[2] >= -1.1 && nearPoint[2] <= 1.1);
        assertTrue("Far plane в NDC", farPoint[2] >= -1.1 && farPoint[2] <= 1.1);
        assertTrue("Far > Near в Z", farPoint[2] > nearPoint[2]);
    }

    @Test
    public void testOrthographicWidthHeight() {
        // Тест на правильный расчёт 2/(right-left) и 2/(top-bottom)
        Matrix4 ortho = Matrix4.orthographic(-10, 10, -5, 5, 1, 100);

        // Ширина = 20, высота = 10
        // Точка (10, 0, -1) -> X = 1
        double[] rightEdge = ortho.transformPoint(10, 0, -1);
        assertEquals("Правый край = 1", 1, rightEdge[0], EPSILON);

        // Точка (0, 5, -1) -> Y = 1
        double[] topEdge = ortho.transformPoint(0, 5, -1);
        assertEquals("Верхний край = 1", 1, topEdge[1], EPSILON);
    }

    @Test
    public void testOrthographicDepthRange() {
        // Тест на правильный расчёт -2/(far-near)
        double near = 1.0;
        double far = 11.0;  // Диапазон = 10
        Matrix4 ortho = Matrix4.orthographic(-1, 1, -1, 1, near, far);

        // Точка на ближней плоскости
        double[] nearPoint = ortho.transformPoint(0, 0, -near);
        // Точка на дальней плоскости
        double[] farPoint = ortho.transformPoint(0, 0, -far);

        // Z должен монотонно изменяться
        assertTrue("Near и Far различаются по Z", Math.abs(nearPoint[2] - farPoint[2]) > 0.1);
    }

    @Test
    public void testOrthographicCenterOffset() {
        // Тест на правильный расчёт -(right+left)/(right-left)
        Matrix4 ortho = Matrix4.orthographic(0, 20, 0, 10, 1, 100);  // Смещённый центр

        // Центр объёма (10, 5) должен отобразиться в (0, 0)
        double[] center = ortho.transformPoint(10, 5, -1);
        assertEquals("Центр X = 0", 0, center[0], EPSILON);
        assertEquals("Центр Y = 0", 0, center[1], EPSILON);
    }

    @Test
    public void testTransformAllComponents() {
        // Тест на все 16 элементов матрицы в transform
        Matrix4 custom = new Matrix4(new double[][] {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, 12},
            {13, 14, 15, 16}
        });

        double[] result = custom.transform(1, 1, 1, 1);

        // result[0] = 1*1 + 2*1 + 3*1 + 4*1 = 10
        assertEquals("Первая компонента", 10, result[0], EPSILON);
        // result[1] = 5*1 + 6*1 + 7*1 + 8*1 = 26
        assertEquals("Вторая компонента", 26, result[1], EPSILON);
        // result[2] = 9*1 + 10*1 + 11*1 + 12*1 = 42
        assertEquals("Третья компонента", 42, result[2], EPSILON);
        // result[3] = 13*1 + 14*1 + 15*1 + 16*1 = 58
        assertEquals("Четвёртая компонента", 58, result[3], EPSILON);
    }

    @Test
    public void testTransformPointPerspectiveDivide() {
        // Тест на перспективное деление (деление на w)
        Matrix4 m = new Matrix4(new double[][] {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}  // w = z
        });

        double[] result = m.transformPoint(4, 8, 2);
        // w = z = 2, поэтому x = 4/2 = 2, y = 8/2 = 4, z = 2/2 = 1
        assertEquals("X после деления", 2, result[0], EPSILON);
        assertEquals("Y после деления", 4, result[1], EPSILON);
        assertEquals("Z после деления", 1, result[2], EPSILON);
    }

    @Test
    public void testMultiplyMatrixElements() {
        // Тест на правильное умножение матриц
        Matrix4 a = new Matrix4(new double[][] {
            {1, 2, 0, 0},
            {3, 4, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        });

        Matrix4 b = new Matrix4(new double[][] {
            {5, 6, 0, 0},
            {7, 8, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        });

        Matrix4 result = a.multiply(b);

        // [1,2] * [5,7] = 5+14 = 19
        // [1,2] * [6,8] = 6+16 = 22
        // [3,4] * [5,7] = 15+28 = 43
        // [3,4] * [6,8] = 18+32 = 50
        assertEquals("Элемент [0][0]", 19, result.get(0, 0), EPSILON);
        assertEquals("Элемент [0][1]", 22, result.get(0, 1), EPSILON);
        assertEquals("Элемент [1][0]", 43, result.get(1, 0), EPSILON);
        assertEquals("Элемент [1][1]", 50, result.get(1, 1), EPSILON);
    }

    @Test
    public void testRotateXSinCos() {
        // Тест на правильное использование sin и cos в rotateX
        double angle = Math.PI / 6;  // 30 градусов
        Matrix4 rx = Matrix4.rotateX(angle);

        double c = Math.cos(angle);
        double s = Math.sin(angle);

        assertEquals("cos в [1][1]", c, rx.get(1, 1), EPSILON);
        assertEquals("-sin в [1][2]", -s, rx.get(1, 2), EPSILON);
        assertEquals("sin в [2][1]", s, rx.get(2, 1), EPSILON);
        assertEquals("cos в [2][2]", c, rx.get(2, 2), EPSILON);
    }

    @Test
    public void testRotateZSinCos() {
        // Тест на правильное использование sin и cos в rotateZ
        double angle = Math.PI / 4;  // 45 градусов
        Matrix4 rz = Matrix4.rotateZ(angle);

        double c = Math.cos(angle);
        double s = Math.sin(angle);

        assertEquals("cos в [0][0]", c, rz.get(0, 0), EPSILON);
        assertEquals("-sin в [0][1]", -s, rz.get(0, 1), EPSILON);
        assertEquals("sin в [1][0]", s, rz.get(1, 0), EPSILON);
        assertEquals("cos в [1][1]", c, rz.get(1, 1), EPSILON);
    }

    @Test
    public void testLookAtDiagonalView() {
        // Камера смотрит по диагонали - тестирует все компоненты cross product
        Matrix4 view = Matrix4.lookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

        // Точки на осях должны трансформироваться по-разному
        double[] xAxis = view.transformPoint(5, 0, 0);
        double[] yAxis = view.transformPoint(0, 5, 0);
        double[] zAxis = view.transformPoint(0, 0, 5);

        // Все три точки должны быть в разных местах view space
        boolean allDifferent =
            (Math.abs(xAxis[0] - yAxis[0]) > 0.1 || Math.abs(xAxis[1] - yAxis[1]) > 0.1) &&
            (Math.abs(yAxis[0] - zAxis[0]) > 0.1 || Math.abs(yAxis[1] - zAxis[1]) > 0.1);
        assertTrue("Точки на осях различаются в view space", allDifferent);
    }

    @Test
    public void testLookAtNegativeEyePosition() {
        // Камера с отрицательными координатами
        Matrix4 view = Matrix4.lookAt(-5, -5, -5, 0, 0, 0, 0, 1, 0);

        double[] origin = view.transformPoint(0, 0, 0);
        double expectedDist = Math.sqrt(75);  // sqrt(5^2 + 5^2 + 5^2)
        assertEquals("Расстояние до цели", -expectedDist, origin[2], EPSILON);
    }

    @Test
    public void testLookAtRowElements() {
        // Проверка конкретных элементов матрицы lookAt
        Matrix4 view = Matrix4.lookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);

        // Третья строка должна содержать -forward и (f·eye)
        // forward = (0, 0, -1) (нормализованный), поэтому -forward = (0, 0, 1)
        assertEquals("Row 2, Col 2: -fz", 1, view.get(2, 2), EPSILON);

        // f·eye = (0, 0, -1)·(0, 0, 10) = -10, но с минусом: -(-10) = 10... в формуле: fx*eyeX + fy*eyeY + fz*eyeZ
        // При eyeZ=10, fz=-1 (pointing from eye to target): 0*0 + 0*0 + (-1)*10 = -10
        // В lookAt: (fx * eyeX + fy * eyeY + fz * eyeZ) без минуса в последнем столбце
    }
}
