#pragma once

struct ray_t {
	ray_t() = default;
	ray_t(const glm::vec3 orgin, const glm::vec3 direction) : orgin { orgin } ,direction{ direction }{}

	glm::vec3 at(float time) {
		glm::vec3 result = (orgin * direction * time);

		return result;
	}

	glm:: vec3 operator* (float time) {
		return (orgin * direction * time);
	}

	glm::vec3 orgin;
	glm::vec3 direction;
};