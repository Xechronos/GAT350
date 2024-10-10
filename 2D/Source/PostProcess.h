#pragma once
#include "FrameBuffer.h"

namespace PostProcess {
	void Invert(std::vector<color_t>& buffer);

	void Monochrome(std::vector<color_t>& buffer);

	void Brightness(std::vector<color_t>& buffer, int16_t brightness);

}