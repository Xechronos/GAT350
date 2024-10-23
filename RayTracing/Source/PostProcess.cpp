#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>
#include <cmath>

void PostProcess::Invert(std::vector<color_t>& buffer)
{
	/*for (auto& color : buffer) {
		color.r = 255 - color.r;
		color.g = 255 - color.g;
		color.b = 255 - color.b;
	}*/

	std::for_each(buffer.begin(), buffer.end(), [](auto& c) {
		c.r = 255 - c.r;
		c.g = 255 - c.g;
		c.b = 255 - c.b;
		});
}

void PostProcess::Monochrome(std::vector<color_t>& buffer)
{
	std::for_each(buffer.begin(), buffer.end(), [](auto& c) {
		uint8_t average = static_cast<uint8_t>((c.r + c.g + c.b) / 3);
		c.r = average;
		c.g = average;
		c.b = average;
		});
}

void PostProcess::Brightness(std::vector<color_t>& buffer, int16_t brightness)
{
	std::for_each(buffer.begin(), buffer.end(), [brightness](auto& c) {
		c.r = static_cast<uint8_t>(Clamp(c.r + brightness, 0, 255));
		c.g = static_cast<uint8_t>(Clamp(c.g + brightness, 0, 255));
		c.b = static_cast<uint8_t>(Clamp(c.b + brightness, 0, 255));
		});
}
void PostProcess::Noise(std::vector<color_t>& buffer, int8_t noise)
{
	int offset = (rand() % ((noise * 2) + 1)) - noise;
	std::for_each(buffer.begin(), buffer.end(), [offset](auto& c) {
		c.r = static_cast<uint8_t>(Clamp(c.r + offset, 0, 255));
		c.g = static_cast<uint8_t>(Clamp(c.g + offset, 0, 255));
		c.b = static_cast<uint8_t>(Clamp(c.b + offset, 0, 255));
		});
}

void PostProcess::Posterize(std::vector<color_t>& buffer, int8_t level)
{
	std::for_each(buffer.begin(), buffer.end(), [level](auto& c) {
		c.r = (c.r / level) * level;
		c.g = (c.g / level) * level;
		c.b = (c.b / level) * level;
		});
}

void PostProcess::Alpha(std::vector<color_t>& buffer, uint8_t alpha)
{
	std::for_each(buffer.begin(), buffer.end(), [alpha](auto& c) {
		c.a = alpha;
		});
}

void PostProcess::Threshold(std::vector<color_t>& buffer, int8_t threshold)
{
}

void PostProcess::BoxBlur(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;

	int k[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % width;
		int y = i / width;

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t color = buffer[i];
		color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
		color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
		color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
	}

}

void PostProcess::GaussianBlur(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;

	int k[3][3] = {
		{1,2,1},
		{2,4,2},
		{1,2,1}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % width;
		int y = i / width;

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t color = buffer[i];
		color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
		color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
		color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
	}
}

void PostProcess::Sharpen(std::vector<color_t>& buffer, int width, int height)
{
	std::vector<color_t> source = buffer;

	int k[3][3] = {
		{0,-1,0},
		{-1,5,-1},
		{0,-1,0}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % width;
		int y = i / width;

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int r = 0;
		int g = 0;
		int b = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy) * width];
				int weight = k[iy][ix];

				r += pixel.r * weight;
				g += pixel.g * weight;
				b += pixel.b * weight;
			}
		}
		color_t color = buffer[i];
		color.r = static_cast<uint8_t>(Clamp(r,0,255));
		color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
		color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
	}
}

void PostProcess::Edge(std::vector<color_t>& buffer, int width, int height, int threshold)
{
	std::vector<color_t> source = buffer;

	int hk[3][3] = {
		{1,0,-1},
		{2,0,-1},
		{1,0,-1}
	};
	int vk[3][3] = {
		{-1,-2,-1},
		{0,0,0},
		{1,2,1}
	};
	for (int i = 0; i < buffer.size(); i++) {
		int x = i % width;
		int y = i / width;

		if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

		int h = 0;
		int v = 0;

		for (int iy = 0; iy < 3; iy++) {
			for (int ix = 0; ix < 3; ix++) {
				color_t pixel = source[(x + ix - 1) + (y + iy) * width];

				h += pixel.r * hk[iy][ix];
				v += pixel.r * vk[iy][ix];
			}
		}
		int m = static_cast<int>(std::sqrt(h * h + v * v));
		m = (m >= threshold) ? m : 0;
		uint8_t c = std::clamp(m, 0, 255);
		color_t color = buffer[i];
		color.r = c;
		color.g = c;
		color.b = c;
	}
}

void PostProcess::Emboss(std::vector<color_t>& buffer, int width, int height)
{
	{
		// create source buffer to read from, buffer will be written to
		std::vector<color_t> source = buffer;

		// kernal values
		int16_t k[3][3] =
		{
			{-2,-1,0},
			{-1,0,1},
			{0,1,2}
		};

		// process buffer pixels
		for (int i = 0; i < buffer.size(); i++)
		{
			// get pixel x, y
			int x = i % width;
			int y = i / width;

			// check bounds (x and y must be one pixel away from buffer edges)
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			// color values from kernel operation
			int r = 0;
			int g = 0;
			int b = 0;

			// process kernel pixels
			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					// get pixel in kernel
					const color_t& pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					// get kernel weight
					int weight = k[iy][ix];

					// accumulate color values of kernel pixels
					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}

			// set buffer color, set values from average of kernel color values
			color_t& color = buffer[i];
			// help from ChatGPT
			color.r = static_cast<uint8_t>(Clamp(r + 128, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(g + 128, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(b + 128, 0, 255));
		}
	}
}
