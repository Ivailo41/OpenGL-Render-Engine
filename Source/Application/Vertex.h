#pragma once
#include <iostream>

struct Vertex
{
	float x;
	float y;
	float z;

	float u;
	float v;

	float nx;
	float ny;
	float nz;

	float tx;
	float ty;
	float tz;

	Vertex() : x(0), y(0), z(0), u(0), v(0), nx(0), ny(0), nz(0) {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz) : x(x), y(y), z(z), u(u), v(v), nx(nx), ny(ny), nz(nz), tx(tx), ty(ty), tz(tz) {}
	friend std::ostream& operator<<(std::ostream& stream, const Vertex& vertex)
	{
		stream << vertex.x << ' ' << vertex.y << ' ' << vertex.z << ' ' << vertex.u << ' ' << vertex.v << ' ' << vertex.nx << ' ' << vertex.ny << ' ' << vertex.nz;
		return stream;
	}
};