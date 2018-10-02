#include "OpenGL.h"
#include "Globals.h"
#include "Primitive.h"

// ------------------------------------------------------------
Primitive::Primitive() : transform(float4x4::identity), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}
// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.ptr());

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.Translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const float3 &u)
{
	transform.RotateAxisAngle(u, angle);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.Scale(x, y, z);
}

// CUBE ============================================
Cube::Cube(float3 position, float3 size) : Primitive(), size(size)
{
	type = PrimitiveTypes::Primitive_Cube;

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;
	/*
glBegin(GL_QUADS);

glNormal3f(0.0f, 0.0f, 1.0f);
glVertex3f(-sx, -sy, sz);
glVertex3f(sx, -sy, sz);
glVertex3f(sx, sy, sz);
glVertex3f(-sx, sy, sz);

glNormal3f(0.0f, 0.0f, -1.0f);
glVertex3f(sx, -sy, -sz);
glVertex3f(-sx, -sy, -sz);
glVertex3f(-sx, sy, -sz);
glVertex3f(sx, sy, -sz);

glNormal3f(1.0f, 0.0f, 0.0f);
glVertex3f(sx, -sy, sz);
glVertex3f(sx, -sy, -sz);
glVertex3f(sx, sy, -sz);
glVertex3f(sx, sy, sz);

glNormal3f(-1.0f, 0.0f, 0.0f);
glVertex3f(-sx, -sy, -sz);
glVertex3f(-sx, -sy, sz);
glVertex3f(-sx, sy, sz);
glVertex3f(-sx, sy, -sz);

glNormal3f(0.0f, 1.0f, 0.0f);
glVertex3f(-sx, sy, sz);
glVertex3f(sx, sy, sz);
glVertex3f(sx, sy, -sz);
glVertex3f(-sx, sy, -sz);

glNormal3f(0.0f, -1.0f, 0.0f);
glVertex3f(-sx, -sy, -sz);
glVertex3f(sx, -sy, -sz);
glVertex3f(sx, -sy, sz);
glVertex3f(-sx, -sy, sz);

glEnd();
*/

	float vertex[24] = {
		-sx, -sy,  sz,
		 sx, -sy,  sz,
		-sx,  sy,  sz,
		 sx,  sy,  sz,
		-sx, -sy, -sz,
		 sx, -sy, -sz,
		-sx,  sy, -sz,
		 sx,  sy, -sz
	};

	for (int i = 0; i < 24; i += 3) {

		vertex[i] += position.x;
		vertex[i + 1] += position.y;
		vertex[i + 2] += position.z;
	}

	glGenBuffers(1, (GLuint*) &vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint index[36]{
		// Front
		0, 1, 2,
		1, 3, 2,
		// Right
		1, 5, 3,
		5, 7, 3,
		// Back
		5, 4, 7,
		4, 6, 7,
		// Left
		4, 0, 6,
		0, 2, 6,
		// Top
		2, 3, 6,
		3, 7, 6,
		// Bottom
		0, 4, 1,
		1, 4, 5 
	};

	//indexSize = sizeof(index) / sizeof(GLubyte);

	glGenBuffers(1, (GLuint*) &indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 36, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Cube::~Cube()
{
}

void Cube::InnerRender() const
{	
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

/*
// SPHERE ============================================
sphere::sphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

sphere::sphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void sphere::InnerRender() const
{
	//glutSolidSphere(radius, 25, 25);
}


// CYLINDER ============================================
cylinder::cylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

cylinder::cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void cylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);

	for (int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * pi / 180; // degrees to radians
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * pi / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 480; i += (360 / n))
	{
		float a = i * pi / 180; // degrees to radians

		glVertex3f(height*0.5f, radius * cos(a), radius * sin(a));
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();
}

// LINE ==================================================
line::line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

line::line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void line::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}
*/

// PLANE ==================================================
plane::plane(float3 position, float3 size) : Primitive(), size(size)
{
	type = PrimitiveTypes::Primitive_Plane;
	float vertex[12] = {
		 size.x, position.y, -size.z,
		 size.x, position.y,  size.z,
		-size.x, position.y, -size.z,
		-size.x, position.y,  size.z 
	};

	for (int i = 0; i < 24; i += 3) {

		vertex[i] += position.x;
		vertex[i + 1] += position.y;
		vertex[i + 2] += position.z;
	}

	glGenBuffers(1, (GLuint*)&vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint index[6]{
		2, 1, 0,
		3, 1, 2 
	};

	glGenBuffers(1, (GLuint*)&indexId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 6, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void plane::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	if (grid) {
		glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, NULL);
	}
	else {
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);	
}

ray::ray(float3 startPos, float3 endPos) : Primitive()
{
	float vertex[6] = {
		 startPos.x, startPos.y, startPos.z,
		 endPos.x, endPos.y, endPos.z
	};


	glGenBuffers(1, (GLuint*)&vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ray::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_LINES, 0, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

}