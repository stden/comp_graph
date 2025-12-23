package graphics;

import graphics.OptimizedRayTracer.*;
import graphics.RayTracer.*;
import math3d.Vector3D;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Tests for OptimizedRayTracer with BVH acceleration
 */
public class OptimizedRayTracerTest {
    private static final float EPSILON = 0.001f;

    // ==================== AABB Tests ====================

    @Test
    public void testAABBCreation() {
        Vector3D min = new Vector3D(-1, -1, -1);
        Vector3D max = new Vector3D(1, 1, 1);

        AABB box = new AABB(min, max);

        assertEquals("Min X", -1, box.min.x, EPSILON);
        assertEquals("Max X", 1, box.max.x, EPSILON);
    }

    @Test
    public void testAABBHitDirect() {
        AABB box = new AABB(
            new Vector3D(-1, -1, -1),
            new Vector3D(1, 1, 1)
        );

        Ray ray = new Ray(
            new Vector3D(0, 0, -5),
            new Vector3D(0, 0, 1)
        );

        assertTrue("Ray hits AABB", box.hit(ray, 0, Float.MAX_VALUE));
    }

    @Test
    public void testAABBMiss() {
        AABB box = new AABB(
            new Vector3D(-1, -1, -1),
            new Vector3D(1, 1, 1)
        );

        Ray ray = new Ray(
            new Vector3D(5, 5, -5),
            new Vector3D(0, 0, 1)
        );

        assertFalse("Ray misses AABB", box.hit(ray, 0, Float.MAX_VALUE));
    }

    @Test
    public void testAABBHitFromInside() {
        AABB box = new AABB(
            new Vector3D(-1, -1, -1),
            new Vector3D(1, 1, 1)
        );

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)
        );

        assertTrue("Ray from inside hits AABB", box.hit(ray, 0, Float.MAX_VALUE));
    }

    @Test
    public void testAABBHitEdgeCase() {
        AABB box = new AABB(
            new Vector3D(0, 0, 0),
            new Vector3D(2, 2, 2)
        );

        // Ray through center of box
        Ray ray = new Ray(
            new Vector3D(1, 1, -1),
            new Vector3D(0, 0, 1)
        );

        assertTrue("Ray through center hits", box.hit(ray, 0, Float.MAX_VALUE));
    }

    @Test
    public void testAABBSurrounding() {
        AABB box1 = new AABB(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 1, 1)
        );
        AABB box2 = new AABB(
            new Vector3D(2, 2, 2),
            new Vector3D(3, 3, 3)
        );

        AABB surrounding = AABB.surrounding(box1, box2);

        assertEquals("Surrounding min X", 0, surrounding.min.x, EPSILON);
        assertEquals("Surrounding max X", 3, surrounding.max.x, EPSILON);
        assertEquals("Surrounding min Y", 0, surrounding.min.y, EPSILON);
        assertEquals("Surrounding max Y", 3, surrounding.max.y, EPSILON);
    }

    @Test
    public void testAABBHitNegativeDirection() {
        AABB box = new AABB(
            new Vector3D(-1, -1, -1),
            new Vector3D(1, 1, 1)
        );

        Ray ray = new Ray(
            new Vector3D(0, 0, 5),
            new Vector3D(0, 0, -1)
        );

        assertTrue("Ray with negative Z direction hits", box.hit(ray, 0, Float.MAX_VALUE));
    }

    @Test
    public void testAABBHitAllAxes() {
        AABB box = new AABB(
            new Vector3D(-1, -1, -1),
            new Vector3D(1, 1, 1)
        );

        // Diagonal ray
        Ray ray = new Ray(
            new Vector3D(-5, -5, -5),
            new Vector3D(1, 1, 1)
        );

        assertTrue("Diagonal ray hits", box.hit(ray, 0, Float.MAX_VALUE));
    }

    @Test
    public void testAABBHitWithTRange() {
        AABB box = new AABB(
            new Vector3D(4, -1, -1),
            new Vector3D(6, 1, 1)
        );

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)
        );

        // Box is at t=4-6, so tMin=7 should miss
        assertFalse("Ray misses with restricted tMin", box.hit(ray, 7, 10));
        // tMax=3 should miss
        assertFalse("Ray misses with restricted tMax", box.hit(ray, 0, 3));
        // Normal range should hit
        assertTrue("Ray hits with correct range", box.hit(ray, 0, 10));
    }

    // ==================== BoundedSphere Tests ====================

    @Test
    public void testBoundedSphereCreation() {
        Vector3D center = new Vector3D(0, 0, 5);
        BoundedSphere sphere = new BoundedSphere(center, 1.0f, Material.red());

        assertEquals("Center matches", 0, sphere.getCenter().x, EPSILON);
        assertNotNull("Has bounding box", sphere.getBoundingBox());
    }

    @Test
    public void testBoundedSphereBoundingBox() {
        Vector3D center = new Vector3D(0, 0, 0);
        BoundedSphere sphere = new BoundedSphere(center, 2.0f, Material.blue());

        AABB box = sphere.getBoundingBox();

        assertEquals("Min X", -2, box.min.x, EPSILON);
        assertEquals("Max X", 2, box.max.x, EPSILON);
        assertEquals("Min Y", -2, box.min.y, EPSILON);
        assertEquals("Max Y", 2, box.max.y, EPSILON);
        assertEquals("Min Z", -2, box.min.z, EPSILON);
        assertEquals("Max Z", 2, box.max.z, EPSILON);
    }

    @Test
    public void testBoundedSphereHit() {
        BoundedSphere sphere = new BoundedSphere(
            new Vector3D(0, 0, 5),
            1.0f,
            Material.red()
        );

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Sphere hit", hit);
        assertEquals("Hit distance", 4.0f, hit.t, EPSILON);
    }

    @Test
    public void testBoundedSphereMiss() {
        BoundedSphere sphere = new BoundedSphere(
            new Vector3D(0, 0, 5),
            1.0f,
            Material.red()
        );

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)  // Wrong direction
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);

        assertNull("Sphere miss", hit);
    }

    // ==================== BVH Tests ====================

    @Test
    public void testBVHSingleObject() {
        java.util.List<BoundedHittable> objects = new java.util.ArrayList<>();
        objects.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));

        BVHNode bvh = new BVHNode(objects, 0, objects.size());

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = bvh.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("BVH hit single object", hit);
    }

    @Test
    public void testBVHTwoObjects() {
        java.util.List<BoundedHittable> objects = new java.util.ArrayList<>();
        objects.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        objects.add(new BoundedSphere(new Vector3D(0, 0, 10), 1.0f, Material.blue()));

        BVHNode bvh = new BVHNode(objects, 0, objects.size());

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = bvh.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("BVH hit closest object", hit);
        assertEquals("Closest object is red", 1.0f, hit.material.r, EPSILON);
        assertEquals("Hit distance", 4.0f, hit.t, EPSILON);
    }

    @Test
    public void testBVHManyObjects() {
        java.util.List<BoundedHittable> objects = new java.util.ArrayList<>();
        for (int i = 0; i < 100; i++) {
            float z = 5 + i * 2;
            objects.add(new BoundedSphere(new Vector3D(0, 0, z), 0.5f, Material.red()));
        }

        BVHNode bvh = new BVHNode(objects, 0, objects.size());

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = bvh.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("BVH hit with many objects", hit);
        assertEquals("Closest object at z=5", 4.5f, hit.t, EPSILON);
    }

    @Test
    public void testBVHMissAllObjects() {
        java.util.List<BoundedHittable> objects = new java.util.ArrayList<>();
        objects.add(new BoundedSphere(new Vector3D(10, 10, 10), 1.0f, Material.red()));
        objects.add(new BoundedSphere(new Vector3D(-10, -10, 10), 1.0f, Material.blue()));

        BVHNode bvh = new BVHNode(objects, 0, objects.size());

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = bvh.hit(ray, 0, Float.MAX_VALUE);
        assertNull("BVH miss all objects", hit);
    }

    @Test
    public void testBVHBoundingBox() {
        java.util.List<BoundedHittable> objects = new java.util.ArrayList<>();
        objects.add(new BoundedSphere(new Vector3D(-5, 0, 0), 1.0f, Material.red()));
        objects.add(new BoundedSphere(new Vector3D(5, 0, 0), 1.0f, Material.blue()));

        BVHNode bvh = new BVHNode(objects, 0, objects.size());

        AABB box = bvh.getBoundingBox();
        assertNotNull("BVH has bounding box", box);
        assertTrue("BVH bounding box contains left sphere", box.min.x <= -4);
        assertTrue("BVH bounding box contains right sphere", box.max.x >= 4);
    }

    // ==================== OptimizedScene Tests ====================

    @Test
    public void testOptimizedSceneEmpty() {
        OptimizedScene scene = new OptimizedScene();
        scene.buildBVH();

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = scene.hit(ray, 0, Float.MAX_VALUE);
        assertNull("Empty scene returns null", hit);
    }

    @Test
    public void testOptimizedSceneWithObjects() {
        OptimizedScene scene = new OptimizedScene();
        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        scene.add(new BoundedSphere(new Vector3D(0, 0, 10), 1.0f, Material.blue()));
        scene.buildBVH();

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = scene.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Scene hit", hit);
        assertEquals("Closest sphere (red)", 1.0f, hit.material.r, EPSILON);
    }

    @Test
    public void testOptimizedSceneSize() {
        OptimizedScene scene = new OptimizedScene();
        assertEquals("Empty scene size", 0, scene.size());

        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        assertEquals("Scene size after add", 1, scene.size());
    }

    @Test
    public void testOptimizedSceneWithoutBuild() {
        OptimizedScene scene = new OptimizedScene();
        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        // Don't build BVH

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = scene.hit(ray, 0, Float.MAX_VALUE);
        assertNull("Scene without BVH returns null", hit);
    }

    // ==================== TraceRay Tests ====================

    @Test
    public void testTraceRayBackground() {
        OptimizedScene scene = new OptimizedScene();
        scene.buildBVH();

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 1, 0)
        );

        float[] color = OptimizedRayTracer.traceRay(ray, scene, 5);

        assertTrue("Background has color", color[0] > 0 || color[1] > 0 || color[2] > 0);
    }

    @Test
    public void testTraceRayHitObject() {
        OptimizedScene scene = new OptimizedScene();
        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        scene.buildBVH();

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        float[] color = OptimizedRayTracer.traceRay(ray, scene, 5);

        assertTrue("Red component > green", color[0] > color[1]);
        assertTrue("Red component > blue", color[0] > color[2]);
    }

    @Test
    public void testTraceRayDepthZero() {
        OptimizedScene scene = new OptimizedScene();
        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        scene.buildBVH();

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        float[] color = OptimizedRayTracer.traceRay(ray, scene, 0);

        assertEquals("Depth 0 returns black R", 0, color[0], EPSILON);
        assertEquals("Depth 0 returns black G", 0, color[1], EPSILON);
        assertEquals("Depth 0 returns black B", 0, color[2], EPSILON);
    }

    @Test
    public void testTraceRayReflection() {
        OptimizedScene scene = new OptimizedScene();
        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.mirror()));
        scene.buildBVH();

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        float[] color = OptimizedRayTracer.traceRay(ray, scene, 5);

        // Mirror should reflect something (background or other color)
        assertTrue("Mirror reflects something", color[0] > 0 || color[1] > 0 || color[2] > 0);
    }

    // ==================== Batch Tracing Tests ====================

    @Test
    public void testTraceRaysBatch() {
        OptimizedScene scene = new OptimizedScene();
        for (int i = 0; i < 10; i++) {
            scene.add(new BoundedSphere(
                new Vector3D(i - 5, 0, 10),
                1.0f,
                Material.red()
            ));
        }
        scene.buildBVH();

        long count = OptimizedRayTracer.traceRaysBatch(scene, 100, 3);

        assertTrue("Some rays hit objects", count > 0);
    }

    // ==================== ParallelRenderer Tests ====================

    @Test
    public void testParallelRendererCreation() {
        ParallelRenderer renderer = new ParallelRenderer();
        assertNotNull("Renderer created", renderer);
        renderer.shutdown();
    }

    @Test
    public void testParallelRendererWithThreads() {
        ParallelRenderer renderer = new ParallelRenderer(4);
        assertNotNull("Renderer created with threads", renderer);
        renderer.shutdown();
    }

    @Test
    public void testParallelRender() {
        OptimizedScene scene = new OptimizedScene();
        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 2.0f, Material.red()));
        scene.buildBVH();

        ParallelRenderer renderer = new ParallelRenderer(2);
        float[][][] image = renderer.render(scene, 10, 10, 3);
        renderer.shutdown();

        assertNotNull("Image rendered", image);
        assertEquals("Image height", 10, image.length);
        assertEquals("Image width", 10, image[0].length);
        assertEquals("Pixel has 3 components", 3, image[0][0].length);
    }

    @Test
    public void testParallelRenderSmallImage() {
        OptimizedScene scene = new OptimizedScene();
        scene.add(new BoundedSphere(new Vector3D(0, 0, 5), 1.0f, Material.blue()));
        scene.buildBVH();

        ParallelRenderer renderer = new ParallelRenderer(1);
        float[][][] image = renderer.render(scene, 4, 4, 2);
        renderer.shutdown();

        // Check center pixel (should hit the sphere)
        float[] centerPixel = image[2][2];
        assertTrue("Center pixel has color",
            centerPixel[0] > 0 || centerPixel[1] > 0 || centerPixel[2] > 0);
    }
}
