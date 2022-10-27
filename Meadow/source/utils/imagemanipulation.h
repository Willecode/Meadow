#pragma once
#include "renderer/renderer.h"
namespace ImageManipulation {
	enum class IMGChannel {
		R = 0,
		G,
		B
	};
	void extractChannel(unsigned char* source, int sourceW, int sourceH, std::vector<unsigned char>& destination, int& destinationW, int& destinationH, Renderer::ImageFormat sourceFMT, IMGChannel chnl);
};