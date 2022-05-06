#include "asset.h"

Asset::Asset(std::string name): m_id(0), name(name)
{
}

void Asset::setId(unsigned int id)
{
	m_id = id;
}

unsigned int Asset::getId() const
{
	return m_id;
}
