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
	SetPos(position.x, position.y, position.z);

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
	vertex[0] = -sx; vertex[1] = -sy; vertex[2] = sz;
	vertex[3] = sx; vertex[4] = -sy; vertex[5] = sz;
	vertex[6] = -sx; vertex[7] = sy; vertex[8] = sz;
	vertex[9] = sx; vertex[10] = sy; vertex[11] = sz;
	vertex[12] = -sx; vertex[13] = -sy; vertex[14] = sz;
	vertex[15] = sx; vertex[16] = -sy; vertex[17] = sz;
	vertex[18] = -sx; vertex[19] = sy; vertex[20] = sz;
	vertex[21] = sx; vertex[22] = sy; vertex[23] = sz;

	glGenBuffers(1, (GLuint*) &vertexId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLubyte index[36]{
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
	delete[] vertex;
}

void Cube::InnerRender() const
{	
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
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
plane::plane() : Primitive()
{
	type = PrimitiveTypes::Primitive_Plane;
}

plane::plane(float x, float y, float z, float d) : Primitive()
{
	type = PrimitiveTypes::Primitive_Plane;
	float3 pos(x, y, z);
	mathGeoLibPlane = new Plane(pos, d);
}

void plane::InnerRender() const
{
	glLineWidth(1.0f);
	
	if (grid) {
		glBegin(GL_LINES);
	}
	else {
		glBegin(GL_TRIANGLE_STRIP);
	}

	float d = 200.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}