#pragma once
class Asset
{
public:
	enum class AssetType {
		TEXTURE = 0,
		MESH,
		SHADER,
		MATERIAL
	};
public:

	Asset();
	virtual AssetType getAssetType() = 0;
	void setId(unsigned int id);
	unsigned int getId();
private:
	unsigned int m_id;
};

