#include "FileManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <unordered_map>

bool FileManager::isRunning = false;

void FileManager::init()
{
	if(!isRunning)
	{
		isRunning = true;
		//init code
	}
}

void FileManager::stop()
{
	if(isRunning)
	{
		isRunning = false;
		//stop code
	}
}

bool FileManager::loadOBJ(const std::string& fileName)
{
	checkRunState();

	std::ifstream objFile(fileName, std::ios::in);
	if (!objFile.is_open())
	{
		std::cerr << "Could not open mesh file for read : " << fileName << std::endl;
		return false;
	}
	std::string line;

	//Get the name of the file without its extention to name the object
	size_t substrStart = fileName.find_last_of('/') + 1;
	size_t substrEnd = fileName.find_last_of('.');
	std::string objectName = fileName.substr(substrStart, substrEnd - substrStart);

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

	//TODO: make use of shared pointers instead using new
	Object* object = new Object(objectName);

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
					std::cerr << "Model has incorrect vertex textures" << std::endl;
					delete object;
					return false;
				}
				verticesTexture.push_back(glm::vec2(X,-Y));
			}
			else if (prefix[1] == 'n')
			{
				if (inputs != 4)
				{
					std::cerr << "Model has incorrect vertex normals" << std::endl;
					delete object;
					return false;
				}
				verticesNormal.push_back(glm::vec3(X, Y, Z));
			}
			else
			{
				if (inputs != 4)
				{
					std::cerr << "Model has incorrect vertex coordinates" << std::endl;
					delete object;
					return false;
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
				std::cerr << "Model has incorrect faces data" << std::endl;
				delete object;
				return false;
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
			if (isMaterialInList(name))
			{
				currentMaterial = getMaterial(name);
			}
			else
			{
				currentMaterial = addMaterial(name);
			}
		}
	}

	objFile.close();
	//At the end add the final mesh with the last vertices and close the file
	Mesh mesh(vertices, indices);
	mesh.setName(currentMesh);
	mesh.setMaterial(currentMaterial);

	//Add debug lines to each vertex pointing the normal direction
	//TODO: Move that feature to the geometry shader 
	for (size_t i = 0; i < vertices.size(); i++)
	{
		mesh.debugLinesContainer.pushLine(Line(
			Point(vertices[i].x, vertices[i].y, vertices[i].z),
			Point(vertices[i].x + vertices[i].nx * 0.002, vertices[i].y + vertices[i].ny * 0.002, vertices[i].z + vertices[i].nz * 0.002)));
	}

	object->addChild(mesh);

	Scene::activeScene->sceneObjects.push_back(object);
	return true;
}

void FileManager::createDirectory(const std::string& path)
{
	checkRunState();

	struct stat info;
	int statRC = stat(path.c_str(), &info);
	if (statRC != 0)
	{
		std::filesystem::create_directory(path);
		std::cout << "Directory created: " << path << std::endl;
	}
}

//OLD FUNCTION DO NOT USE
GLuint FileManager::loadTexture(const std::string& texturePath)
{
	checkRunState();

	GLuint texture = 0;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << texturePath << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

void FileManager::loadTextures(const std::vector<std::string>& texturesPaths)
{
	checkRunState();

	std::mutex textureMutex;
	//will store the API calls to OpenGL that will be executed on the main thread
	std::vector<std::function<void()>> commandVector;

	std::vector<Texture*>& textures = Scene::activeScene->textures;
	//alocating the needed size for the textures
	unsigned texturesSize = textures.size();
	unsigned texturesToAdd = texturesPaths.size();
	//IF PATHS ARE INVALID THE VECTOR WILL STILL RESIZE!!
	textures.resize(texturesSize + texturesToAdd);

	{
		std::vector<std::future<void>> futures;
		for (unsigned i = 0; i < texturesToAdd; i++)
		{
			//We store the returned future objects in a vector, when they are destructed the program should wait untill the end of the async function
			futures.push_back(std::async(std::launch::async, [&texturesPaths, i, &textureMutex, &commandVector, &textures, texturesSize]()
				{
					//Will carry the i index so we return the textures in order, since they are loaded asynchronous
					int width, height, nrChannels;
					unsigned char* data = stbi_load(texturesPaths[i].c_str(), &width, &height, &nrChannels, 0);
					if (data)
					{

						std::lock_guard<std::mutex> lock(textureMutex);
						//OpenGL API calls should be made ONLY on the main thread, so we are storing them in a vector to call them later
						commandVector.push_back([data, i, width, height, nrChannels, &textures, texturesSize, &texturesPaths]()
							{
								GLuint textureId = 0;
								glGenTextures(1, &textureId);
								glBindTexture(GL_TEXTURE_2D, textureId);

								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
								glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
								glGenerateMipmap(GL_TEXTURE_2D);

								//sets only id of the Texture struct, type and filepath will not be set, consider changing it
								textures[texturesSize + i] = new Texture(textureId, texturesPaths[i].c_str());
								stbi_image_free(data);
							});
					}
					else
					{
						std::lock_guard<std::mutex> lock(textureMutex);
						commandVector.push_back([data, i, &texturesPaths]() 
							{
								//Create a logging class that will handle messages
								std::cout << "Failed to load texture : " << texturesPaths[i] << std::endl;
								stbi_image_free(data);
							});
					}
				}));
		}
	}
	
	std::lock_guard<std::mutex> lock(textureMutex);

	for(unsigned i = 0; i < commandVector.size(); i++)
	{
		auto command = commandVector[i];
		command();
	}
}

GLuint FileManager::loadCubemap(const std::string texturePaths[6])
{
	checkRunState();

	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for(size_t i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if(data)
		{

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			//create logging class that will handle messages
			std::cout << "Failed to load cubemap face: " << texturePaths[i] << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}

//Change it to create Shader object that will be put in a hashmap, make other function that will return the wanted shader from the map
std::string FileManager::loadShader(const std::string& shaderPath)
{
	checkRunState();

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

bool FileManager::loadShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragShaderPath)
{
	checkRunState();

	//check if shader exists in map and return it if so

	const std::string* paths[] = { &vertexShaderPath , &fragShaderPath };
	std::string result[2];

	for (size_t i = 0; i < 2; i++)
	{
		std::ifstream saderFile(*paths[i], std::ios::in);
		if (!saderFile.is_open())
		{
			return false;
		}

		std::string line;

		while (!saderFile.eof())
		{
			std::getline(saderFile, line);
			result[i] += line + "\n";
		}

		saderFile.close();
	}

	std::pair<std::string, Shader> shaderPair(shaderName, Shader(result[0], result[1]));
	return Shader::shadersList.insert(shaderPair).second;
}

Material* const FileManager::getMaterial(const std::string& name)
{
	unsigned materialsCount = Scene::activeScene->materials.size();
	for (size_t i = 0; i < materialsCount; i++)
	{
		if (Scene::activeScene->materials[i]->name == name)
		{
			return Scene::activeScene->materials[i];
		}
	}

	return nullptr;
}

unsigned FileManager::isMaterialInList(const std::string& name)
{
	unsigned timesFound = 0;
	unsigned materialsCount = Scene::activeScene->materials.size();
	for (size_t i = 0; i < materialsCount; i++)
	{
		if (Scene::activeScene->materials[i]->name.substr(0, Scene::activeScene->materials[i]->name.find_last_of('_') - 1) == name)
		{
			timesFound++;
		}
	}
	return timesFound;
}

Material* FileManager::addMaterial(const std::string name)
{
	Material* material = new Material(name);
	Scene::activeScene->materials.push_back(material);
	return material;
}

bool FileManager::removeMaterial(const std::string name)
{
	unsigned materialsCount = Scene::activeScene->materials.size();
	for (size_t i = 0; i < materialsCount; i++)
	{
		if (Scene::activeScene->materials[i]->name == name)
		{
			delete Scene::activeScene->materials[i];
			Scene::activeScene->materials.erase(Scene::activeScene->materials.begin() + i);
			return true;
		}
	}
	return false;
}

void FileManager::checkRunState()
{
	assert(isRunning); //Forgot to call the initialisation function init() before calling functions
}
