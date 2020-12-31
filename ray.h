#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const float FLOAT_INF = 100000000.0f;
static const float FLOAT_EPS = 1e-4;

class Ray {
public:
	Ray(glm::vec3 src, glm::vec3 dest):
		_origin(src), _direction(glm::normalize(dest - src)) {}

	glm::vec3 at(float t) const { return _origin + t * _direction; }
	glm::vec3 origin() const { return _origin; }
	glm::vec3 direction() const { return _direction; }
private:
	glm::vec3 _origin;
	glm::vec3 _direction;
};

#endif // !RAY_H