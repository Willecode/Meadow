#pragma once
#include <string>
class Asset
{
public:
	enum class AssetType {
		TEXTURE = 0,
		MESH,
		SUBMESH,
		SHADER,
		MATERIAL
	};
	std::string name;
public:

	Asset(std::string name = "Nameless asset");
	virtual AssetType getAssetType() = 0;
	void setId(unsigned int id);
	unsigned int getId();
private:
	unsigned int m_id;
};

