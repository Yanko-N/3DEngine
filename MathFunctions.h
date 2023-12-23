#pragma once
#include <iostream>
#include <strstream>
#include <vector>
#include <fstream>


constexpr auto Pi = 3.14159f;
/// <summary>
/// Estrutura que guarda um Vetor Tridimensional
/// </summary>
struct  vec3d
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

/// <summary>
/// Um tringulo é feito a partir de 3 pontos
/// </summary>
struct triangle {
	vec3d p[3];

	wchar_t sym; //simbolo
	short col; 
};

/// <summary>
/// Uma Mesh é feita a partir de varios triangulos
/// </summary>
struct mesh
{
	std::vector<triangle> tris;
	bool LoadFromObjectFile(std::string sFileName) {
		std::ifstream f(sFileName);
		if (!f.is_open()) {
			return false;
		}

		//local cache de vertices
		std::vector<vec3d> vertices;

		//read to end of file
		while (!f.eof()) {
			char line[128]; //basicamente crio uma array de chars
			f.getline(line, 128); //coloco na array a linha da stream/ficheiro

			std::strstream s;
			s << line;
			char junk;
			if (line[0] == 'v') { //Aqui irei ver se é um vertice
				vec3d v;
				s >> junk >> v.x >> v.y >> v.z;
				vertices.push_back(v);
			}

			if (line[0] == 'f') {
				int faceIndex[3];
				s >> junk >> faceIndex[0] >> faceIndex[1] >> faceIndex[2];
				//- 1 porque no ficheiro os indices começam em 1 e não em 0
				tris.push_back({ vertices[faceIndex[0] - 1],vertices[faceIndex[1] - 1], vertices[faceIndex[2] - 1] });
			}
		}
		return true;
	}
};


/// <summary>
/// Matrix 4 por 4
/// ela é linha - coluna
/// </summary>
struct mat4x4 {
	float m[4][4] = { 0 };
};

static class MathFunctions
{

public : 
	static void MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m);
	static void NormalizeVectorByReference(vec3d& vector);
	static mat4x4 Matrix_MakeIdentity();
	static mat4x4 Matrix_MakeRotationX(float fAngleRad);
	static mat4x4 Matrix_MakeRotationY(float fAngleRad);
	static mat4x4 Matrix_MakeRotationZ(float fAngleRad);
	static mat4x4 Matrix_MakeTranslation(float x, float y, float z);
	static mat4x4 Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
	static mat4x4 Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2);
	static vec3d Matrix_MultiplyVector(mat4x4& m, vec3d& i);
	static mat4x4 Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up);
	static mat4x4 Matrix_QuickInverse(mat4x4& m);
	static vec3d Vector_Add(vec3d& v1, vec3d& v2);
	static vec3d Vector_Sub(vec3d& v1, vec3d& v2);
	static vec3d Vector_Mul(vec3d& v1, float k);
	static vec3d Vector_Div(vec3d& v1, float k);
	static float Vector_DotProduct(vec3d& v1, vec3d& v2);
	static float Vector_Length(vec3d& v);
	static vec3d Vector_Normalise(vec3d& v);
	static vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2);
	static vec3d Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd);
};

