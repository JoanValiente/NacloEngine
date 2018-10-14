#include "OpenGL.h"
#include "Globals.h"
#include "Primitive.h"

#define PI 3.141592
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
		AxisRender();
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

void Primitive::AxisRender() const
{
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
Cube::Cube(float3 position, float3 size) : Primitive(), size(size), position(position)
{
	type = PrimitiveTypes::Primitive_Cube;

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

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

void Cube::AxisRender() const
{
	glBegin(GL_LINES);

	glLineWidth(1.f);

	// Y axis
	glColor3f(0.f, 255.f, 0.f);
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(position.x, position.y + size.y, position.z);
	glVertex3f(position.x - 0.05f, position.y + size.y, position.z); glVertex3f(position.x, position.y + size.y + 0.15f, position.z);
	glVertex3f(position.x + 0.05f, position.y + size.y, position.z); glVertex3f(position.x, position.y + size.y + 0.15f, position.z);

	// X axis
	glColor3f(255.f, 0.f, 0.f);
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(position.x + size.x, position.y, position.z);
	glVertex3f(position.x + size.x, position.y - 0.05f, position.z); glVertex3f(position.x + size.x + 0.15f, position.y, position.z);
	glVertex3f(position.x + size.x, position.y + 0.05f, position.z); glVertex3f(position.x + size.x + 0.15f, position.y, position.z);

	// Z axis
	glColor3f(0.f, 0.f, 255.f);
	glVertex3f(position.x, position.y, position.z);
	glVertex3f(position.x, position.y, position.z + size.z);
	glVertex3f(position.x, position.y - 0.05f, position.z + size.z); glVertex3f(position.x, position.y, position.z + size.z + 0.15f);
	glVertex3f(position.x, position.y + 0.05f, position.z + size.z); glVertex3f(position.x, position.y, position.z + size.z + 0.15f);

	glColor3f(1.f, 1.f, 1.f);

	glEnd();
}

/*

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

*/

// PLANE ==================================================
plane::plane(float3 position, float3 size) : Primitive(), size(size), position(position)
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
	if (grid) 
	{
		glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, NULL);
	}
	else 
	{
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);	
}

void plane::AxisRender() const
{
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

//RAY
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

//ARROW
Arrow::Arrow(float3 startPos, float3 endPos) : Primitive()
{
	float vertex[18] = {
		startPos.x, startPos.y, startPos.z,
		endPos.x, endPos.y, endPos.z,
		endPos.x + 1.0f, endPos.y - 2.0f, endPos.z,
		endPos.x, endPos.y, endPos.z,
		endPos.x, endPos.y, endPos.z,
		endPos.x - 1.0f, endPos.y - 2.0f, endPos.z,
	};
	
	glGenBuffers(1, (GLuint*)&vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Arrow::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_LINES, 0, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

//SPHERE
sphere::sphere(float3 pos, float radius, uint rings, uint sectors)
{
	float const R = 1.0f / (float)(rings - 1);
	float const S = 1.0f / (float)(sectors - 1);
	int r, s;

	vertexId.resize(rings * sectors * 3);

	std::vector<GLfloat>::iterator v = vertexId.begin();
	
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(-PI / 2 + PI * r * R);
			float const x = cos(2 * PI * s * S) * sin(PI * r * R);
			float const z = sin(2 * PI * s * S) * sin(PI * r * R);

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;
		}
	}
	

	indexId.resize(rings * sectors * 4);
	
	std::vector<GLushort>::iterator i = indexId.begin();
	for (r = 0; r < rings; r++) 
	{
		for (s = 0; s < sectors; s++) 
		{
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}
	}
	indexId.resize((rings * (sectors - 1) * 4 ) - 4);
}

sphere::~sphere()
{
}

void sphere::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertexId[0]);
	glDrawElements(GL_QUADS, indexId.size(), GL_UNSIGNED_SHORT, &indexId[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//CYLYNDER

cylinder::cylinder(float3 pos, float radius, float height, uint rings, uint sectors)
{
	float const R = 1.0f / (float)(rings - 1);
	float const S = 1.0f / (float)(sectors - 1);
	int r, s;

	vertexId.resize(rings * sectors * 4);

	std::vector<GLfloat>::iterator v = vertexId.begin();

	// Cylinder Top
	/*
	for (s = 0; s < sectors; s++)
	{
		float const y = 0;
		float const x = cos(2 * PI * s * S);
		float const z = sin(2 * PI * s * S);
		*v++ = z * radius;
		*v++ = y;
		*v++ = x * radius;
		
	}
	
	
	for (int i = 0; i < 200; i += 3) {

		vertexId[i] += pos.x;
		vertexId[i + 1] += pos.y + height;
		vertexId[i + 2] += pos.z;
	}	
	*/
	// Cylinder "Cover"
	
	//v = vertexId.end();

	for (r = 1; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const y = sin(PI * r * R);
			float const x = cos(2 * PI * s * S);
			float const z = sin(2 * PI * s * S);

			*v++ = x * radius;
			*v++ = y;
			*v++ = z * radius;
		}
	}

	vertexId.resize((sectors - 1) * 2 * 4);
	
	indexId.resize(rings * sectors * 4);

	std::vector<GLushort>::iterator i = indexId.begin();
	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}
	}

	indexId.resize((rings * rings + (sectors - 1) * 2 + 1) * 4);

}

cylinder::~cylinder()
{
}

void cylinder::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertexId[0]);
	glDrawElements(GL_QUADS, indexId.size(), GL_UNSIGNED_SHORT, &indexId[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
