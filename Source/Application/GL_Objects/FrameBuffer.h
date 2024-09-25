#pragma once
#include <GL/glew.h>
#include <vector>

class FrameBuffer
{
public:

	void genFrameBuffer(unsigned width, unsigned height);
	void bind() const;
	void unbind() const;
	void blitTo(const FrameBuffer& other, unsigned width, unsigned height);

	unsigned operator[](unsigned index) { return buffers[index]; }
	const unsigned operator[](unsigned index) const { return buffers[index]; }

	operator unsigned() const { return fbo; }

public:
	FrameBuffer(unsigned buffersCount, bool depth);
	~FrameBuffer();

private:
	void deleteFrameBuffer();

private:
	unsigned fbo;
	std::vector<unsigned> buffers; //can be fixed count
	unsigned rbo;
	unsigned buffersCount;

	unsigned width, height;
	bool hasDepth;
};