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

	// �ݹ�����ﵽ���
	if (recursionTime >= MAX_RECURSION_TIME) {
		return lightIntensity;
	}

	// ����������������Ľ���
	const auto& pointAndSphere = getIntersection(ray);
	const Sphere* collidedSpherePtr = pointAndSphere.second;

	// ������û�����䵽������ֱ�ӷ���
	if (collidedSpherePtr == nullptr) {
		return lightIntensity;
	}

	// �������㼰�䷨����
	glm::vec3 collidedPoint = pointAndSphere.first;
	glm::vec3 normal = glm::normalize(collidedSpherePtr->calNormal(collidedPoint));

	// �����ߴ������ڲ������������ȡ��
	bool enterSphere = collidedSpherePtr->rayInSphere(ray);
	if (enterSphere) {
		normal = -normal;
	}

	// ��׷���ֲ�����ǿ��
	if (!enterSphere) {
		lightIntensity = collidedSpherePtr->material().kShade() * shade(*collidedSpherePtr, collidedPoint, ray);
	}

	// ��׷���������ǿ��
	glm::vec3 reflectDirection = glm::reflect(ray.direction(), normal);
	lightIntensity += collidedSpherePtr->material().kReflect() * 
		traceRay(Ray(collidedPoint, collidedPoint + reflectDirection), recursionTime + 1);

	// ��׷���������ǿ��
	float currentIndex = 1.0f;
	float nextIndex = collidedSpherePtr->material().refractiveIndex();
	if (enterSphere) {
		// �����ߴ������ڲ�����������ʽ���
		std::swap(currentIndex, nextIndex);
	}
	glm::vec3 refractDirection = glm::refract(ray.direction(), normal, currentIndex / nextIndex);
	lightIntensity += collidedSpherePtr->material().kRefract() * 
		traceRay(Ray(collidedPoint, collidedPoint + refractDirection), recursionTime + 1);
	
	return lightIntensity;
}