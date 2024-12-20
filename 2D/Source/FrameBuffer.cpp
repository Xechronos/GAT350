#include "FrameBuffer.h"
#include "Renderer.h"
#include "Image.h"
#include "MathUtils.h"
#include "Color.h"
#include <iostream>

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!m_texture) {
		std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;
		}
	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	color_t& dest = m_buffer[x + y * m_width];

	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawPointClip(int x, int y, const color_t& color)
{
	color_t& dest = m_buffer[x + y * m_width];

	if (x >= m_width || x < 0 || y < 0 || y >= m_height) return;
	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	if (x + w < 0 || x >= m_width || y + h < 0 || y >= m_height) return;

	int x1 = std::max(x, 0);
	int x2 = std::min(x + w, m_width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + h, m_height);

	for (int sy = y1; sy < y2; sy++) {
		int index = x1 + sy * m_width;
		std::fill(m_buffer.begin() + index, m_buffer.begin() + (index + x2 - x1), color);
	}
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	bool steep = std::abs(dy) > std::abs(dx);
	/*
	if (steep)
	{
		DrawPoint(y, x, color);
	}
	else
	{
		DrawPoint(x, y, color);
	}
	*/
	if (steep) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
	}

	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	for (int x = x1, y = y1; x <= x2; x++) {
		(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);
		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	float m = dy / (float)dx;
	float b = y1 - (m * x1);
	if (dx == 0) {
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y < y2; y++) {
			m_buffer[x1 + y * m_width] = color;
		}
	}
	else {
		if (std::abs(dx) > std::abs(dy)) {
			for (int x = x1; x <= x2; x++) {
				int y = (int)round((m * x) + b);
				m_buffer[x + y * m_width] = color;
			}
		}
		else {
			for (int y = y1; y <= y2; y++) {
				int x = (int)round((y - b) / m);
				m_buffer[x + y * m_width] = color;
			}
		}
	}
}

void Framebuffer::DrawTriangle(int x1, int x2, int x3, int y1, int y2, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::DrawImage(int x, int y, const Image& image)
{
	// check if off-screen
	if (x + image.m_width < 0 || x >= m_width || y + image.m_height < 0 || y >= m_height) return;

	// iterate through image y
	for (int iy = 0; iy < image.m_height; iy++)
	{
		// set screen y 
		int sy = y + iy;
		// check if off-screen, don't draw if off-screen
		if (sy < 0 || sy >= m_height) continue;

		// iterate through image x
		for (int ix = 0; ix < image.m_width; ix++)
		{
			// set screen x
			int sx = x + ix;
			// check if off-screen, don't draw if off-screen
			if (sx < 0 || sx >= m_width) continue;

			// get image pixel color
			color_t color = image.m_buffer[ix + (iy * image.m_width)];
			// check alpha, if 0 don't draw
			if (color.a) continue;
			// set buffer to color
			/*m_buffer[sx + sy * m_width] = color;*/
		}
	}
}

