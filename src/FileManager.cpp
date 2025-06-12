#include "FileManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <sys/stat.h>

#include <unordered_map>
#include <sstream>

bool FileManager::isRunning = false;

bool FileManager::init()
{
	assert(!isRunning);

	std::cout << "Initializing File Manager!" << std::endl;
	isRunning = true;
	//init code

	return true;
}

void FileManager::stop()
{
	if(isRunning)
	{
		std::cout << "Shutting down File Manager!" << std::endl;
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

	//Storages for the coords of each vertex, texture coordinate and normal vector
	std::vector<glm::vec3> vertPosList;
	std::vector<glm::vec2> vertTextureList;
	std::vector<glm::vec3> vertNormalList;

	//final combined vertices that will be passed to the mesh construction
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	std::vector<MaterialGroup> materialGroups;

	//unordered map that keeps track if we already have added the read vertex and if so use it again
	std::unordered_map<std::string, unsigned> vertTable;

	std::string currentMeshName = "";

	//We will subtract these from the read indices because we clear the vertices container each time we create a new mesh
	unsigned vOffset = 0;
	unsigned vtOffset = 0;
	unsigned vnOffset = 0;

	float X, Y, Z;
	char prefix[3];

	//TODO: make use of shared pointers instead using new
	BaseObject* object = new BaseObject(objectName);

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
				vertTextureList.push_back(glm::vec2(X,-Y));
			}
			else if (prefix[1] == 'n')
			{
				if (inputs != 4)
				{
					std::cerr << "Model has incorrect vertex normals" << std::endl;
					delete object;
					return false;
				}
				vertNormalList.push_back(glm::vec3(X, Y, Z));
			}
			else
			{
				if (inputs != 4)
				{
					std::cerr << "Model has incorrect vertex coordinates" << std::endl;
					delete object;
					return false;
				}
				vertPosList.push_back(glm::vec3(X, Y, Z));
			}
		}
		else if (prefix[0] == 'f')
		{
			//Split the face line into tokens
			std::vector<std::string> tokens;
			unsigned faceVertsCount = tokenizeOBJFaceLine(tokens, line);

			//for each token (a single token is one vertex data) we will parse that vertex data based on what information we are given for the vertex.
			//the possible variations are to have vertexPos/vertexTextures/vertexData or some of them to be absent as seen in the formats below.

			bool hasNormalData = false;
			bool hasTextureData = false;

			//each token is a vertex data
			std::vector<unsigned> faceVertIndices(faceVertsCount);
			for (size_t i = 0; i < tokens.size(); i++)
			{
				unsigned v = 0;
				unsigned vt = 0;
				unsigned vn = 0;

				if (sscanf(tokens[i].c_str(), "%d/%d/%d", &v, &vt, &vn) == 3) {
					// v/vt/vn
				}
				else if (sscanf(tokens[i].c_str(), "%d//%d", &v, &vn) == 2) {
					// v//vn
				}
				else if (sscanf(tokens[i].c_str(), "%d/%d", &v, &vt) == 2) {
					// v/vt
				}
				else if (sscanf(tokens[i].c_str(), "%d", &v) == 1) {
					// v
				}

				std::string face(std::to_string(v) + '/' + std::to_string(vt) + '/' + std::to_string(vn));

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

				//Add the vertices
				unsigned Indx;
				if (vertTable.find(face) == vertTable.end())
				{
					vertTable[face] = vertices.size();
					vertices.push_back(Vertex(vertPos, vertTexture, vertNormal));
				}
				Indx = vertTable[face];
				faceVertIndices[i] = Indx;
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
			if (currentMeshName.empty())
			{
				currentMeshName = line.substr(2);
				continue;
			}

			//if we read the prefix 'o' we create a new mesh with the vertices we have read.
			Mesh* mesh = createMesh(vertices, indices, currentMeshName, materialGroups);
			mesh->attachTo(object);

			currentMeshName = line.substr(2);

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
		else if (line.find("usemtl") != std::string::npos)
		{
			materialGroups.push_back(MaterialGroup());
			materialGroups.back().offset = indices.size();

			std::string name = line.substr(7);
			if (isMaterialInList(name))
			{
				materialGroups.back().material = getMaterial(name);
			}
			else
			{
				materialGroups.back().material = addMaterial(name);
			}
		}
	}

	objFile.close();
	//At the end add the final mesh with the last vertices and close the file
	Mesh* mesh = createMesh(vertices, indices, currentMeshName, materialGroups);
	mesh->attachTo(object);

	//Scene::activeScene->sceneObjects.push_back(object);
	Scene::activeScene->root.addChild(object);
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

inline bool FileManager::checkFileExistance(const std::string& name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
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
	unsigned texturesToAdd = 0;

	//Check if the paths are valid files before resizing the vector to avoid creating invalid textures
	for (size_t i = 0; i < texturesPaths.size(); i++)
	{
		if(checkFileExistance(texturesPaths[i]))
		{
			texturesToAdd++;
		}
		else
		{
			//Create a logging class that will handle messages
			std::cout << "Texture not found! : " << texturesPaths[i] << std::endl;
		}
	}

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
						commandVector.push_back([data, i, width, height, &textures, texturesSize, &texturesPaths]()
						{
							textures[texturesSize + i] = new Texture(data, width, height, texturesPaths[i].c_str());
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

	//create a shader, if the shader cannot be created print the throw message
	//the shader constructor adds it to the shader map
	try
	{
		Shader shader(shaderName, result[0], result[1]);
	}
	catch(std::exception error)
	{
		std::cout << error.what() << std::endl;
		return false;
	}
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

Mesh* FileManager::createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::string& name, const std::vector<MaterialGroup>& matGroups)
{
	Mesh* mesh = new Mesh(vertices, indices, name, matGroups);

	//Add debug lines to each vertex pointing the normal direction
	//currently disabling this feature as it may slow down object loading and it is usually done with geometric shader.
	/*for (size_t i = 0; i < vertices.size(); i++)
	{
		mesh->debugLinesContainer.pushLine(Line(
			Point(vertices[i].x, vertices[i].y, vertices[i].z),
			Point(vertices[i].x + vertices[i].nx * 0.002, vertices[i].y + vertices[i].ny * 0.002, vertices[i].z + vertices[i].nz * 0.002)));
	}*/

	return mesh;
}

unsigned FileManager::tokenizeOBJFaceLine(std::vector<std::string>& tokens, const std::string& line)
{
	std::istringstream iss(line);
	std::string word;
	iss >> word; // Skip the leading 'f'
	while (iss >> word) {
		tokens.push_back(word);
	}

	return tokens.size();
}
