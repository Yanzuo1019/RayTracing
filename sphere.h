#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ray.h"
#include "material.h"

class Sphere {
public:
	Sphere(const glm::vec3& center, float radius, Material material):
		_center(center), _radius(radius), _material(material) {}
	
	glm::vec3 center() const { return _center; }
	float radius() const { return _radius; }
	Material material() const { return _material; }

	bool inSphere(const glm::vec3& p) const;
	float rayCollision(const Ray& ray) const;
	glm::vec3 calNormal(const glm::vec3& p) const;
	bool rayInEntity(const Ray& ray) const;
private:
	glm::vec3 _center;
	float _radius;
	Material _material;
};

#endif // !SPHERE_H
