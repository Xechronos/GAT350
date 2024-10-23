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
#include "Tracer.h"
#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[])
{
    Time time;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("RayTracer", 800, 600);

    Tracer tracer;

    SetBlendMode(BlendMode::Normal);

    Framebuffer framebuffer(renderer, renderer.m_width, renderer.m_height);

    Input input;
    input.Initialize();
    input.Update();

    Camera camera{ 70.0f, (float)framebuffer.m_width / (float)framebuffer.m_height };
    camera.SetView({ 0,0,-20 }, { 0,0,0 });


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

        framebuffer.Clear(ColorConvert(color4_t{0,1,0,1}));
        tracer.Render(framebuffer, camera);

        framebuffer.Update();

        renderer.CopyFrameBuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }


    return 0;
}