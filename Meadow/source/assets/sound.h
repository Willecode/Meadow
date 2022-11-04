#pragma once
#include "asset.h"

namespace Meadow {
	class Sound : public Asset
	{
	public:
		Sound(std::string path, std::string name = "Nameless sound") : Asset(name), path(path)
		{

		}
		std::string path;
		AssetType getAssetType() override
		{
			return AssetType::SOUND;
		}
	};

}