#pragma once
#include "FrameBuffer.h"

namespace PostProcess {
	void Invert(std::vector<color_t>& buffer);

	void Monochrome(std::vector<color_t>& buffer);

	void Brightness(std::vector<color_t>& buffer, int16_t brightness);

	void Noise(std::vector<color_t>& buffer, int8_t noise);

	void Posterize(std::vector<color_t>& buffer, int8_t level);
	void Alpha(std::vector<color_t>& buffer, uint8_t alpha);

	void Threshold(std::vector<color_t>& buffer, int8_t threshold);

	void BoxBlur(std::vector<color_t>& buffer, int width, int height);

	void GaussianBlur(std::vector<color_t>& buffer, int width, int height);

	void Sharpen(std::vector<color_t>& buffer, int width, int height);

	void Edge(std::vector<color_t>& buffer, int width, int height, int threshold);

	void Emboss(std::vector<color_t>& buffer, int width, int height);

}