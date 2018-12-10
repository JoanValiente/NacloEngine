#pragma once
#include "Color.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/MathGeoLib.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render(uint textureid = 0) const;
	virtual void	InnerRender(uint textureid = 0) const;
	virtual void	AxisRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube(float3 pos = float3(0.0f,0.0f,0.0f), float3 size = float3(1.0f, 1.0f, 1.0f));
	~Cube();
	void InnerRender() const;
	void AxisRender() const;
public:
	float3 position;
	float3 size;

private:
	uint indexId = 0;
	uint vertexId = 0;

	//GLsizei indexSize = 0;
};

// ============================================

class plane : public Primitive
{
public:
	plane(float3 pos = float3(0.0f, 0.0f, 0.0f), float3 size = float3(200.0f, 0.0f, 200.0f));
	void InnerRender(uint textureid = 0) const;
	void AxisRender() const;
public:
	bool grid = false;
	float3 position;
	float3 size;

	uint indexId = 0;
	uint vertexId = 0;
	uint textureId = 0;
};

class grid : public Primitive
{
public:
	grid(float x, float y, float z, float d);
	void InnerRender() const;
	void AxisRender() const;

public:
	Plane* Grid;

};

class ray : public Primitive
{
public:
	ray(float3 startPos = float3(0.0f, 0.0f, 0.0f), float3 endPos = float3(0.0f, 0.0f, 0.0f));
	void InnerRender() const;

private:
	uint vertexId = 0;
};

class Arrow : public Primitive
{
public:
	Arrow(float3 startPos = float3(0.0f, 0.0f, 0.0f), float3 endPos = float3(0.0f, 0.0f, 0.0f));
	void InnerRender() const;

private:
	uint vertexId = 0;
};

class sphere : public Primitive
{
public:
	sphere(float3 pos = float3(0.0f, 0.0f, 0.0f), float radius = 1, uint rings = 8, uint sectors = 8);
	~sphere();
	void InnerRender() const;

public:
	float3 position;
	float radius;

private:
	std::vector<GLfloat> vertexId;
	std::vector<GLushort> indexId;

	//GLsizei indexSize = 0;
};

class cylinder : public Primitive
{
public: 
	cylinder(float3 pos = float3(0.0f, 0.0f, 0.0f), float radius = 1, float height = 1, uint rings = 4U, uint sectors = 16U);
	virtual ~cylinder();
	void InnerRender() const;

public:
	float3 position;
	float radius;
	float height;

private: 
	std::vector<GLfloat> vertexId;
	std::vector<GLushort> indexId;
};

/*
// ============================================
class cylinder : public Primitive
{
public:
	cylinder();
	cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class line : public Primitive
{
public:
	line();
	line(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};
*/