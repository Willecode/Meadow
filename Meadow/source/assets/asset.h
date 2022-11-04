#pragma once
#include <string>
class Asset
{
public:
	enum class AssetType {
		TEXTURE = 0,
		MESH,
		SUBMESH,
		MESH2D,
		SHADER,
		MATERIAL,
		CUBEMAP,
		SOUND
	};
	std::string name;
	bool exposeToUI;
public:

	Asset(std::string name = "Nameless asset", bool exposeUI = true);
	virtual AssetType getAssetType() = 0;
	void setId(unsigned int id);
	unsigned int getId() const;
private:
	unsigned int m_id;
};

