#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>
#include <cmath>

#include "ray.h"
#include "sphere.h"

class Sphere;

class Material {
public:
	virtual std::pair<std::vector<Ray>, std::vector<glm::vec3>> scatter(
		const Ray& r_in, Sphere* hitSphere, float t) = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const glm::vec3 color) {
		_color.push_back(color);
	}

	virtual std::pair<std::vector<Ray>, std::vector<glm::vec3>> scatter(
		const Ray& r_in, Sphere* hitSphere, float t) {
		std::vector<Ray> result;
		std::vector<glm::vec3> color = _color;
		glm::vec3 hitPoint(r_in.origin() + t * r_in.direction());
		glm::vec3 normal = glm::normalize(hitPoint - hitSphere->center());
		glm::vec3 target = hitPoint + normal + randomInUnitSphere();
		result.push_back(Ray(hitPoint, target));
		return std::make_pair(result, color);
	}

private:
	glm::vec3 randomInUnitSphere() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> distribution(0.0, 1.0);

		glm::vec3 p;
		do {
			p = 2.0f * glm::vec3(distribution(gen), distribution(gen), distribution(gen)) \
				- glm::vec3(1.0f, 1.0f, 1.0f);
		} while (std::sqrt(glm::dot(p, p)) >= 1.0);
		return p;
	}

	std::vector<glm::vec3> _color;
};

class Metal : public Material {
public:
	Metal(const glm::vec3 color) {
		_color.push_back(color);
	}

	virtual std::pair<std::vector<Ray>, std::vector<glm::vec3>> scatter(
		const Ray& r_in, Sphere* hitSphere, float t) {
		std::vector<Ray> result;
		std::vector<glm::vec3> color = _color;
		glm::vec3 hitPoint(r_in.origin() + t * r_in.direction());
		glm::vec3 normal = glm::normalize(hitPoint - hitSphere->center());
		glm::vec3 reflected = glm::reflect(r_in.direction(), normal);
		result.push_back(Ray(hitPoint, hitPoint + reflected));
		return std::make_pair(result, color);
	}

private:
	std::vector<glm::vec3> _color;
};

class Dielectric : public Material {
public:
	Dielectric(float reflectIdx) : _reflectIdx{ reflectIdx } {}
	float reflectIdx() { return _reflectIdx; }

	virtual std::pair<std::vector<Ray>, std::vector<glm::vec3>> scatter(
		const Ray& r_in, Sphere* hitSphere, float t) {
		std::vector<Ray> result;
		std::vector<glm::vec3> color;
		glm::vec3 hitPoint(r_in.origin() + t * r_in.direction());
		glm::vec3 normal = glm::normalize(hitPoint - hitSphere->center());
		float eta, cosine;

		if (glm::dot(r_in.direction(), normal) > 0.0f) {
			normal = -normal;
			eta = _reflectIdx;
			cosine = glm::dot(r_in.direction(), -normal) / std::sqrt(glm::dot(
				r_in.direction(), r_in.direction()));
			cosine = std::sqrt(1 - _reflectIdx * _reflectIdx * (1 - cosine * cosine));
		}
		else {
			normal = normal;
			eta = 1.0f / _reflectIdx;
			cosine = -glm::dot(r_in.direction(), normal) / std::sqrt(glm::dot(
				r_in.direction(), r_in.direction()));
		}

		glm::vec3 reflected = glm::reflect(r_in.direction(), normal);
		glm::vec3 refracted = glm::refract(r_in.direction(), normal, eta);
		if (refracted.x != refracted.x) {
			// NaN
			result.push_back(Ray(hitPoint, hitPoint + reflected));
			color.push_back(glm::vec3(1.0f));
		}
		else {
			result.push_back(Ray(hitPoint, hitPoint + reflected));
			color.push_back(glm::vec3(schlick(cosine)));
			result.push_back(Ray(hitPoint, hitPoint + refracted));
			color.push_back(glm::vec3(1 - schlick(cosine)));
		}
		return std::make_pair(result, color);
	}

private:
	float schlick(float cosine) {
		float r0 = (1.0f - _reflectIdx) / (1.0f + _reflectIdx);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * std::pow((1 - cosine), 5.0f);
	}

	float _reflectIdx;
};

#endif // !MATERIAL_H