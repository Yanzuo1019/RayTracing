#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material {
public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 speclar, float shininess, float refractiveIndex):
		_ambient(ambient), _diffuse(diffuse), _specular(speclar), _shininess(shininess), _refractiveIndex(refractiveIndex) {}

	glm::vec3 ambient() const { return _ambient; }
	glm::vec3 diffuse() const { return _diffuse; }
	glm::vec3 speclar() const { return _specular; }
	float shininess() const { return _shininess * 128.0; }

	//float kShade() const { return _kShade; }
	//float kReflect() const { return _kReflect; }
	//float kRefract() const { return _kRefract; }
	float refractiveIndex() const { return _refractiveIndex; }

	//void setk(float kShade, float kReflect, float kRefract);
private:
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _shininess;

	//float _kShade = 0.6f;
	//float _kReflect = 0.2f;
	//float _kRefract = 0.2f;
	float _refractiveIndex;
};

namespace DefaultMaterial {
	// 定义一些在绘制场景时会使用到的材质(数据来自http://devernay.free.fr/cours/opengl/materials.html)
	extern Material emerald, pearl, bronze, gold, cyan_plastic, red_plastic, green_rubber, yellow_rubber, glass;

	// 每种材质都安排一个下标，方便随机选取
	extern Material materialSet[9];
};

#endif // !MATERIAL_H