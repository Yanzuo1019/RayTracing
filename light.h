#ifndef LIGHT_H
#define LIGHT_H

#include "material.h"

class Light {
public:
	Light(glm::vec3 color, glm::vec3 pos) :
		_ambient(color * 1.0f), _diffuse(color * 1.0f), _specular(color * 1.0f), _pos(pos) {}

	glm::vec3 calLight(const Material& material, const glm::vec3& fragPos,
		const glm::vec3& norm, const glm::vec3& viewDir);
private:
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	glm::vec3 _pos;
};

#endif // !LIGHT_H
