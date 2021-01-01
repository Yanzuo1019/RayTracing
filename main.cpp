#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <vector>
#include <cmath>

#include "shader.h"
#include "camera.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
constexpr int MAX_RECURSION_TIME = 10;
constexpr int SAMPLE_PER_PIXEL = 1000;
glm::vec3 col[SCR_WIDTH][SCR_HEIGHT];

void vecPrint(glm::vec3 v) {
    std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}

glm::vec3 color(const Ray& ray, Sphere** world, int length, int depth) {
    float minT = FLOAT_INF;
    Sphere* collidedSphere = nullptr;
    for (int i = 0; i < length; i++) {
        float t = world[i]->rayCollision(ray);
        if (t > FLOAT_EPS && t < minT) {
            minT = t;
            collidedSphere = world[i];
        }
    }
   
    if (collidedSphere != nullptr) {
        if (depth < MAX_RECURSION_TIME) {
            auto ret = collidedSphere->material()->scatter(ray, collidedSphere, minT);
            auto r = ret.first;
            auto c = ret.second;

            glm::vec3 result(0.0f);
            for (int i = 0; i < r.size(); i++) {
                result += c[i] * color(r[i], world, length, depth + 1);
            }
            //vecPrint(result);
            return result;
        }
        else {
            return glm::vec3(0.0f);
        }
    }
    else {
        if (ray.direction().y != ray.direction().y) return glm::vec3(0.0f);
        float t = 0.5f * (ray.direction().y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
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
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

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

    // 随机场景
    int n = 500;
    Sphere** world = new Sphere * [n];
    world[0] = new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f,
        new Lambertian(glm::vec3(0.5f, 0.5f, 0.5f)));
    int cnt = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = distribution(gen);
            glm::vec3 center(a + 0.9f * distribution(gen), 0.2f, b + 0.9f * distribution(gen));
            auto tmp = center - glm::vec3(4.0f, 0.2f, 0.0f);
            if (std::sqrt(glm::dot(tmp, tmp)) > 0.9f) {
                if (choose_mat < 0.8f) { // diffuse
                    world[cnt++] = new Sphere(center, 0.2, new Lambertian(glm::vec3(distribution(gen) * distribution(gen), 
                        distribution(gen) * distribution(gen), distribution(gen) * distribution(gen))));
                }
                else if (choose_mat < 0.95f) { // metal
                    world[cnt++] = new Sphere(center, 0.2, new Metal(glm::vec3(0.5f * (1.0f + distribution(gen)),
                        0.5f * (1.0f + distribution(gen)), 0.5f * (1.0f + distribution(gen)))));
                }
                else // glass
                {
                    world[cnt++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
                }
            }
        }
    }

    world[cnt++] = new Sphere(glm::vec3(6.0f, 1.0f, 0.0f), 1.0f, new Metal(glm::vec3(0.7f, 0.6f, 0.5f)));
    world[cnt++] = new Sphere(glm::vec3(2.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f));
    //world[cnt++] = new Sphere(glm::vec3(2.0f, 1.0f, 0.0f), 0.8f, new Dielectric(1.5f));
    world[cnt++] = new Sphere(glm::vec3(-2.0f, 1.0f, 0.0f), 1.0f, new Lambertian(glm::vec3(0.4f, 0.2f, 0.1f)));

    // 观察点
    glm::vec3 lookfrom(13.0f, 2.0f, 3.0f);
    glm::vec3 lookat(0.0f, 0.0f, 0.0f);

    Camera cam(lookfrom, lookat, glm::vec3(0.0f, 1.0f, 0.0f), 20.0f, float(SCR_WIDTH) / float(SCR_HEIGHT));

    // 主循环渲染画面
    for (int sample = 1; sample <= SAMPLE_PER_PIXEL; sample++) {
        std::cout << "SAMPLE TIMES: " << sample << std::endl;
        if (!glfwWindowShouldClose(window)) {
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
                    glm::vec2 pos(float(i) * 2.0f / SCR_WIDTH - 1.0f, float(j) * 2.0f / SCR_HEIGHT - 1.0f);
                    shader.setVec2("screenPos", pos.x, pos.y);

                    float u = float(i + distribution(gen)) / float(SCR_WIDTH);
                    float v = float(j + distribution(gen)) / float(SCR_HEIGHT);
                    Ray r = cam.get_ray(u, v);

                    glm::vec3 c = color(r, world, cnt, 0);
                    c = glm::vec3(std::sqrt(c[0]), std::sqrt(c[1]), std::sqrt(c[2]));
                    col[i][j] = col[i][j] * float(sample - 1) / float(sample) + c / float(sample);
                    
                    shader.setVec3("vertexColor", col[i][j]);
                    glDrawArrays(GL_POINTS, 0, 1);
                }
            }

            // 双缓冲
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        else {
            break;
        }
    } 

    delete[] world;
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