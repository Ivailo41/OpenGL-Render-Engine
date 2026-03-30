#include "FileManager.h"
#include "FileManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <algorithm>

#include "stb_image.h"
#include <sys/stat.h>

#include <unordered_map>
#include <sstream>

bool FileManager::init()
{
	if(running)
	{
		return true;
	}

	LOG_TRACE("File Manager initialized!");
	//init code
	running = true;

	return true;
}

void FileManager::stop()
{
	if(running)
	{
		LOG_TRACE("File Manager stopped!");
		running = false;
		//stop code
	}
}

RawModel FileManager::loadOBJ(const std::filesystem::path& filePath) const
{
	assert(running);

	std::ifstream objFile(filePath, std::ios::in);
	if (!objFile.is_open())
	{
		LOG_ERROR("Could not open mesh file for read: " + filePath.string());
		throw std::runtime_error("Could not open mesh file for read: " + filePath.string());
	}


	//Get the name of the file without its extention to name the object
	std::string objectName = filePath.filename();
	std::replace(objectName.begin(), objectName.end(), '\\', '/'); //replace backslash with forward slash for compatibility
	size_t substrStart = objectName.find_last_of('/') + 1;
	size_t substrEnd = objectName.find_last_of('.');
	objectName = objectName.substr(substrStart, substrEnd - substrStart);

	char buffer[2048];
	char prefix[7];

	//Storages for the coords of each vertex, texture coordinate and normal vector
	std::vector<glm::vec3> vertPosList;
	std::vector<glm::vec2> vertTextureList;
	std::vector<glm::vec3> vertNormalList;

	//final combined vertices that will be passed to the mesh construction
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	std::vector<RawMaterialGroup> materialGroups;

	std::vector<char*> tokens;
	tokens.reserve(10); //reserve space for 10 tokens, can be adjusted based on expected face size

	//unordered map that keeps track if we already have added the read vertex and if so use it again
	//std::unordered_map<std::string, unsigned> vertTable;

	std::string currentMeshName = "";

	//We will subtract these from the read indices because we clear the vertices container each time we create a new mesh
	unsigned vOffset = 0;
	unsigned vtOffset = 0;
	unsigned vnOffset = 0;

	RawModel object;
	object.name = objectName;

	while (!objFile.eof())
	{
		//Potential error if the line is too long, but we can ignore it for now
		objFile.getline(buffer, sizeof(buffer));

		getPrefixFromLine(buffer, prefix);

		if (prefix[0] == '\0' || prefix[0] == '#' || prefix[0] == 's')
		{
			continue;
		}

		else if (prefix[0] == 'v')
		{
			if (prefix[1] == 't')
			{
				float x, y;
				unsigned inputs = std::sscanf(buffer, "%*s %f %f", &x, &y); //read the texture coordinates

				if (inputs != 2)
				{
					LOG_ERROR("Model has incorrect vertex textures");
					throw std::runtime_error("Model has incorrect vertex textures");
				}

				vertTextureList.push_back(glm::vec2(x,-y));
			}
			else if (prefix[1] == 'n')
			{
				float x, y, z;
				unsigned inputs = std::sscanf(buffer, "%*s %f %f %f", &x, &y, &z); //read the vertex normal

				if (inputs != 3)
				{
					LOG_ERROR("Model has incorrect vertex normals");
					throw std::runtime_error("Model has incorrect vertex normals");
				}

				vertNormalList.push_back(glm::vec3(x, y, z));
			}
			else
			{
				float x, y, z;
				unsigned inputs = std::sscanf(buffer, "%*s %f %f %f", &x, &y, &z); //read the vertex position

				if (inputs != 3)
				{
					LOG_ERROR("Model has incorrect vertex coordinates");
					throw std::runtime_error("Model has incorrect vertex coordinates");
				}

				vertPosList.push_back(glm::vec3(x, y, z));
			}
		}
		else if (prefix[0] == 'f')
		{
			tokens.clear();
			unsigned inputs = tokenizeOBJFaceLine(tokens, buffer);
			unsigned faceVertsCount = inputs - 1;

			//for each token (a single token is one vertex data) we will parse that vertex data based on what information we are given for the vertex.
			//the possible variations are to have vertexPos/vertexTextures/vertexData or some of them to be absent as seen in the formats below.

			bool hasNormalData = false;
			bool hasTextureData = false;

			//each token is a vertex data
			std::vector<unsigned> faceVertIndices(faceVertsCount);
			for (size_t i = 0; i < faceVertsCount; i++)
			{
				unsigned v = 0;
				unsigned vt = 0;
				unsigned vn = 0;

				char* face = tokens[i+1];

				if (sscanf(face, "%d/%d/%d", &v, &vt, &vn) == 3) {
					// v/vt/vn
				}
				else if (sscanf(face, "%d//%d", &v, &vn) == 2) {
					// v//vn
				}
				else if (sscanf(face, "%d/%d", &v, &vt) == 2) {
					// v/vt
				}
				else if (sscanf(face, "%d", &v) == 1) {
					// v
				}

				glm::vec3 vertPos;
				glm::vec2 vertTexture(1, 1);
				glm::vec3 vertNormal(0, 0, 1);

				if(v != 0)
				{
					vertPos = vertPosList[v - vOffset - 1];
				}
				if(vt != 0)
				{
					vertTexture = vertTextureList[vt - vtOffset - 1];
					hasTextureData = true;
				}
				if(vn != 0)
				{
					vertNormal = vertNormalList[vn - vnOffset - 1];
					hasNormalData = true;
				}

				vertices.push_back(Vertex(vertPos, vertTexture, vertNormal));
				faceVertIndices[i] = vertices.size() - 1;
			}

			//Here we need to triangulate if we have an ngon and do the steps below for each triangle
			for (size_t i = 1; i <= faceVertsCount - 2; i++)
			{
				unsigned I1 = faceVertIndices[0];
				unsigned I2 = faceVertIndices[i];
				unsigned I3 = faceVertIndices[i+1];

				//check if we miss normal data, if so, calculate flat normal
				if (!hasNormalData)
				{
					glm::vec3 p0 = vertices[I1].getPos();
					glm::vec3 p1 = vertices[I2].getPos();
					glm::vec3 p2 = vertices[I3].getPos();

					glm::vec3 edge1 = p1 - p0;
					glm::vec3 edge2 = p2 - p0;

					glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

					// Assign the same normal to all three vertices
					vertices[I1].setNormal(faceNormal);
					vertices[I2].setNormal(faceNormal);
					vertices[I3].setNormal(faceNormal);
				}

				//Calculate tangent if the 3D model is unwraped (has texture data)
				//if all vertices share the same point on the UV plane, then the delta1 and delta2 variables will be 0 resulting division by 0
				if (hasTextureData)
				{
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

				//push the triangle
				indices.push_back(I1);
				indices.push_back(I2);
				indices.push_back(I3);

				materialGroups.back().indicesCount += 3;
			}
		}
		else if (prefix[0] == 'o')
		{
			tokens.clear();
			unsigned inputs = tokenizeOBJFaceLine(tokens, buffer);

			if (currentMeshName.empty())
			{
				currentMeshName = tokens[1];
				continue;
			}

			//if we read the prefix 'o' we create a new mesh with the vertices we have read.
			RawMesh mesh = {vertices, indices, currentMeshName, materialGroups};
			RawModelNode modelNode;
			modelNode.meshIndices.push_back(object.meshes.size());

			object.meshes.push_back(mesh);
			object.root.children.push_back(modelNode);

			currentMeshName = tokens[1];

			//increment the offset which will be used to subtract the indices of the next mesh so they begin from 0
			vOffset += vertPosList.size();
			vtOffset += vertTextureList.size();
			vnOffset += vertNormalList.size();

			//clear the used vertices, indices and material groups
			vertices.clear();
			indices.clear();
			materialGroups.clear();

			vertPosList.clear();
			vertTextureList.clear();
			vertNormalList.clear();
		}
		else if (prefix[0] == 'u')
		{
			tokens.clear();
			unsigned inputs = tokenizeOBJFaceLine(tokens, buffer);

			materialGroups.emplace_back();
			materialGroups.back().offset = indices.size();

			materialGroups.back().materialName = tokens[1];
		}
	}

	objFile.close();
	//At the end add the final mesh with the last vertices and close the file
	RawMesh mesh = {vertices, indices, currentMeshName, materialGroups};
	RawModelNode modelNode;
	modelNode.meshIndices.push_back(object.meshes.size());

	object.meshes.push_back(mesh);
	object.root.children.push_back(modelNode);

	return object;
}

void FileManager::createDirectory(const std::string& path) const {
	assert(running);

	struct stat info;
	int statRC = stat(path.c_str(), &info);
	if (statRC != 0)
	{
		std::filesystem::create_directory(path);
		LOG_TRACE("Directory created: " + path);
	}
}

inline bool FileManager::checkFileExistence(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

std::vector<RawTexture> FileManager::loadTextures(const std::vector<std::filesystem::path>& texturesPaths) const {
	assert(running);

	unsigned texturesToAdd = 0;

	//Check if the paths are valid files before resizing the vector to avoid creating invalid textures
	for (size_t i = 0; i < texturesPaths.size(); i++)
	{
		if(checkFileExistence(texturesPaths[i]))
		{
			texturesToAdd++;
		}
		else
		{
			LOG_ERROR("Texture not found: " + texturesPaths[i].string());
		}
	}

	std::vector<RawTexture> rawTextures(texturesToAdd);

	{
		std::vector<std::future<void>> futures;
		for (unsigned i = 0; i < texturesToAdd; i++)
		{
			//We store the returned future objects in a vector, when they are destructed the program should wait untill the end of the async function
			futures.push_back(std::async(std::launch::async, [&texturesPaths, i, &rawTextures]()
				{
					//Will carry the i index so we return the textures in order, since they are loaded asynchronous
					int width, height, nrChannels;
					if (unsigned char* data = stbi_load(texturesPaths[i].c_str(), &width, &height, &nrChannels, 0))
					{
						rawTextures[i] = {data, width, height, nrChannels};
					}
					else
					{
						LOG_ERROR("Failed to load texture: " + texturesPaths[i].string());
					}
				}));
		}
	}
	return rawTextures;
}

std::vector<std::string> FileManager::loadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geometryPath) const
{
	assert(running);

	//check if shader exists in map and return it if so
	std::filesystem::path paths[] = {vertexPath, fragmentPath, geometryPath};
	std::vector<std::string> result(3);

	for (size_t i = 0; i < 3; i++)
	{
		if (paths[i].empty())
		{
			result[i] = "";
			continue;
		}

		std::ifstream shaderFile(paths[i], std::ios::in);
		if (!shaderFile.is_open())
		{
			throw std::runtime_error("Failed to open shader file: " + paths[i].string());
		}

		std::string line;

		while (!shaderFile.eof())
		{
			std::getline(shaderFile, line);
			result[i] += line + "\n";
		}

		shaderFile.close();
	}

	return result;
}

unsigned FileManager::tokenizeOBJFaceLine(std::vector<char*>& tokens, char* line)
{
	char* pch;
	pch = strtok(line, " \t"); // Tokenize the line by spaces and tabs
	while(pch != nullptr) 
	{
		//this function creates and destroys a string object for each token, which is not efficient
		tokens.emplace_back(pch);
		pch = strtok(nullptr, " \t");
	}

	return tokens.size();
}

void FileManager::getPrefixFromLine(char* line, char* prefix)
{
	// Extract the prefix from the line
	char* spacePos = strchr(line, ' ');
	if (spacePos != nullptr)
	{
		size_t prefixLength = spacePos - line;
		strncpy(prefix, line, prefixLength);
		prefix[prefixLength] = '\0'; // Null-terminate the prefix string
	}
	else
	{
		strcpy(prefix, line); // If no space found, copy the whole line as prefix
	}
}
