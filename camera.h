#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

#include "ray.h"

constexpr float PI = 3.14159265358979323846;

class Camera {
public:
	Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect) {
		glm::vec3 u, v, w;
		float theta = vfov * PI / 180.0f;
		float half_height = std::tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookfrom;
		w = glm::normalize(lookfrom - lookat);
		u = glm::normalize(glm::cross(vup, w));
		v = glm::cross(w, u);

		lower_left_corner = glm::vec3(-half_width, -half_height, -1.0f);
		lower_left_corner = origin - half_width * u - half_height * v - w;
		horizontal = 2.0f * half_width * u;
		vertical = 2.0f * half_height * v;
	}

	Ray get_ray(float s, float t) {
		return Ray(origin, lower_left_corner + s * horizontal + t * vertical);
	}

	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
};

#endif // !CAMERA_H
