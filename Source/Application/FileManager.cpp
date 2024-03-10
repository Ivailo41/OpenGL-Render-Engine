#include "FileManager.h"

Object* FileManager::readOBJ(std::string fileName)
{
	std::ifstream objFile(fileName, std::ios::in);
	if(!objFile.is_open())
	{
		return nullptr;
	}
	std::string line;

	//Get the name of the file without its extention to name the object
	size_t substrStart = fileName.find_last_of('/') + 1;
	size_t substrEnd = fileName.find_last_of('.');
	std::string objectName = fileName.substr(substrStart, substrEnd - substrStart);

	Object* object = new Object(objectName);

	Material* currentMaterial = nullptr;

	std::vector<Vertex> vertices;
	std::vector<glm::vec3> verticesList;
	std::vector<glm::vec3> verticesNormal;
	std::vector<unsigned> vIndices;
	std::vector<unsigned> vtIndices;
	std::vector<unsigned> vnIndices;

	std::string currentMesh = "";
	unsigned vOffset = 1;
	unsigned vtOffset = 1;
	unsigned vnOffset = 1;

	float X, Y, Z;
	char prefix[3];
	bool* isIndexInList = nullptr;

	while(!objFile.eof())
	{
		std::getline(objFile, line);
		int inputs = sscanf_s(line.c_str(), "%2s %f %f %f", prefix, 3, &X, &Y, &Z);
		if(prefix == "#" || prefix == "s" || inputs < 1)
		{
			continue;
		}
		else if(prefix[0] == 'v')
		{
			if(prefix[1] == 't')
			{
				if (inputs != 3)
				{
					return nullptr;
				}
				vertices.push_back(Vertex(0, 0, 0, X, -Y, 0, 0, 0, 0, 0, 0));
			}
			else if(prefix[1] == 'n')
			{
				if (inputs != 4)
				{
					return nullptr;
				}
				verticesNormal.push_back(glm::vec3(X, Y, Z));
			}
			else
			{
				if (inputs != 4)
				{
					return nullptr;
				}
				verticesList.push_back(glm::vec3(X, Y, Z));
			}
		}
		else if(prefix[0] == 'f')
		{
			unsigned VI1, VI2, VI3, VT1, VT2, VT3, VN1, VN2, VN3;
			inputs = sscanf_s(line.c_str(), "%2s %u/%u/%u %u/%u/%u %u/%u/%u", prefix, 3, &VI1, &VT1, &VN1, &VI2, &VT2, &VN2, &VI3, &VT3, &VN3);

			if (inputs != 10)
			{
				return nullptr;
			}
			VI1 -= vOffset;
			VI2 -= vOffset;
			VI3 -= vOffset;
			VT1 -= vtOffset;
			VT2 -= vtOffset;
			VT3 -= vtOffset;
			VN1 -= vnOffset;
			VN2 -= vnOffset;
			VN3 -= vnOffset;

			vIndices.push_back(VI1);
			vtIndices.push_back(VT1);
			vnIndices.push_back(VN1);

			vIndices.push_back(VI2);
			vtIndices.push_back(VT2);
			vnIndices.push_back(VN2);

			vIndices.push_back(VI3);
			vtIndices.push_back(VT3);
			vnIndices.push_back(VN3);

			if (!isIndexInList[VT1])
			{
				isIndexInList[VT1] = true;

				vertices[VT1].x = verticesList[VI1].x;
				vertices[VT1].y = verticesList[VI1].y;
				vertices[VT1].z = verticesList[VI1].z;

				vertices[VT1].nx = verticesNormal[VN1].x;
				vertices[VT1].ny = verticesNormal[VN1].y;
				vertices[VT1].nz = verticesNormal[VN1].z;
			}
			if(!isIndexInList[VT2])
			{
				isIndexInList[VT2] = true;

				vertices[VT2].x = verticesList[VI2].x;
				vertices[VT2].y = verticesList[VI2].y;
				vertices[VT2].z = verticesList[VI2].z;

				vertices[VT2].nx = verticesNormal[VN2].x;
				vertices[VT2].ny = verticesNormal[VN2].y;
				vertices[VT2].nz = verticesNormal[VN2].z;
			}
			if(!isIndexInList[VT3])
			{
				isIndexInList[VT3] = true;

				vertices[VT3].x = verticesList[VI3].x;
				vertices[VT3].y = verticesList[VI3].y;
				vertices[VT3].z = verticesList[VI3].z;

				vertices[VT3].nx = verticesNormal[VN3].x;
				vertices[VT3].ny = verticesNormal[VN3].y;
				vertices[VT3].nz = verticesNormal[VN3].z;
			}

			glm::vec2 delta1, delta2;
			glm::vec3 tangent, edge1, edge2;

			edge1 = glm::vec3(vertices[VT2].x - vertices[VT1].x,
							  vertices[VT2].y - vertices[VT1].y,
							  vertices[VT2].z - vertices[VT1].z);

			edge2 = glm::vec3(vertices[VT3].x - vertices[VT1].x,
							  vertices[VT3].y - vertices[VT1].y,
							  vertices[VT3].z - vertices[VT1].z);

			delta1 = glm::vec2(vertices[VT2].u - vertices[VT1].u,
							   vertices[VT2].v - vertices[VT1].v);

			delta2 = glm::vec2(vertices[VT3].u - vertices[VT1].u,
							   vertices[VT3].v - vertices[VT1].v);


			float f = 1.0f / (delta1.x * delta2.y - delta2.x * delta1.y);
			tangent.x = f * (delta2.y * edge1.x - delta1.y * edge2.x);
			tangent.y = f * (delta2.y * edge1.y - delta1.y * edge2.y);
			tangent.z = f * (delta2.y * edge1.z - delta1.y * edge2.z);

			vertices[VT1].tx += tangent.x;
			vertices[VT1].ty += tangent.y;
			vertices[VT1].tz += tangent.z;

			vertices[VT2].tx += tangent.x;
			vertices[VT2].ty += tangent.y;
			vertices[VT2].tz += tangent.z;

			vertices[VT3].tx += tangent.x;
			vertices[VT3].ty += tangent.y;
			vertices[VT3].tz += tangent.z;

		}
		else if(prefix[0] == 'o')
		{
			if (currentMesh.empty())
			{
				currentMesh = line.substr(2);
				continue;
			}

			Mesh mesh(vertices, vtIndices);
			mesh.setName(currentMesh);
			mesh.setMaterial(currentMaterial);

			//mesh.attachTo(*object);
			object->addChild(mesh);

			delete[] isIndexInList;

			currentMesh = line.substr(2);
			vOffset += verticesList.size();
			vtOffset += vertices.size();
			vnOffset += verticesNormal.size();

			verticesList.clear();
			vertices.clear();
			verticesNormal.clear();

			vIndices.clear();
			vtIndices.clear();
			vnIndices.clear();
		}
		else if (line.find("usemtl") != std::string::npos)
		{
			isIndexInList = new bool[vertices.size()]();
			std::string name = line.substr(7);
			if(Material::isMaterialInList(name))
			{
				currentMaterial = Material::getMaterial(name);
			}
			else
			{
				currentMaterial = Material::addMaterial(name);
			}
		}
	}
	Mesh mesh(vertices, vtIndices);
	mesh.setName(currentMesh);
	mesh.setMaterial(currentMaterial);

	//mesh.attachTo(*object);
	object->addChild(mesh);

	delete[] isIndexInList;

	objFile.close();

	return object;
}

void FileManager::createDirectory(const std::string path)
{
	struct stat info;
	int statRC = stat(path.c_str(), &info);
	if (statRC != 0)
	{
		std::filesystem::create_directory(path);
		std::cout << "Directory created: " << path << std::endl;
	}
}

std::string FileManager::loadShader(const std::string shaderPath, unsigned type)
{
	std::ifstream shaderFile(shaderPath, std::ios::in);
	if (!shaderFile.is_open())
	{
		return "";
	}

	std::string shaderSource;
	std::string line;

	while (!shaderFile.eof())
	{
		std::getline(shaderFile, line);
		shaderSource += line + "\n";
	}

	shaderFile.close();
	return shaderSource;
}