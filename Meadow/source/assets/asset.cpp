#include "asset.h"

Asset::Asset(): m_id(0)
{
}

void Asset::setId(unsigned int id)
{
	m_id = id;
}

unsigned int Asset::getId()
{
	return m_id;
}
