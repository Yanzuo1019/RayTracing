#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "material.h"

class Sphere {
public:
	Sphere(const glm::vec3& center, float radius, Material material):
		_center(center), _radius(radius), _material(material) {}
	
	glm::vec3 center() const { return _center; }
	float radius() const { return _radius; }
	Material material() const { return _material; }

	glm::vec3 calNormal(const glm::vec3& p) const;
	bool inSphere(const glm::vec3& p) const;
	bool rayInSphere(const Ray& ray) const;
	float rayCollision(const Ray& ray) const;
private:
	glm::vec3 _center;
	float _radius;
	Material _material;
};

#endif // !SPHERE_H
