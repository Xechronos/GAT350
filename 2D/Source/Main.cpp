#include "Renderer.h"
#include "FrameBuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include "Color.h"
#include "ETime.h"
#include "Transform.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Actor.h"
#include "Random.h"
#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[])
{
    Time time;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 800, 600);

    SetBlendMode(BlendMode::Normal);

    Input input;
    input.Initialize();
    input.Update();

    Camera camera(renderer.m_width, renderer.m_height);
    camera.SetView(glm::vec3{ 0,0,-20 }, glm::vec3{ 0 });
    camera.SetProjection(60.0f, 800.0f / 600, 0.1f, 200.0f);
    Transform cameraTransform{ {0,0,-20} };

    Framebuffer framebuffer(renderer, 800, 600);
    Image image;
    image.Load("hydra logo.png");

    Image imageAlpha;
    imageAlpha.Load("shield.jpg");
    PostProcess::Alpha(imageAlpha.m_buffer, 128); // int8_t = (-128 <-> 127), uint8_t = (0, 255)


    vertices_t vertices = { {-5,5,0},{5,5,0}, };
    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->Load("cube.obj");
    /*model->SetColor({ 0, 255, 0, 255 });*/
    Transform modelTransform{ {0,0,0}, glm::vec3{0,0,90}, glm::vec3{0} };
    Actor actor(modelTransform, model);

    std::vector<std::unique_ptr<Actor>> actors;
    for (int i = 0; i < 20; i++)
    {
        Transform transform{ { randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f) }, glm::vec3{0, 0, 0}, glm::vec3{ randomf(2, 20) } };
        std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
        actor->SetColor({ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256), 255 });
        actors.push_back(std::move(actor));
    }


    bool quit = false;

    while (!quit) {

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
        // clear screen
        //SDL_SetRenderDrawColor(renderer.m_renderer, 0, 0, 0, 0);
        //SDL_RenderClear(renderer.m_renderer);

        framebuffer.Clear({ 0, 0, 0,255 });
        /*for (int i = 0; i < 100; i++) {
            int x = rand() % framebuffer.m_width;
            int y = rand() % framebuffer.m_height;
            int x2 = rand() % framebuffer.m_width;
            int y2 = rand() % framebuffer.m_height;
            framebuffer.DrawPoint(x, y, { 255,255,255,255 });
        }*/

        glm::vec3 direction{ 0 };
        if (input.GetKeyDown(SDL_SCANCODE_D))   direction.x = 1;
        if (input.GetKeyDown(SDL_SCANCODE_A))   direction.x = -1;
        if (input.GetKeyDown(SDL_SCANCODE_E))   direction.y = 1;
        if (input.GetKeyDown(SDL_SCANCODE_Q))   direction.y = -1;
        if (input.GetKeyDown(SDL_SCANCODE_W))   direction.z = 1;
        if (input.GetKeyDown(SDL_SCANCODE_S))   direction.z = -1;

        cameraTransform.rotation.y += input.GetMouseRelative().x * 0.5f;
        cameraTransform.rotation.x += input.GetMouseRelative().y * 0.5f;

        glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

        cameraTransform.position += offset * 70.0f * time.GetDeltaTime();
        camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());

        //transform.position += direction * 70 * time.GetDeltaTime();
        //transform.rotaion.z += 90 * time.GetDeltaTime();
        //model.Draw(framebuffer, transform.GetMatrix());

        for (auto& actor : actors)
        {
            actor->Draw(framebuffer, camera);
        }

        framebuffer.DrawImage(100, 100, image);

        framebuffer.Update();

        renderer.CopyFrameBuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }


    return 0;
}