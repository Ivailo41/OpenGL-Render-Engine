#pragma once
#include <iostream>
#include <glm.hpp>

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

    Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& norm) : x(pos.x), y(pos.y), z(pos.z), u(tex.x), v(tex.y), nx(norm.x), ny(norm.y), nz(norm.z)
    {
        tx = 1;
        ty = 0;
        tz = 0;
    }

	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz) : x(x), y(y), z(z), u(u), v(v), nx(nx), ny(ny), nz(nz), tx(tx), ty(ty), tz(tz) {}
	friend std::ostream& operator<<(std::ostream& stream, const Vertex& vertex)
	{
		stream << vertex.x << ' ' << vertex.y << ' ' << vertex.z << ' ' << vertex.u << ' ' << vertex.v << ' ' << vertex.nx << ' ' << vertex.ny << ' ' << vertex.nz;
		return stream;
	}

    const glm::vec3 getPos() const { return glm::vec3(x, y, z); }
    const glm::vec3 getNormal() const { return glm::vec3(nx, ny, nz); }

    void setNormal(const glm::vec3& normals) 
    {
        nx = normals.x;
        ny = normals.y;
        nz = normals.z;
    }
};

struct Point
{
	float x, y, z;
	Point() : x(0), y(0), z(0){}
	Point(float x, float y, float z) : x(x), y(y), z(z){}
};

struct Line
{
	Point start;
	Point end;

	Line(const Point& start, const Point& end) : start(start), end(end) {}
};

struct Color
{
	float r, g, b;
	Color() : r(0), g(0), b(0){}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}

	bool operator==(const Color& other) 
	{
		return r == other.r && g == other.g && b == other.b;
	}
};

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    //glm::mat4 modelMatrix;

    Transform() : position(0), rotation(0), scale(1)
    {
        //nothing to do here
    }
};

namespace PRIMITIVE_SHAPES
{
    static float cubeVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
}