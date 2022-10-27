#include "imagemanipulation.h"

void ImageManipulation::extractChannel(unsigned char* source, int sourceW, int sourceH, std::vector<unsigned char>& destination, int& destinationW, int& destinationH, Renderer::ImageFormat sourceFMT, IMGChannel channel)
{
	int channelIndex = 0;
	if (channel == IMGChannel::R)
		channelIndex = 0;
	if (channel == IMGChannel::G)
		channelIndex = 1;
	if (channel == IMGChannel::B)
		channelIndex = 2;

	int channelCount = 0;
	if (sourceFMT == Renderer::ImageFormat::R)
		channelCount = 1;
	if (sourceFMT == Renderer::ImageFormat::RGB)
		channelCount = 3;
	if (sourceFMT == Renderer::ImageFormat::RGBA)
		channelCount = 4;
	unsigned char* temp = new unsigned char[sourceW * sourceH];
	for (int i = 0, j = 0; i < sourceW * sourceH; i++) {

		temp[i] = source[j + channelIndex];
		j += channelCount;
	}
	destination.clear();
	destination.insert(destination.end(), &temp[0], &temp[sourceW * sourceH]);

	destinationW = sourceW;
	destinationH = sourceH;

	delete[] temp;
}
