#pragma once
#include "Color.h"
#include "SceneObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Transform.h"

using Vertex_t = glm::vec3;
using Verticies_t = std::vector<Vertex_t>;

class Model : public SceneObject
{
public:
	Model() = default;
	Model(const Verticies_t& verticies, const color_t& color) : m_verticies{ verticies }, m_color{ color } {}
	Model(const Transform& transform, std::shared_ptr<Material> material) : SceneObject{ transform,material } {}

	void Update();
	bool Load(const std::string& filename);
	void SetColor(color_t color) { m_color = color; }
	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance);

private:
	Verticies_t m_verticies;
	Verticies_t m_local_verticies;
	color_t m_color;

	glm::vec3 m_center{ 0 };
	float m_radius{ 0 };
};