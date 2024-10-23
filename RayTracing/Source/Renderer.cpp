#include "Renderer.h"
#include "FrameBuffer.h"
#include <SDL.h>
#include <iostream>

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{// initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool Renderer::CreateWindow(const std::string& name, int width, int height)
{
    m_width = width;
    m_height = height;
    // create window
    // returns pointer to window if successful or nullptr if failed
    m_window = SDL_CreateWindow("Game Engine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    // create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    return true;
}

void Renderer::CopyFrameBuffer(const Framebuffer& framebuffer) {
    SDL_RenderCopy(m_renderer, framebuffer.m_texture, NULL, NULL);
}
