#include "Renderer.h"
#include "FrameBuffer.h"
#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 800, 600);

    Framebuffer framebuffer(renderer, 400, 300);

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

        framebuffer.Clear({ 255,0,0,255 });
        for (int i = 0; i < 100; i++) {
            int x = rand() % 400;
            int y = rand() % 300;
            framebuffer.DrawPoint(x, y, { 255,255,255,255 });
        }

        framebuffer.DrawRect(10, 10, 100, 150, {255,0,0,255});
        framebuffer.Update();

        renderer.CopyFrameBuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.m_renderer);
    }


    return 0;
}