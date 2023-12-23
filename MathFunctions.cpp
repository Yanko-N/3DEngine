#include "MathFunctions.h"
#include <iostream>

	/// <summary>
	/// Esta função realiza a multiplicação de um vetor  (i) por uma matriz 4x4 (m),
	/// armazenando o resultado no vetor de saída (o). Além disso, realiza a divisão perspetiva,
	/// se necessário, para normalizar o vetor resultante.
	/// </summary>
	/// <param name="i"></param>
	/// <param name="o"></param>
	/// <param name="m"></param>
void MathFunctions::MultiplyMatrixVector(vec3d& i, vec3d& o, mat4x4& m) {

	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}
}

/// <summary>
/// Esta função vai receber por referencia um vetor e irá normalizar-lo
/// </summary>
/// <param name="vector"></param>
void MathFunctions::NormalizeVectorByReference(vec3d& vector) {
	float length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector.x /= length;			vector.y /= length;			vector.z /= length; //Normalizar o vetor
}

/// <summary>
/// Devolve uma matriz Identidade
/// </summary>
/// <returns></returns>
mat4x4 MathFunctions::Matrix_MakeIdentity()
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

/// <summary>
/// Roda um matrix no eixo X
/// </summary>
/// <param name="fAngleRad"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_MakeRotationX(float fAngleRad)
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = cosf(fAngleRad);
	matrix.m[1][2] = sinf(fAngleRad);
	matrix.m[2][1] = -sinf(fAngleRad);
	matrix.m[2][2] = cosf(fAngleRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

/// <summary>
/// Roda uma MAtrix no eixo Y
/// </summary>
/// <param name="fAngleRad"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_MakeRotationY(float fAngleRad)
{
	mat4x4 matrix;
	matrix.m[0][0] = cosf(fAngleRad);
	matrix.m[0][2] = sinf(fAngleRad);
	matrix.m[2][0] = -sinf(fAngleRad);
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = cosf(fAngleRad);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

/// <summary>
/// Roda uma Matrix no eixo Z
/// </summary>
/// <param name="fAngleRad"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_MakeRotationZ(float fAngleRad)
{
	mat4x4 matrix;
	matrix.m[0][0] = cosf(fAngleRad);
	matrix.m[0][1] = sinf(fAngleRad);
	matrix.m[1][0] = -sinf(fAngleRad);
	matrix.m[1][1] = cosf(fAngleRad);
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

/// <summary>
/// Mexe uma matrix nos eixos X;Y;Z pela qauntidade x,y,z nos especificos eixos
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_MakeTranslation(float x, float y, float z)
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.0f;
	matrix.m[1][1] = 1.0f;
	matrix.m[2][2] = 1.0f;
	matrix.m[3][3] = 1.0f;
	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;
	return matrix;
}

/// <summary>
/// Devolve a matrix de Projeção
/// </summary>
/// <param name="fFovDegrees"></param>
/// <param name="fAspectRatio"></param>
/// <param name="fNear"></param>
/// <param name="fFar"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
	float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
	mat4x4 matrix;
	matrix.m[0][0] = fAspectRatio * fFovRad;
	matrix.m[1][1] = fFovRad;
	matrix.m[2][2] = fFar / (fFar - fNear);
	matrix.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matrix.m[2][3] = 1.0f;
	matrix.m[3][3] = 0.0f;
	return matrix;
}

/// <summary>
/// Multiplica uma matriz 4x4 por uma matriz 4x4
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_MultiplyMatrix(mat4x4& m1, mat4x4& m2)
{
	mat4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}

/// <summary>
/// Multiplica uma matrix por um vetor
/// </summary>
/// <param name="m"></param>
/// <param name="i"></param>
/// <returns></returns>
vec3d MathFunctions::Matrix_MultiplyVector(mat4x4& m, vec3d& i)
{
	vec3d v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}


/// <summary>
/// Devolve uma matrix apontada para uma target Position
/// </summary>
/// <param name="pos"></param>
/// <param name="target"></param>
/// <param name="up"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_PointAt(vec3d& pos, vec3d& target, vec3d& up)
{
	// Calculate new forward direction
	vec3d newForward = Vector_Sub(target, pos);
	newForward = Vector_Normalise(newForward);

	// Calculate new Up direction
	vec3d a = Vector_Mul(newForward, Vector_DotProduct(up, newForward));
	vec3d newUp = Vector_Sub(up, a);
	newUp = Vector_Normalise(newUp);

	// New Right direction is easy, its just cross product
	vec3d newRight = Vector_CrossProduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix	
	mat4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}


/// <summary>
/// Devolve a matriz inversa
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
mat4x4  MathFunctions::Matrix_QuickInverse(mat4x4& m) // Only for Rotation/Translation Matrices
{
	mat4x4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

/// <summary>
/// Adiciona vetores
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_Add(vec3d& v1, vec3d& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/// <summary>
/// Subtrai Vetores
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_Sub(vec3d& v1, vec3d& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}


/// <summary>
/// Multiplica vetores
/// </summary>
/// <param name="v1"></param>
/// <param name="k"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_Mul(vec3d& v1, float k)
{
	return { v1.x * k, v1.y * k, v1.z * k };
}


/// <summary>
/// Divide Vetores
/// </summary>
/// <param name="v1"></param>
/// <param name="k"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_Div(vec3d& v1, float k)
{
	return { v1.x / k, v1.y / k, v1.z / k };
}


/// <summary>
/// PRoduto Escadar entre dois vetores
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float  MathFunctions::Vector_DotProduct(vec3d& v1, vec3d& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// <summary>
/// Devolve a distancia do vetor
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float  MathFunctions::Vector_Length(vec3d& v)
{
	return sqrtf(MathFunctions::Vector_DotProduct(v, v));
}

/// <summary>
/// devolve o vetor normalizado
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_Normalise(vec3d& v)
{
	float l = MathFunctions::Vector_Length(v);
	return { v.x / l, v.y / l, v.z / l };
}


/// <summary>
/// devolve um vetor com o produto cruzado entre dois vetores
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_CrossProduct(vec3d& v1, vec3d& v2)
{
	vec3d v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}


/// <summary>
/// Devolve o Vetor normal a interção de um plano
/// </summary>
/// <param name="plane_p"></param>
/// <param name="plane_n"></param>
/// <param name="lineStart"></param>
/// <param name="lineEnd"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
{
	plane_n = Vector_Normalise(plane_n);
	float plane_d = -Vector_DotProduct(plane_n, plane_p);
	float ad = Vector_DotProduct(lineStart, plane_n);
	float bd = Vector_DotProduct(lineEnd, plane_n);
	float t = (-plane_d - ad) / (bd - ad);
	vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
	vec3d lineToIntersect =Vector_Mul(lineStartToEnd, t);
	return  Vector_Add(lineStart, lineToIntersect);
}