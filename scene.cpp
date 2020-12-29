#include "scene.h"

Scene::~Scene() {
	for (auto ptr : _spheres) {
		delete ptr;
	}
	for (auto ptr : _lights) {
		delete ptr;
	}
}

void Scene::addSphere(Sphere* sphere) {
	_spheres.push_back(sphere);
}

void Scene::addLight(Light* light) {
	_lights.push_back(light);
}

std::pair<const glm::vec3&, const Sphere*> Scene::getIntersection(const Ray& ray) {
	float minT = FLOAT_INF;
	const Sphere* collidedSphere = nullptr;
	for (auto pSphere : _spheres) {
		float t = pSphere->rayCollision(ray);
		if (t > FLOAT_EPS && t < minT) {
			minT = t;
			collidedSphere = pSphere;
		}
	}
	return std::make_pair(ray.at(minT), collidedSphere);
}

glm::vec3 Scene::shade(const Sphere& sphere, glm::vec3 fragPos, const Ray& ray) {
	glm::vec3 result(0.0f);
	for (auto pLight : _lights) {
		result += pLight->calLight(sphere.material(), fragPos, sphere.calNormal(fragPos), ray.direction());
	}
	return result;
}

glm::vec3 Scene::traceRay(const Ray& ray, int recursionTime) {
	glm::vec3 lightIntensity(0.0f);

	// 递归次数达到最大
	if (recursionTime >= MAX_RECURSION_TIME) {
		return lightIntensity;
	}

	// 计算光线与最近物体的交点
	const auto& pointAndSphere = getIntersection(ray);
	const Sphere* collidedSpherePtr = pointAndSphere.second;

	// 若光线没有照射到物体上直接返回
	if (collidedSpherePtr == nullptr) {
		return lightIntensity;
	}

	// 获得照射点及其法向量
	glm::vec3 collidedPoint = pointAndSphere.first;
	glm::vec3 normal = glm::normalize(collidedSpherePtr->calNormal(collidedPoint));

	// 若光线从物体内部射出，法向量取反
	bool enterSphere = collidedSpherePtr->rayInSphere(ray);
	if (enterSphere) {
		normal = -normal;
	}

	// 光追：局部光照强度
	if (!enterSphere) {
		lightIntensity = collidedSpherePtr->material().kShade() * shade(*collidedSpherePtr, collidedPoint, ray);
	}

	// 光追：反射光照强度
	glm::vec3 reflectDirection = glm::reflect(ray.direction(), normal);
	lightIntensity += collidedSpherePtr->material().kReflect() * 
		traceRay(Ray(collidedPoint, collidedPoint + reflectDirection), recursionTime + 1);

	// 光追：折射光照强度
	float currentIndex = 1.0f;
	float nextIndex = collidedSpherePtr->material().refractiveIndex();
	if (enterSphere) {
		// 若光线从物体内部射出，折射率交换
		std::swap(currentIndex, nextIndex);
	}
	glm::vec3 refractDirection = glm::refract(ray.direction(), normal, currentIndex / nextIndex);
	lightIntensity += collidedSpherePtr->material().kRefract() * 
		traceRay(Ray(collidedPoint, collidedPoint + refractDirection), recursionTime + 1);
	
	return lightIntensity;
}