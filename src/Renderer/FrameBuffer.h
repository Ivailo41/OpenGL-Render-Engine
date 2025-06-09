#pragma once
#include <GL/glew.h>
#include <vector>
#include "Cubemap.h"

class FrameBuffer
{
public:

	void genFrameBuffer(unsigned width, unsigned height);
	void bind() const;
	void unbind() const;
	void blitTo(const FrameBuffer& other, unsigned width, unsigned height);

	unsigned operator[](unsigned index) { return buffers[index]; }
	const unsigned operator[](unsigned index) const { return buffers[index]; }

	operator unsigned() const { return frameBufferID; }

public:
	FrameBuffer(unsigned buffersCount, bool depth);
	~FrameBuffer();

protected:
	void deleteFrameBuffer();

protected:
	unsigned frameBufferID;
	std::vector<unsigned> buffers; //can be fixed count
	unsigned depthBufferID;
	unsigned buffersCount;

	unsigned width, height;
	bool hasDepth;
};

class ShadowFrameBuffer : public FrameBuffer
{
public:
	void genFrameBuffer(unsigned width, unsigned heigth);

	ShadowFrameBuffer();
	~ShadowFrameBuffer();

	Cubemap cubemapTexture;
};