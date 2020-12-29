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

	float kShade() const { return _kShade; }
	float kReflect() const { return _kReflect; }
	float kRefract() const { return _kRefract; }
	float refractiveIndex() const { return _refractiveIndex; }
private:
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	float _shininess;

	float _kShade = 0.6f;
	float _kReflect = 0.2f;
	float _kRefract = 0.2f;
	float _refractiveIndex;
};

// 定义一些在绘制场景时会使用到的材质(数据来自http://devernay.free.fr/cours/opengl/materials.html)
Material emerald(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f),
	glm::vec3(0.633f, 0.727811f, 0.633f), 0.6f, 1.58f);
Material pearl(glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f),
	glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f, 1.61f);
Material bronze(glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f),
	glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2f, 1.180f);
Material gold(glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f),
	glm::vec3(0.628281f, 0.555802f, 0.366065f), 0.4f, 0.470f);

Material cyan_plastic(glm::vec3(0.0f, 0.1f, 0.06f), glm::vec3(0.0f, 0.50980392f, 0.50980392f),
	glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 0.25f, 1.460f);
Material red_plastic(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f),
	glm::vec3(0.7f, 0.6f, 0.6f), 0.25f, 1.460f);
Material green_rubber(glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.4f, 0.5f, 0.4f),
	glm::vec3(0.04f, 0.7f, 0.04f), 0.078125f, 1.519f);
Material yellow_rubber(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(0.5f, 0.5f, 0.4f),
	glm::vec3(0.7f, 0.7f, 0.04f), 0.078125f, 1.519f);

Material glass(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.588235f, 0.670588f, 0.729412f), 
	glm::vec3(0.9f, 0.9f, 0.9f), 0.75f, 1.5f);

// 每种材质都安排一个下标，方便随机选取
Material materialSet[] = { emerald, pearl, bronze, gold, cyan_plastic, red_plastic, green_rubber, yellow_rubber, glass };

#endif // !MATERIAL_H