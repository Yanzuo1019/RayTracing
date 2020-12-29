#include <algorithm>

#include "light.h"

glm::vec3 Light::calLight(const Material& material, const glm::vec3& fragPos,
	const glm::vec3& norm, const glm::vec3& viewDir) {
	// 环境光
	glm::vec3 ambient = _color * material.ambient();

	// 漫反射
	glm::vec3 lightDir = glm::normalize(_pos - fragPos);
	float diff = std::max(glm::dot(norm, lightDir), 0.0f);
	glm::vec3 diffuse = _color * (diff * material.diffuse());

	// 镜面光
	glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
	float spec = glm::pow(std::max(glm::dot(-viewDir, reflectDir), 0.0f), material.shininess());
	glm::vec3 specular = _color * (spec * material.speclar());

	return ambient + diffuse + specular;
}