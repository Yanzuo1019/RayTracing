#ifndef LIGHT_H
#define LIGHT_H

#include "material.h"

class Light {
public:
	Light(glm::vec3 color, glm::vec3 pos) :
		_color(color), _pos(pos) {}

	glm::vec3 calLight(const Material& material, const glm::vec3& fragPos,
		const glm::vec3& norm, const glm::vec3& viewDir);
private:
	glm::vec3 _color;
	glm::vec3 _pos;
};

#endif // !LIGHT_H
