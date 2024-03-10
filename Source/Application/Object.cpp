#include "Object.h"

Object::Object(const std::string& name) : BaseObject(name)
{
	//nothing to do here
}

void Object::draw() const
{
	BaseObject::draw();
	/*unsigned meshesCount = children.getSize();
	for (size_t i = 0; i < meshesCount; i++)
	{
		children[i].draw();
	}*/
}

