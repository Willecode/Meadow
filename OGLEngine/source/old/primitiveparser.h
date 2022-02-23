#pragma once
#include "mesh.h"
#include <string>
// STRING FORMATTING
#define FMT_HEADER_ONLY
#include <fmt/core.h>
//--------------------------------------

/*
Parses files containing vertex data, builds objects with meshes based on the data.
*/
class PrimitiveParser
{
public:
	Mesh parse(std::string path);
};

