#include "Camera.h"
#include "Color.h"
#include "ETime.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Input.h"
#include "Model.h"
#include "Plane.h"
#include "PostProcess.h"
#include "Random.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Transform.h"
#include "Tracer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

//void InitScene(Scene& scene);
void InitScene01(Scene& scene, Camera& camera);

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	Time time;
	Input input;

	Renderer* renderer = new Renderer;
	renderer->Initialize();
	renderer->CreateWindow("Ray Tracer", 800, 600);
	Framebuffer framebuffer(*renderer, renderer->GetWidth(), renderer->GetHeight());
	
	Camera camera{70.0f,framebuffer.m_width / (float)framebuffer.m_height};
	camera.SetView({0,10,-20},{0,0,0});

	SetBlendMode(BlendMode::Normal);
	
	Scene scene;
	InitScene01(scene, camera);
	
	scene.Update();
	scene.Render(framebuffer, camera, 100, 3);
	framebuffer.Update();

	

	bool quit = false;


	while (!quit)
	{
	
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}
		
		//framebuffer.Clear(ColorConvert(color4_t{ 128,128,128,255 }));
		//scene.Render(framebuffer, camera);
		//tracer.Render(framebuffer, camera);
		
		renderer->CopyFramebuffer(framebuffer);
		//framebuffer.Update();

		SDL_RenderPresent(renderer->m_renderer);

	}




	return 0;
}

void InitScene01(Scene& scene, Camera& camera)
{
	camera.SetFOV(20.0f);
	camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

	auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = randomf();
			glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					sphere_material = std::make_shared<Lambertian>(albedo);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					auto fuzz = randomf(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));

	auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

	auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
}