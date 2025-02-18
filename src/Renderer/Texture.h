#pragma once
#include <string>
#include <vector>
#include <iostream>

class Texture
{
public:
	unsigned id;
	std::string type; // can be enum if needed later on for something
	std::string path; //might be usefull to open the texture location through the software or reloading the texture

	//Casting the texture to unsigned will return the id, makes it easier to read instead in some functions
	operator unsigned() const;

	friend class FileManager;

private:
	//Constructors will be called only from the FileManager class
	Texture();
	Texture(unsigned id, const char* path);

};