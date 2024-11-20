#include "Tracer.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Color.h"

color3_t Tracer::Trace(Scene& scene, const ray_t& ray, float minDistance, float maxDistance, int depth)
{
	raycastHit_t raycastHit;
	float closestDistance = maxDistance;
	bool isHit = false;
	color3_t color{ 0 };

	if (depth == 0) return color3_t{ 0 };

	//check for object hit in scene	
	for (auto& object : scene.m_objects)
	{
		if (object->Hit(ray,raycastHit,minDistance,closestDistance))
		{
			isHit = true;
			closestDistance = raycastHit.distance;
			
		}
	}

	if (isHit)
	{
		color3_t attenuation;
		ray_t scatter;
		if (raycastHit.material.lock()->Scatter(ray, raycastHit, attenuation, scatter))
		{
			return attenuation * Trace(scene, scatter, minDistance, maxDistance, depth - 1);
		}
		else
		{
			return raycastHit.material.lock()->GetEmissive();
		}
	}

	//sky
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f;
	 color = Lerp(scene.m_skyBottom, scene.m_skyTop, t);

	return color;
}
