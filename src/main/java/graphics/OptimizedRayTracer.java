package graphics;

import math3d.Vector3D;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.*;

/**
 * Optimized Ray Tracer with:
 * 1. BVH (Bounding Volume Hierarchy) for O(log n) intersection tests
 * 2. Multi-threading for parallel ray processing
 */
public class OptimizedRayTracer {

    /**
     * Axis-Aligned Bounding Box (AABB)
     */
    public static class AABB {
        public Vector3D min;
        public Vector3D max;

        public AABB(Vector3D min, Vector3D max) {
            this.min = new Vector3D(min);
            this.max = new Vector3D(max);
        }

        public static AABB surrounding(AABB a, AABB b) {
            Vector3D min = new Vector3D(
                Math.min(a.min.x, b.min.x),
                Math.min(a.min.y, b.min.y),
                Math.min(a.min.z, b.min.z)
            );
            Vector3D max = new Vector3D(
                Math.max(a.max.x, b.max.x),
                Math.max(a.max.y, b.max.y),
                Math.max(a.max.z, b.max.z)
            );
            return new AABB(min, max);
        }

        /**
         * Fast ray-AABB intersection test (slab method)
         */
        public boolean hit(RayTracer.Ray ray, float tMin, float tMax) {
            // X axis
            float invD = 1.0f / ray.direction.x;
            float t0 = (min.x - ray.origin.x) * invD;
            float t1 = (max.x - ray.origin.x) * invD;
            if (invD < 0) { float tmp = t0; t0 = t1; t1 = tmp; }
            tMin = Math.max(t0, tMin);
            tMax = Math.min(t1, tMax);
            if (tMax <= tMin) return false;

            // Y axis
            invD = 1.0f / ray.direction.y;
            t0 = (min.y - ray.origin.y) * invD;
            t1 = (max.y - ray.origin.y) * invD;
            if (invD < 0) { float tmp = t0; t0 = t1; t1 = tmp; }
            tMin = Math.max(t0, tMin);
            tMax = Math.min(t1, tMax);
            if (tMax <= tMin) return false;

            // Z axis
            invD = 1.0f / ray.direction.z;
            t0 = (min.z - ray.origin.z) * invD;
            t1 = (max.z - ray.origin.z) * invD;
            if (invD < 0) { float tmp = t0; t0 = t1; t1 = tmp; }
            tMin = Math.max(t0, tMin);
            tMax = Math.min(t1, tMax);

            return tMax > tMin;
        }
    }

    /**
     * Interface for objects that can be bounded
     */
    public interface BoundedHittable extends RayTracer.Hittable {
        AABB getBoundingBox();
    }

    /**
     * Bounded Sphere implementation
     */
    public static class BoundedSphere implements BoundedHittable {
        private final RayTracer.Sphere sphere;
        private final AABB boundingBox;

        public BoundedSphere(Vector3D center, float radius, RayTracer.Material material) {
            this.sphere = new RayTracer.Sphere(center, radius, material);
            Vector3D radiusVec = new Vector3D(radius, radius, radius);
            this.boundingBox = new AABB(
                new Vector3D(center).sub(radiusVec),
                new Vector3D(center).add(radiusVec)
            );
        }

        @Override
        public RayTracer.HitRecord hit(RayTracer.Ray ray, float tMin, float tMax) {
            return sphere.hit(ray, tMin, tMax);
        }

        @Override
        public AABB getBoundingBox() {
            return boundingBox;
        }

        public Vector3D getCenter() {
            return sphere.center;
        }
    }

    /**
     * BVH Node - binary tree for spatial acceleration
     */
    public static class BVHNode implements BoundedHittable {
        private final AABB boundingBox;
        private final BoundedHittable left;
        private final BoundedHittable right;

        public BVHNode(List<BoundedHittable> objects, int start, int end) {
            int axis = (int)(Math.random() * 3); // Random axis for splitting

            Comparator<BoundedHittable> comparator = (a, b) -> {
                AABB boxA = a.getBoundingBox();
                AABB boxB = b.getBoundingBox();
                float va = axis == 0 ? boxA.min.x : (axis == 1 ? boxA.min.y : boxA.min.z);
                float vb = axis == 0 ? boxB.min.x : (axis == 1 ? boxB.min.y : boxB.min.z);
                return Float.compare(va, vb);
            };

            int objectSpan = end - start;

            if (objectSpan == 1) {
                left = right = objects.get(start);
            } else if (objectSpan == 2) {
                if (comparator.compare(objects.get(start), objects.get(start + 1)) < 0) {
                    left = objects.get(start);
                    right = objects.get(start + 1);
                } else {
                    left = objects.get(start + 1);
                    right = objects.get(start);
                }
            } else {
                objects.subList(start, end).sort(comparator);
                int mid = start + objectSpan / 2;
                left = new BVHNode(objects, start, mid);
                right = new BVHNode(objects, mid, end);
            }

            boundingBox = AABB.surrounding(left.getBoundingBox(), right.getBoundingBox());
        }

        @Override
        public RayTracer.HitRecord hit(RayTracer.Ray ray, float tMin, float tMax) {
            if (!boundingBox.hit(ray, tMin, tMax)) {
                return null;
            }

            RayTracer.HitRecord leftHit = left.hit(ray, tMin, tMax);
            RayTracer.HitRecord rightHit = right.hit(ray, tMin, leftHit != null ? leftHit.t : tMax);

            if (rightHit != null) return rightHit;
            return leftHit;
        }

        @Override
        public AABB getBoundingBox() {
            return boundingBox;
        }
    }

    /**
     * Optimized Scene with BVH acceleration
     */
    public static class OptimizedScene {
        private BVHNode bvhRoot;
        private List<BoundedHittable> objects;

        public OptimizedScene() {
            this.objects = new ArrayList<>();
        }

        public void add(BoundedHittable object) {
            objects.add(object);
        }

        public void buildBVH() {
            if (!objects.isEmpty()) {
                bvhRoot = new BVHNode(new ArrayList<>(objects), 0, objects.size());
            }
        }

        public RayTracer.HitRecord hit(RayTracer.Ray ray, float tMin, float tMax) {
            if (bvhRoot == null) return null;
            return bvhRoot.hit(ray, tMin, tMax);
        }

        public int size() {
            return objects.size();
        }
    }

    /**
     * Trace a single ray through the scene
     */
    public static float[] traceRay(RayTracer.Ray ray, OptimizedScene scene, int depth) {
        if (depth <= 0) {
            return new float[]{0, 0, 0};
        }

        RayTracer.HitRecord hit = scene.hit(ray, 0.001f, Float.MAX_VALUE);

        if (hit == null) {
            float t = 0.5f * (ray.direction.y + 1.0f);
            return new float[]{1.0f - 0.5f * t, 1.0f - 0.3f * t, 1.0f};
        }

        Vector3D lightDir = new Vector3D(0, 1, 0);
        float intensity = Math.max(0.2f, hit.normal.dot(lightDir));

        float r = hit.material.r * intensity;
        float g = hit.material.g * intensity;
        float b = hit.material.b * intensity;

        if (hit.material.reflectivity > 0 && depth > 1) {
            Vector3D reflected = RayTracer.reflect(ray.direction, hit.normal);
            RayTracer.Ray reflectedRay = new RayTracer.Ray(hit.point, reflected);
            float[] reflectedColor = traceRay(reflectedRay, scene, depth - 1);

            float k = hit.material.reflectivity;
            r = r * (1 - k) + reflectedColor[0] * k;
            g = g * (1 - k) + reflectedColor[1] * k;
            b = b * (1 - k) + reflectedColor[2] * k;
        }

        return new float[]{r, g, b};
    }

    /**
     * Parallel image renderer using multiple threads
     */
    public static class ParallelRenderer {
        private final ExecutorService executor;
        private final int numThreads;

        public ParallelRenderer() {
            this.numThreads = Runtime.getRuntime().availableProcessors();
            this.executor = Executors.newFixedThreadPool(numThreads);
        }

        public ParallelRenderer(int numThreads) {
            this.numThreads = numThreads;
            this.executor = Executors.newFixedThreadPool(numThreads);
        }

        /**
         * Render image in parallel
         */
        public float[][][] render(OptimizedScene scene, int width, int height, int depth) {
            float[][][] image = new float[height][width][3];

            List<Future<?>> futures = new ArrayList<>();
            int rowsPerThread = height / numThreads;

            for (int t = 0; t < numThreads; t++) {
                final int startRow = t * rowsPerThread;
                final int endRow = (t == numThreads - 1) ? height : (t + 1) * rowsPerThread;

                futures.add(executor.submit(() -> {
                    renderRows(scene, image, width, height, startRow, endRow, depth);
                }));
            }

            // Wait for all tasks to complete
            for (Future<?> future : futures) {
                try {
                    future.get();
                } catch (InterruptedException | ExecutionException e) {
                    Thread.currentThread().interrupt();
                }
            }

            return image;
        }

        private void renderRows(OptimizedScene scene, float[][][] image,
                               int width, int height, int startRow, int endRow, int depth) {
            float aspectRatio = (float) width / height;
            float fov = 90.0f;
            float scale = (float) Math.tan(Math.toRadians(fov * 0.5));

            for (int y = startRow; y < endRow; y++) {
                for (int x = 0; x < width; x++) {
                    float px = (2 * (x + 0.5f) / width - 1) * aspectRatio * scale;
                    float py = (1 - 2 * (y + 0.5f) / height) * scale;

                    Vector3D origin = new Vector3D(0, 0, -5);
                    Vector3D direction = new Vector3D(px, py, 1);
                    RayTracer.Ray ray = new RayTracer.Ray(origin, direction);

                    float[] color = traceRay(ray, scene, depth);
                    image[y][x] = color;
                }
            }
        }

        public void shutdown() {
            executor.shutdown();
        }
    }

    /**
     * Single-threaded batch ray tracing (for benchmarking)
     */
    public static long traceRaysBatch(OptimizedScene scene, int numRays, int depth) {
        long count = 0;
        for (int i = 0; i < numRays; i++) {
            float px = (float)(Math.random() * 2 - 1);
            float py = (float)(Math.random() * 2 - 1);
            Vector3D origin = new Vector3D(0, 0, -5);
            Vector3D direction = new Vector3D(px, py, 1);
            RayTracer.Ray ray = new RayTracer.Ray(origin, direction);
            float[] color = traceRay(ray, scene, depth);
            if (color[0] > 0) count++;
        }
        return count;
    }
}
