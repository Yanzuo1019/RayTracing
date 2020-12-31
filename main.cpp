#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

#include "shader.h"
#include "scene.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 675;

glm::vec3 write_color(glm::vec3 pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    float scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    return glm::vec3(glm::clamp(r, 0.0f, 0.999f), glm::clamp(g, 0.0f, 0.999f), glm::clamp(b, 0.0f, 0.999f));
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RayTracing", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 初始化随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(-1.0, 1.0);

    // 初始化顶点和片元着色器
    Shader shader("shader.vs", "shader.fs");

    // 这里只是一个象征性的VAO和VBO初始化，实际上没有用到渲染管线的特点
    float point[] = { 0, 0, 0 };
    GLuint VAO, VBO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 初始化场景并设置一个光源
    Scene scene;
    scene.addLight(new Light(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(100.0f, 100.0f, 100.0f)
    ));

    // 初始化观察点照相机
    glm::vec3 viewPos = glm::vec3(0.0f, 2.0f, 3.0f);
    glm::vec3 viewFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 viewRight = glm::normalize(glm::cross(viewFront, viewUp));

    // 创建背景
    scene.addSphere(new Sphere(
        glm::vec3(0.0f, -100.0f, -5.0f),
        100.0f,
        DefaultMaterial::glass
    ));

    // 中间再放个红色塑料球
    scene.addSphere(new Sphere(
        glm::vec3(0.0f, 1.0f, 0.0f),
        1.0f,
        DefaultMaterial::bronze
    ));

    scene.addSphere(new Sphere(
        glm::vec3(2.0f, 1.0f, 0.0f),
        1.0f,
        DefaultMaterial::cyan_plastic
    ));

    scene.addSphere(new Sphere(
        glm::vec3(-2.0f, 1.0f, 0.0f),
        1.0f,
        DefaultMaterial::emerald
    ));

    // 主循环渲染画面
    while (!glfwWindowShouldClose(window)) {
        // 处理输入信息
        processInput(window);

        // 初始化颜色缓冲（随便找个颜色，其实只有刷新的作用）
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 将点设置为绘制对象
        shader.use();
        glBindVertexArray(VAO);

        // 枚举屏幕上每一个像素
        for (int j = SCR_HEIGHT - 1; j >= 0; j--) {
            for (int i = 0; i < SCR_WIDTH; i++) {
                // 将坐标映射到NDC[-1,1]
                glm::vec3 pos(float(i) * 2 / SCR_WIDTH - 1.0f, float(j) * 2 / SCR_HEIGHT - 1.0f, 0.0f);
                shader.setVec2("screenPos", pos.x, pos.y);

                glm::vec3 pixel_color(0.0f);
                for (int s = 0; s < SAMPLE_PER_PIXEL; s++) {
                    auto u = pos.x + distribution(gen) / SCR_WIDTH;
                    auto v = pos.y + distribution(gen) / SCR_HEIGHT;
                    glm::vec3 globalPos = viewPos + viewFront + u * viewRight * (float(SCR_WIDTH) / SCR_HEIGHT) + v * viewUp;
                    Ray r(viewPos, globalPos);
                    pixel_color = pixel_color + scene.traceRay(r, 0);
                }
                
                // 绘制该点的像素
                glm::vec3 color = write_color(pixel_color, SAMPLE_PER_PIXEL);
                shader.setVec3("vertexColor", color);
                glDrawArrays(GL_POINTS, 0, 1);
            }
        }

        // 双缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}