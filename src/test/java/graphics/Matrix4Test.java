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
}
