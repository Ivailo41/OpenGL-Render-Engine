#pragma once
#include "BaseObject.h"
#include "Mesh.h"

class Object : public BaseObject
{
public:
	virtual void draw() const override;
	BaseObject* clone() const override { return new Object(*this); }

public:
	virtual ~Object() = default;

protected:


};


