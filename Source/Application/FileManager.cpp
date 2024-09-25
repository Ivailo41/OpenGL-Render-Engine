#include "FileManager.h"
#include <unordered_map>

Object* FileManager::readOBJ(std::string fileName)
{
	std::ifstream objFile(fileName, std::ios::in);
	if (!objFile.is_open())
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

	//Storages for the coords of each vertex, texture coordinate and normal vector
	std::vector<glm::vec3> verticesLoc;
	std::vector<glm::vec2> verticesTexture;
	std::vector<glm::vec3> verticesNormal;

	//final combined vertices that will be passed to the mesh construction
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	//unordered map that keeps track if we already have added the read vertex and if so use it again
	std::unordered_map<std::string, unsigned> vertTable;

	std::string currentMesh = "";

	//We will subtract these from the read indices because we clear the vertices container each time we create a new mesh
	unsigned vOffset = 0;
	unsigned vtOffset = 0;
	unsigned vnOffset = 0;

	float X, Y, Z;
	char prefix[3];

	while (!objFile.eof())
	{
		std::getline(objFile, line);
		int inputs = sscanf_s(line.c_str(), "%2s %f %f %f", prefix, 3, &X, &Y, &Z);
		if (prefix == "#" || prefix == "s" || inputs < 1)
		{
			continue;
		}
		else if (prefix[0] == 'v')
		{
			if (prefix[1] == 't')
			{
				if (inputs != 3)
				{
					return nullptr;
				}
				verticesTexture.push_back(glm::vec2(X,-Y));
			}
			else if (prefix[1] == 'n')
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
				verticesLoc.push_back(glm::vec3(X, Y, Z));
			}
		}
		else if (prefix[0] == 'f')
		{
			unsigned VI1, VI2, VI3, VT1, VT2, VT3, VN1, VN2, VN3;

			inputs = sscanf_s(line.c_str(), "%2s %u/%u/%u %u/%u/%u %u/%u/%u", prefix, 3, &VI1, &VT1, &VN1, &VI2, &VT2, &VN2, &VI3, &VT3, &VN3);

			//Convert the indices to a string of type VI1/VT1/VN1 that will be passed to the unordered map to be hashed.
			//Could use some optimisation in future or make custom hash function that hashes vector of 3 coordinates.
			std::string face1(std::to_string(VI1) + '/' + std::to_string(VT1) + '/' + std::to_string(VN1));
			std::string face2(std::to_string(VI2) + '/' + std::to_string(VT2) + '/' + std::to_string(VN2));
			std::string face3(std::to_string(VI3) + '/' + std::to_string(VT3) + '/' + std::to_string(VN3));

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

			//check if we have already added the read vertex
			unsigned I1;
			if(vertTable.find(face1) == vertTable.end())
			{
				vertTable[face1] = vertices.size();
				vertices.push_back(Vertex(verticesLoc[VI1 - 1].x, verticesLoc[VI1 - 1].y, verticesLoc[VI1 - 1].z, verticesTexture[VT1 - 1].x, verticesTexture[VT1 - 1].y,
					verticesNormal[VN1 - 1].x, verticesNormal[VN1 - 1].y, verticesNormal[VN1 - 1].z, 0, 0, 0));
			}
			I1 = vertTable[face1];
			indices.push_back(I1);

			unsigned I2;
			if (vertTable.find(face2) == vertTable.end())
			{
				vertTable[face2] = vertices.size();
				vertices.push_back(Vertex(verticesLoc[VI2 - 1].x, verticesLoc[VI2 - 1].y, verticesLoc[VI2 - 1].z, verticesTexture[VT2 - 1].x, verticesTexture[VT2 - 1].y,
					verticesNormal[VN2 - 1].x, verticesNormal[VN2 - 1].y, verticesNormal[VN2 - 1].z, 0, 0, 0));
			}
			I2 = vertTable[face2];
			indices.push_back(I2);

			unsigned I3;
			if (vertTable.find(face3) == vertTable.end())
			{
				vertTable[face3] = vertices.size();
				vertices.push_back(Vertex(verticesLoc[VI3 - 1].x, verticesLoc[VI3 - 1].y, verticesLoc[VI3 - 1].z, verticesTexture[VT3 - 1].x, verticesTexture[VT3 - 1].y,
					verticesNormal[VN3 - 1].x, verticesNormal[VN3 - 1].y, verticesNormal[VN3 - 1].z, 0, 0, 0));
			}
			I3 = vertTable[face3];
			indices.push_back(I3);

			//Calculate tangent vector of the vertex based on the face, it is necessary to calculate normal maps in tangent space. 
			glm::vec2 delta1, delta2;
			glm::vec3 tangent, edge1, edge2;

			edge1 = glm::vec3(vertices[I2].x - vertices[I1].x,
				vertices[I2].y - vertices[I1].y,
				vertices[I2].z - vertices[I1].z);

			edge2 = glm::vec3(vertices[I3].x - vertices[I1].x,
				vertices[I3].y - vertices[I1].y,
				vertices[I3].z - vertices[I1].z);

			delta1 = glm::vec2(vertices[I2].u - vertices[I1].u,
				vertices[I2].v - vertices[I1].v);

			delta2 = glm::vec2(vertices[I3].u - vertices[I1].u,
				vertices[I3].v - vertices[I1].v);


			float f = 1.0f / (delta1.x * delta2.y - delta2.x * delta1.y);
			tangent.x = f * (delta2.y * edge1.x - delta1.y * edge2.x);
			tangent.y = f * (delta2.y * edge1.y - delta1.y * edge2.y);
			tangent.z = f * (delta2.y * edge1.z - delta1.y * edge2.z);

			vertices[I1].tx += tangent.x;
			vertices[I1].ty += tangent.y;
			vertices[I1].tz += tangent.z;

			vertices[I2].tx += tangent.x;
			vertices[I2].ty += tangent.y;
			vertices[I2].tz += tangent.z;

			vertices[I3].tx += tangent.x;
			vertices[I3].ty += tangent.y;
			vertices[I3].tz += tangent.z;

		}
		else if (prefix[0] == 'o')
		{
			if (currentMesh.empty())
			{
				currentMesh = line.substr(2);
				continue;
			}

			//if we read the prefix 'o' we create a new mesh with the vertices we have read.
			Mesh mesh(vertices, indices);
			mesh.setName(currentMesh);
			mesh.setMaterial(currentMaterial);

			//Add debug lines to each vertex pointing the normal direction
			for (size_t i = 0; i < vertices.size(); i++)
			{
				mesh.debugLinesContainer.pushLine(Line(
					Point(vertices[i].x, vertices[i].y, vertices[i].z),
					Point(vertices[i].x + vertices[i].nx * 0.002, vertices[i].y + vertices[i].ny * 0.002, vertices[i].z + vertices[i].nz * 0.002)));
			}

			object->addChild(mesh);

			currentMesh = line.substr(2);

			//increment the offset which will be used to subtract the indices of the next mesh so they begin from 0
			vOffset += verticesLoc.size();
			vtOffset += verticesTexture.size();
			vnOffset += verticesNormal.size();

			//clear the used vertices and indices
			vertices.clear();
			indices.clear();

			verticesLoc.clear();
			verticesTexture.clear();
			verticesNormal.clear();
		}
		else if (line.find("usemtl") != std::string::npos)
		{
			std::string name = line.substr(7);
			if (Material::isMaterialInList(name))
			{
				currentMaterial = Material::getMaterial(name);
			}
			else
			{
				currentMaterial = Material::addMaterial(name);
			}
		}
	}
	//At the end add the final mesh with the last vertices and close the file
	Mesh mesh(vertices, indices);
	mesh.setName(currentMesh);
	mesh.setMaterial(currentMaterial);

	//Add debug lines to each vertex pointing the normal direction
	for (size_t i = 0; i < vertices.size(); i++)
	{
		mesh.debugLinesContainer.pushLine(Line(
			Point(vertices[i].x, vertices[i].y, vertices[i].z),
			Point(vertices[i].x + vertices[i].nx * 0.002, vertices[i].y + vertices[i].ny * 0.002, vertices[i].z + vertices[i].nz * 0.002)));
	}

	object->addChild(mesh);

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