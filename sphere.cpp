#include <cstdlib>
#include <cmath>

#include "sphere.h"

glm::vec3 Sphere::calNormal(const glm::vec3& p) const {
	return glm::normalize(p - _center);
}

bool Sphere::inSphere(const glm::vec3& p) const {
	return glm::distance(p, _center) < _radius + FLOAT_EPS;
}

bool Sphere::rayInSphere(const Ray& ray) const {
	return inSphere(ray.origin()) && rayCollision(ray) > FLOAT_EPS;
}

float Sphere::rayCollision(const Ray& ray) const {
	glm::vec3 vc = ray.origin() - _center;

	float A = glm::dot(ray.direction(), ray.direction());
	float B = 2 * glm::dot(vc, ray.direction());
	float C = glm::dot(vc, vc) - _radius * _radius;
	if (abs(C) < FLOAT_EPS) {
		C = 0;
	}

	float delta = B * B - 4 * A * C;
	if (delta < FLOAT_EPS) {
		return -1;
	}
	delta = sqrt(delta);

	float t1 = (-B + delta) / 2 / A;
	float t2 = (-B - delta) / 2 / A;
	if (t1 < FLOAT_EPS && t2 < FLOAT_EPS) {
		return -1;
	}
	if (t2 > FLOAT_EPS) {
		return t2;
	}
	else {
		return t1;
	}
}