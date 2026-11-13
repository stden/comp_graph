package graphics;

import graphics.RayTracer.*;
import graphics.OptimizedRayTracer.*;
import math3d.Vector3D;
import org.openjdk.jmh.annotations.*;

import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Benchmark)
@Fork(value = 2, jvmArgs = {"-Xms2G", "-Xmx2G"})
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
public class OptimizedRayTracerBenchmark {

    @State(Scope.Benchmark)
    public static class OriginalState {
        @Param({"100", "500", "1000", "2000", "5000"})
        public int objectCount;

        public Scene scene;
        public Ray ray;

        @Setup(Level.Trial)
        public void setup() {
            scene = new Scene();
            for (int i = 0; i < objectCount; i++) {
                float x = (float) (Math.random() * 20 - 10);
                float y = (float) (Math.random() * 20 - 10);
                float z = (float) (Math.random() * 20);
                float radius = (float) (Math.random() * 1 + 0.5);
                float r = (float) Math.random();
                float g = (float) Math.random();
                float b = (float) Math.random();
                float reflectivity = Math.random() > 0.8 ? 0.9f : 0.0f;
                scene.add(new Sphere(new Vector3D(x, y, z), radius, new Material(r, g, b, reflectivity)));
            }
            ray = new Ray(new Vector3D(0, 0, -5), new Vector3D(0, 0, 1));
        }
    }

    @State(Scope.Benchmark)
    public static class OptimizedState {
        @Param({"100", "500", "1000", "2000", "5000"})
        public int objectCount;

        public OptimizedScene scene;
        public Ray ray;

        @Setup(Level.Trial)
        public void setup() {
            scene = new OptimizedScene();
            for (int i = 0; i < objectCount; i++) {
                float x = (float) (Math.random() * 20 - 10);
                float y = (float) (Math.random() * 20 - 10);
                float z = (float) (Math.random() * 20);
                float radius = (float) (Math.random() * 1 + 0.5);
                float r = (float) Math.random();
                float g = (float) Math.random();
                float b = (float) Math.random();
                float reflectivity = Math.random() > 0.8 ? 0.9f : 0.0f;
                scene.add(new BoundedSphere(new Vector3D(x, y, z), radius, new Material(r, g, b, reflectivity)));
            }
            scene.buildBVH();
            ray = new Ray(new Vector3D(0, 0, -5), new Vector3D(0, 0, 1));
        }
    }

    @Benchmark
    public float[] testOriginalTraceRay(OriginalState state) {
        return RayTracer.traceRay(state.ray, state.scene, 5);
    }

    @Benchmark
    public float[] testOptimizedTraceRay(OptimizedState state) {
        return OptimizedRayTracer.traceRay(state.ray, state.scene, 5);
    }
}
