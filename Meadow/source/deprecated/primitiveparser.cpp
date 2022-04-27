#include "primitiveparser.h"
#include <fstream>
#include <iostream>
#include <vector>

Mesh PrimitiveParser::parse(std::string path)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	std::cout << fmt::format("PrimitiveParser: parsing file {}", path);
	std::ifstream primitiveFile;
	primitiveFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		primitiveFile.open(path);
		std::string line;
		while (std::getline(primitiveFile, line))
		{
			if (!line.empty()) {

			}
		}

	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::PRIMITIVEPARSER::FILE_NOT_SUCCESFULLY_READ\n";
	}
	return Mesh(vertices, indices);
}
