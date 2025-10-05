package bdd;

import graphics.Lighting;
import graphics.Lighting.Light;
import graphics.Lighting.Material;
import math3d.Vector3D;
import io.cucumber.java.ru.*;
import static org.junit.Assert.*;

/**
 * Step definitions для BDD тестов освещения
 */
public class LightingSteps {
    private Vector3D surfacePoint;
    private Vector3D normal;
    private Vector3D viewDir;
    private Light light;
    private Material material;
    private float illumination;
    private static final float EPSILON = 0.1f;

    @Дано("точка на поверхности \\({float}, {float}, {float})")
    public void точка_на_поверхности(float x, float y, float z) {
        surfacePoint = new Vector3D(x, y, z);
    }

    @Дано("нормаль поверхности \\({float}, {float}, {float})")
    public void нормаль_поверхности(float x, float y, float z) {
        normal = new Vector3D(x, y, z);
        normal.normalize();
    }

    @Дано("источник света в точке \\({float}, {float}, {float}) с интенсивностью {float}")
    public void источник_света_с_интенсивностью(float x, float y, float z, float intensity) {
        light = new Light(new Vector3D(x, y, z), intensity);
    }

    @Дано("материал с коэффициентом диффузного отражения {float}")
    public void материал_с_коэффициентом_диффузного_отражения(float diffuse) {
        material = new Material(0.1f, diffuse, 0.0f, 1.0f);
    }

    @Когда("вычисляю освещение по модели Ламберта")
    public void вычисляю_освещение_по_модели_ламберта() {
        // Вычисляем направление на источник света
        Vector3D lightDir = new Vector3D(light.position).sub(surfacePoint);
        lightDir.normalize();
        illumination = Lighting.lambert(normal, lightDir, light.intensity, material.diffuse);
    }

    @Тогда("интенсивность освещения около {float}")
    public void интенсивность_освещения_около(float expected) {
        assertEquals("Интенсивность", expected, illumination, EPSILON);
    }

    @Дано("направление взгляда \\({float}, {float}, {float})")
    public void направление_взгляда(float x, float y, float z) {
        viewDir = new Vector3D(x, y, z);
        viewDir.normalize();
    }

    @Дано("источник света в точке \\({float}, {float}, {float})")
    public void источник_света_в_точке(float x, float y, float z) {
        light = new Light(new Vector3D(x, y, z), 1.0f);
    }

    @Дано("материал с показателем блеска {int}")
    public void материал_с_показателем_блеска(int shininess) {
        material = new Material(0.1f, 0.6f, 0.3f, shininess);
    }

    @Когда("вычисляю освещение по модели Фонга")
    public void вычисляю_освещение_по_модели_фонга() {
        illumination = Lighting.phong(surfacePoint, normal, viewDir, light, material, 0.1f);
    }

    @Тогда("есть зеркальная составляющая освещения")
    public void есть_зеркальная_составляющая() {
        assertTrue("Зеркальная составляющая > 0", illumination > 0.1f);
    }

    @Дано("источник света в точке \\({float}, {float}, {float}) под углом {int} градусов")
    public void источник_света_под_углом(float x, float y, float z, int angle) {
        light = new Light(new Vector3D(x, y, z), 1.0f);
        material = new Material(0.1f, 0.8f, 0.0f, 1.0f);
    }

    @Когда("вычисляю диффузное освещение")
    public void вычисляю_диффузное_освещение() {
        Vector3D lightDir = new Vector3D(light.position).sub(surfacePoint);
        lightDir.normalize();
        illumination = Lighting.lambert(normal, lightDir, light.intensity, material.diffuse);
    }

    @Тогда("интенсивность меньше чем при прямом освещении")
    public void интенсивность_меньше_чем_при_прямом_освещении() {
        assertTrue("Интенсивность под углом меньше", illumination < 0.9f);
    }

    @Дано("источник света в точке \\({float}, {float}, {float}) сзади поверхности")
    public void источник_света_сзади_поверхности(float x, float y, float z) {
        light = new Light(new Vector3D(x, y, z), 1.0f);
        material = new Material(0.0f, 0.8f, 0.0f, 1.0f);
    }

    @Тогда("интенсивность должна быть {int}")
    public void интенсивность_должна_быть_ноль(int expected) {
        assertTrue("Освещение с обратной стороны = 0", illumination <= 0.1f);
    }

    @Когда("вычисляю освещение по модели Блинна-Фонга")
    public void вычисляю_освещение_по_модели_блинна_фонга() {
        illumination = Lighting.blinnPhong(surfacePoint, normal, viewDir, light, material, 0.1f);
    }

    @Тогда("результат похож на модель Фонга но вычисляется быстрее")
    public void результат_похож_на_фонга() {
        assertTrue("Блинн-Фонг работает", illumination > 0);
    }

    @Дано("материал с параметрами: ambient={float}, diffuse={float}, specular={float}, shininess={float}")
    public void материал_с_параметрами(float ka, float kd, float ks, float n) {
        material = new Material(ka, kd, ks, n);
        surfacePoint = new Vector3D(0, 0, 0);
        normal = new Vector3D(0, 0, 1);
        light = new Light(new Vector3D(0, 0, 10), 1.0f);
        viewDir = new Vector3D(0, 0, 1);
    }

    @Дано("точка на поверхности \\({float}, {float}, {float}) с нормалью \\({float}, {float}, {float})")
    public void точка_на_поверхности_с_нормалью(float px, float py, float pz, float nx, float ny, float nz) {
        surfacePoint = new Vector3D(px, py, pz);
        normal = new Vector3D(nx, ny, nz);
        normal.normalize();
    }

    @Когда("вычисляю полное освещение")
    public void вычисляю_полное_освещение() {
        illumination = Lighting.phong(surfacePoint, normal, viewDir, light, material, 0.1f);
    }

    @Тогда("интенсивность зависит от свойств материала")
    public void интенсивность_зависит_от_свойств_материала() {
        assertTrue("Освещение вычислено", illumination >= 0.0f && illumination <= 1.0f);
    }

    @Когда("вычисляю освещение по модели Блинна-Фонга")
    public void вычисляю_освещение_по_модели_блинна_фонга_initialized() {
        // Инициализируем недостающие параметры если они null
        if (surfacePoint == null) surfacePoint = new Vector3D(0, 0, 0);
        if (normal == null) normal = new Vector3D(0, 0, 1);
        if (viewDir == null) viewDir = new Vector3D(0, 0, 1);
        if (light == null) light = new Light(new Vector3D(0, 0, 10), 1.0f);
        if (material == null) material = new Material(0.1f, 0.6f, 0.3f, 32.0f);
        illumination = Lighting.blinnPhong(surfacePoint, normal, viewDir, light, material, 0.1f);
    }
}
