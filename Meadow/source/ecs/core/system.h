#pragma once
#include <set>
#include "ecstypes.h"
using namespace ECSTypes;
class System
{
public:
	std::set<Entity> m_entities;
};

