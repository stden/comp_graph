package fractals;

import fractals.core.Fractal;
import fractals.renderers.*;
import com.jogamp.opengl.*;
import com.jogamp.opengl.awt.GLCanvas;
import com.jogamp.opengl.util.FPSAnimator;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.HashMap;
import java.util.Map;

/**
 * Fractal Explorer - главное приложение для визуализации фракталов
 *
 * Возможности:
 * - Интерактивная навигация (зум/пан мышью)
 * - Переключение между фракталами
 * - Настройка параметров
 * - Отображение FPS и информации
 */
public class FractalExplorer extends JFrame implements GLEventListener {
    private static final int WIDTH = 1920;
    private static final int HEIGHT = 1080;

    private GLCanvas canvas;
    private FPSAnimator animator;
    private Map<String, Fractal> fractals;
    private Fractal currentFractal;

    // UI элементы
    private JLabel fpsLabel;
    private JLabel infoLabel;
    private JComboBox<String> fractalSelector;

    // Управление мышью
    private int mouseX, mouseY;
    private boolean isDragging;

    // FPS счетчик
    private long lastFpsTime;
    private int fpsCount;
    private int currentFps;

    public FractalExplorer() {
        super("Fractal Explorer - OpenGL Visualization");

        // Инициализация фракталов
        initFractals();

        // Создание OpenGL canvas
        GLProfile profile = GLProfile.get(GLProfile.GL2);
        GLCapabilities capabilities = new GLCapabilities(profile);
        capabilities.setSampleBuffers(true);
        capabilities.setNumSamples(4); // MSAA 4x

        canvas = new GLCanvas(capabilities);
        canvas.addGLEventListener(this);
        canvas.setPreferredSize(new Dimension(WIDTH, HEIGHT));

        // Управление мышью
        setupMouseControls();

        // Управление клавиатурой
        setupKeyboardControls();

        // UI панель
        JPanel controlPanel = createControlPanel();

        // Компоновка
        setLayout(new BorderLayout());
        add(canvas, BorderLayout.CENTER);
        add(controlPanel, BorderLayout.SOUTH);

        // Animator для плавной отрисовки
        animator = new FPSAnimator(canvas, 60, true);

        // Настройка окна
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        pack();
        setLocationRelativeTo(null);
        setResizable(true);

        // FPS tracking
        lastFpsTime = System.currentTimeMillis();
        fpsCount = 0;
        currentFps = 0;
    }

    private void initFractals() {
        fractals = new HashMap<>();
        fractals.put("Mandelbrot Set", new MandelbrotSet());
        fractals.put("Julia Set", new JuliaSet());
        fractals.put("Pythagoras Tree", new PythagorasTree());
        fractals.put("Koch Snowflake", new KochSnowflake());
        fractals.put("Sierpinski Triangle", new SierpinskiTriangle());
        fractals.put("Fractal Landscape", new FractalLandscape());
        fractals.put("Lyapunov Fractal", new LyapunovFractal());

        currentFractal = fractals.get("Mandelbrot Set");
    }

    private JPanel createControlPanel() {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        panel.setBackground(Color.DARK_GRAY);

        // Селектор фракталов
        fractalSelector = new JComboBox<>(fractals.keySet().toArray(new String[0]));
        fractalSelector.addActionListener(e -> {
            String selected = (String) fractalSelector.getSelectedItem();
            currentFractal = fractals.get(selected);
            updateInfo();
        });

        // FPS label
        fpsLabel = new JLabel("FPS: 0");
        fpsLabel.setForeground(Color.GREEN);
        fpsLabel.setFont(new Font("Monospaced", Font.BOLD, 14));

        // Info label
        infoLabel = new JLabel();
        infoLabel.setForeground(Color.WHITE);
        infoLabel.setFont(new Font("Monospaced", Font.PLAIN, 12));

        // Кнопки управления
        JButton resetBtn = new JButton("Reset");
        resetBtn.addActionListener(e -> currentFractal.reset());

        JButton iterPlusBtn = new JButton("Iter+");
        iterPlusBtn.addActionListener(e -> {
            currentFractal.setIterations(currentFractal.getIterations() + 32);
            updateInfo();
        });

        JButton iterMinusBtn = new JButton("Iter-");
        iterMinusBtn.addActionListener(e -> {
            currentFractal.setIterations(currentFractal.getIterations() - 32);
            updateInfo();
        });

        panel.add(new JLabel("Fractal:"));
        panel.add(fractalSelector);
        panel.add(resetBtn);
        panel.add(iterPlusBtn);
        panel.add(iterMinusBtn);
        panel.add(Box.createHorizontalStrut(20));
        panel.add(fpsLabel);
        panel.add(Box.createHorizontalStrut(20));
        panel.add(infoLabel);

        return panel;
    }

    private void setupMouseControls() {
        canvas.addMouseListener(new MouseAdapter() {
            @Override
            public void mousePressed(MouseEvent e) {
                mouseX = e.getX();
                mouseY = e.getY();
                isDragging = true;
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                isDragging = false;
            }
        });

        canvas.addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                if (isDragging) {
                    int dx = e.getX() - mouseX;
                    int dy = e.getY() - mouseY;
                    currentFractal.pan(dx, dy);
                    mouseX = e.getX();
                    mouseY = e.getY();
                }
            }
        });

        canvas.addMouseWheelListener(e -> {
            double zoomFactor = e.getWheelRotation() < 0 ? 1.2 : 0.8;
            currentFractal.zoom(zoomFactor);
        });
    }

    private void setupKeyboardControls() {
        canvas.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                switch (e.getKeyCode()) {
                    case KeyEvent.VK_ESCAPE:
                        System.exit(0);
                        break;
                    case KeyEvent.VK_R:
                        currentFractal.reset();
                        break;
                    case KeyEvent.VK_PLUS:
                    case KeyEvent.VK_EQUALS:
                        currentFractal.setIterations(currentFractal.getIterations() + 32);
                        break;
                    case KeyEvent.VK_MINUS:
                        currentFractal.setIterations(currentFractal.getIterations() - 32);
                        break;
                    case KeyEvent.VK_SPACE:
                        if (currentFractal instanceof PythagorasTree) {
                            ((PythagorasTree) currentFractal).toggleAnimation();
                        }
                        break;
                }
                updateInfo();
            }
        });
        canvas.setFocusable(true);
    }

    @Override
    public void init(GLAutoDrawable drawable) {
        GL2 gl = drawable.getGL().getGL2();
        gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl.glEnable(GL2.GL_BLEND);
        gl.glBlendFunc(GL2.GL_SRC_ALPHA, GL2.GL_ONE_MINUS_SRC_ALPHA);
        gl.glEnable(GL2.GL_LINE_SMOOTH);
        gl.glHint(GL2.GL_LINE_SMOOTH_HINT, GL2.GL_NICEST);

        updateInfo();
    }

    @Override
    public void display(GLAutoDrawable drawable) {
        GL2 gl = drawable.getGL().getGL2();
        gl.glClear(GL2.GL_COLOR_BUFFER_BIT);

        // Отрисовка текущего фрактала
        if (currentFractal != null) {
            currentFractal.render(gl, canvas.getWidth(), canvas.getHeight());
        }

        // FPS подсчет
        updateFPS();

        gl.glFlush();
    }

    @Override
    public void reshape(GLAutoDrawable drawable, int x, int y, int width, int height) {
        GL2 gl = drawable.getGL().getGL2();
        gl.glViewport(0, 0, width, height);
    }

    @Override
    public void dispose(GLAutoDrawable drawable) {
        if (animator != null) {
            animator.stop();
        }
    }

    private void updateFPS() {
        fpsCount++;
        long currentTime = System.currentTimeMillis();
        if (currentTime - lastFpsTime >= 1000) {
            currentFps = fpsCount;
            fpsCount = 0;
            lastFpsTime = currentTime;

            SwingUtilities.invokeLater(() -> fpsLabel.setText("FPS: " + currentFps));
        }
    }

    private void updateInfo() {
        if (currentFractal != null) {
            SwingUtilities.invokeLater(() ->
                infoLabel.setText(currentFractal.getInfo().replace("\n", " | "))
            );
        }
    }

    public void start() {
        animator.start();
        setVisible(true);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            FractalExplorer explorer = new FractalExplorer();
            explorer.start();
        });
    }
}
