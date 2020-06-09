#include <iostream>
using namespace std;

#include "olcConsoleGameEngine.h"
#include "dataStructure3D.cpp"

class engine3D : public olcConsoleGameEngine 
{
private:
	mesh meshCube;
	matrix4x4 matrixProjection;
	float fTheta;

	void MultiplyMatrixVector(vector3D &inputVector, vector3D &outputVector, matrix4x4 &projectionMatrix)
	{
		outputVector.x = inputVector.x * projectionMatrix.m[0][0] + inputVector.y * projectionMatrix.m[1][0] + inputVector.z * projectionMatrix.m[2][0] + projectionMatrix.m[3][0];
		outputVector.y = inputVector.x * projectionMatrix.m[0][1] + inputVector.y * projectionMatrix.m[1][1] + inputVector.z * projectionMatrix.m[2][1] + projectionMatrix.m[3][1];
		outputVector.z = inputVector.x * projectionMatrix.m[0][2] + inputVector.y * projectionMatrix.m[1][2] + inputVector.z * projectionMatrix.m[2][2] + projectionMatrix.m[3][2];
		float w = inputVector.x * projectionMatrix.m[0][3] + inputVector.y * projectionMatrix.m[1][3] + inputVector.z * projectionMatrix.m[2][3] + projectionMatrix.m[3][3];

		if (w != 0.0f) 
		{
			outputVector.x /= w;
			outputVector.y /= w;
			outputVector.z /= w;
		}
	}

public:
	engine3D() 
	{
		m_sAppName = L"3D Engine Tutorial";
	}

	virtual bool OnUserCreate() override
	{
		meshCube.tris = 
		{

			// FRONT
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// RIGHT                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// BACK                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// LEFT                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

		// Projection Matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		matrixProjection.m[0][0] = fAspectRatio * fFovRad;
		matrixProjection.m[1][1] = fFovRad;
		matrixProjection.m[2][2] = fFar / (fFar - fNear);
		matrixProjection.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrixProjection.m[2][3] = 1.0f;
		matrixProjection.m[3][3] = 0.0f;

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) override
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		// Rotate the cube on the X and Z axis
		matrix4x4 matRotX, matRotZ;
		fTheta += 1.0f * fElapsedTime;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);;
		matRotX.m[3][3] = 1;

		// Draw Triangles
		for (auto tri : meshCube.tris) 
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

			// Rotate in Z-Axis
			MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			// Rotate in X-Axis
			MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			// Offset into the screen
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

			// Project triangles from 3D --> 2D
			MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matrixProjection);
			MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matrixProjection);
			MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matrixProjection);

			// Scale into view
			triProjected.p[0].x += 1.0f;
			triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f;
			triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f;
			triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
			triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
			triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
			triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

			// Rasterize triangle
			DrawTriangle
			(
				triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				PIXEL_SOLID, FG_WHITE
			);
		}

		return true;
	}
};

int main() 
{
	engine3D engine;

	if (engine.ConstructConsole(256, 240, 2, 2)) 
	{
		std::cout << "YES.";
		engine.Start();
	}
	else 
	{
		std::cout << "Error has occured while created the engine console.";
	}

	return 0;
}