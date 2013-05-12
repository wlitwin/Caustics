#include "wavefront.hpp"
#include "shelper.hpp"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;

WavefrontLoader::WavefrontLoader() :
	is_loaded(false)
{
}

WavefrontLoader::WavefrontLoader(const string& file)
{
	LoadFile(file);
}

bool WavefrontLoader::LoadFile(const string& file)
{
	is_loaded = false;
	vertices.erase(vertices.begin(), vertices.end());
	tex_coords.erase(tex_coords.begin(), tex_coords.end());
	normals.erase(normals.begin(), normals.end());
	faces.erase(faces.begin(), faces.end());

	// Open the file
	std::ifstream inFile(file.c_str());
	if (!inFile.is_open()) {
		std::cerr << "Wavefront: Failed to open file: " << file << "\n";
		return false;
	}

	// File is open, lets read it
	int lineNum = 0;
	string line;
	while (std::getline(inFile, line))
	{
		++lineNum;
		std::stringstream ss(trim(line));
		string first;
		ss >> first;
		
		float x, y, z, w;
		if (first == "v")
		{
			ss >> x; ss >> y; ss >> z;
			if (!ss) {
				std::cerr << "Error reading vertex: " << lineNum << "\n";
				return false;
			}
			ss >> w;
			if (ss) {
				x /= w;
				y /= w;
				z /= w;
			}

			vertices.push_back(glm::vec3(x, y, z));
		}
		else if (first == "vn")
		{
			ss >> x; ss >> y; ss >> z;
			if (!ss) {
				std::cerr << "Error reading normal: " << lineNum << "\n";
				return false;
			}

			normals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (first == "vt")
		{
			ss >> x; ss >> y;
			if (!ss) {
				std::cerr << "Error reading texture coordinate: "
						  << lineNum << "\n";
				return false;
			}
			ss >> z;
			if (ss) {
				x /= z;
				y /= z;
			}

			tex_coords.push_back(glm::vec2(x, y));	
		}
		else if (first == "f")
		{
			string f1, f2, f3, f4;
			ss >> f1; ss >> f2; ss >> f3;
			if (!ss) {
				std::cerr << "Error reading face: " << lineNum << "\n";
				return false;
			}
			ss >> f4;
			if (ss) {
				// It's a quad, need to split into two triangles
				std::cerr << "Quad specified, TODO: " << lineNum << "\n";
				return false;
			} else {
				// It's a triangle pick it apart					
				std::vector<string> coords(split(f1, '/'));
				if (coords.size() < 2) {
					std::cerr << "Bad face specified: " << lineNum << "\n";
					return false;
				}
			}
		}
	}

	is_loaded = true;
	return true;
}
