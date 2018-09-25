#pragma once
#include "Color.h"
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

	virtual void	Render() const;
	virtual void	InnerRender() const;
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
class cube : public Primitive
{
public :
	cube();
	cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float3 size;
};

// ============================================
class sphere : public Primitive
{
public:
	sphere();
	sphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

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

// ============================================
class plane : public Primitive
{
public:
	plane();
	plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	bool grid = false;
	Plane* mathGeoLibPlane;
};
