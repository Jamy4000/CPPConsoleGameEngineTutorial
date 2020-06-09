#include <vector>

struct vector3D
{
	float x, y, z;
};

struct triangle
{
	vector3D p[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

struct matrix4x4
{
	float m[4][4] = { 0 };
};