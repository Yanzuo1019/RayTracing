#ifndef SPHERE_H
#define SPHERE_H

#include <cstdlib>
#include <cmath>

#include "ray.h"

class Material;

class Sphere {
public:
	Sphere(const glm::vec3& center, float radius, Material* material):
		_center(center), _radius(radius), _material(material) {}
	~Sphere() { delete _material; }
	
	glm::vec3 center() const { return _center; }
	float radius() const { return _radius; }
	Material* material() const { return _material; }

	float rayCollision(const Ray& ray) const {
		glm::vec3 vc = ray.origin() - _center;

		float A = glm::dot(ray.direction(), ray.direction());
		float B = glm::dot(vc, ray.direction());
		float C = glm::dot(vc, vc) - _radius * _radius;
		float discriminant = B * B - A * C;

		if (discriminant > 0.0f) {
			float t = (-B - sqrt(discriminant)) / A;
			if (t < FLOAT_INF && t > FLOAT_EPS) {
				return t;
			}
			t = (-B + sqrt(discriminant)) / A;
			if (t < FLOAT_INF && t > FLOAT_EPS) {
				return t;
			}
		}

		return -1;
	}
private:
	glm::vec3 _center;
	float _radius;
	Material* _material;
};

#endif // !SPHERE_H
