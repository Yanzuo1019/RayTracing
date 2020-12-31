#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "sphere.h"
#include "light.h"

static const int MAX_RECURSION_TIME = 20;
static const int SAMPLE_PER_PIXEL = 5;

class Scene {
public:
	Scene() {}
	~Scene();

	void addSphere(Sphere* sphere);
	void addLight(Light* light);

	std::pair<const glm::vec3&, const Sphere*> getIntersection(const Ray& ray);
	glm::vec3 shade(const Sphere& sphere, glm::vec3 fragPos, const Ray& ray);
	glm::vec3 traceRay(const Ray& ray, int recursionTime);
private:
	std::vector<Sphere*> _spheres;
	std::vector<Light*> _lights;
};

#endif // !SCENE_H