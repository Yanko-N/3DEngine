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
/// Serve para projetar vetores 
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
/// Esta Função devolve-nos o ponto que esta a intercetar o plano(plane_p), o plano normal(plane_n) e pegamos o ponto inicial e final do vetor que ira cruzar esse plano
/// </summary>
/// <param name="plane_p"></param>
/// <param name="plane_n"></param>
/// <param name="lineStart"></param>
/// <param name="lineEnd"></param>
/// <returns></returns>
vec3d  MathFunctions::Vector_IntersectPlane(vec3d& plane_p, vec3d& plane_n, vec3d& lineStart, vec3d& lineEnd)
{
	//normaliza o plano 
	plane_n = Vector_Normalise(plane_n);

	//calcula a distancia entre planos
	float plane_d = -Vector_DotProduct(plane_n, plane_p);

	//Matematica relacionada para o calculo do termo
	float ad = Vector_DotProduct(lineStart, plane_n);
	float bd = Vector_DotProduct(lineEnd, plane_n);

	//Calcula o termos a qual iremos escalar o ponto á
	float t = (-plane_d - ad) / (bd - ad);
	vec3d lineStartToEnd = Vector_Sub(lineEnd, lineStart);
	vec3d lineToIntersect =Vector_Mul(lineStartToEnd, t);
	return  Vector_Add(lineStart, lineToIntersect);
}

/// <summary>
/// Recorta triangulos que estejam no plano
/// </summary>
/// <param name="plane_p"></param>
/// <param name="plane_n"></param>
/// <param name="in_tri"></param>
/// <param name="out_tri1"></param>
/// <param name="out_tri2"></param>
/// <returns></returns>
int MathFunctions::Triangle_ClipAgainstPlain(vec3d plane_p, vec3d plane_n, triangle& in_tri, triangle& out_tri1, triangle& out_tri2) {
	

	// Make sure plane normal is indeed normal
	plane_n = Vector_Normalise(plane_n);

	// Return signed shortest distance from point to plane, plane normal must be normalised
	auto dist = [&](vec3d& p)
	{
		vec3d n = Vector_Normalise(p);
		return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - Vector_DotProduct(plane_n, plane_p));
	};

	// Create two temporary storage arrays to classify points either side of plane
	// If distance sign is positive, point lies on "inside" of plane
	vec3d* inside_points[3];  int nInsidePointCount = 0;
	vec3d* outside_points[3]; int nOutsidePointCount = 0;

	// Get signed distance of each point in triangle to plane
	float d0 = dist(in_tri.p[0]);
	float d1 = dist(in_tri.p[1]);
	float d2 = dist(in_tri.p[2]);

	if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
	if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
	if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
	else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

	// Now classify triangle points, and break the input triangle into 
	// smaller output triangles if required. There are four possible
	// outcomes...

	if (nInsidePointCount == 0)
	{
		// All points lie on the outside of plane, so clip whole triangle
		// It ceases to exist

		return 0; // No returned triangles are valid
	}

	if (nInsidePointCount == 3)
	{
		// All points lie on the inside of plane, so do nothing
		// and allow the triangle to simply pass through
		out_tri1 = in_tri;

		return 1; // Just the one returned original triangle is valid
	}

	if (nInsidePointCount == 1 && nOutsidePointCount == 2)
	{
		// Triangle should be clipped. As two points lie outside
		// the plane, the triangle simply becomes a smaller triangle

		// Copy appearance info to new triangle
		out_tri1.col = in_tri.col;
		out_tri1.sym = in_tri.sym;

		// The inside point is valid, so keep that...
		out_tri1.p[0] = *inside_points[0];

		// but the two new points are at the locations where the 
		// original sides of the triangle (lines) intersect with the plane
		out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
		out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

		return 1; // Return the newly formed single triangle
	}

	if (nInsidePointCount == 2 && nOutsidePointCount == 1)
	{
		// Triangle should be clipped. As two points lie inside the plane,
		// the clipped triangle becomes a "quad". Fortunately, we can
		// represent a quad with two new triangles

		// Copy appearance info to new triangles
		out_tri1.col = in_tri.col;
		out_tri1.sym = in_tri.sym;

		out_tri2.col = in_tri.col;
		out_tri2.sym = in_tri.sym;

		// The first triangle consists of the two inside points and a new
		// point determined by the location where one side of the triangle
		// intersects with the plane
		out_tri1.p[0] = *inside_points[0];
		out_tri1.p[1] = *inside_points[1];
		out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

		// The second triangle is composed of one of he inside points, a
		// new point determined by the intersection of the other side of the 
		// triangle and the plane, and the newly created point above
		out_tri2.p[0] = *inside_points[1];
		out_tri2.p[1] = out_tri1.p[2];
		out_tri2.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

		return 2; // Return two newly formed triangles which form a quad
	}


	////Vamos ter a certeza que o plano é normalizado
	//plane_n = Vector_Normalise(plane_n);

	////Pequena função que calcula a distancia dos pontos
	////Isto é uma lambda function
	//auto dist = [&](vec3d& p) {
	//	vec3d n = Vector_Normalise(p);
	//	return(plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z * Vector_DotProduct(plane_n, plane_p));
	//};

	////Cria duas arrays temporarias que iram classificar os pontos se estão dentro ou fora do plano
	////se for positivo os pontos estão dentro do plano

	//vec3d* inside_Points[3];
	//int nInsidePoints = 0;

	//vec3d* outside_Points[3];
	//int nOutsidePoints = 0;

	////distancias
	//float dist0 = dist(in_tri.p[0]);
	//float dist1 = dist(in_tri.p[1]);
	//float dist2 = dist(in_tri.p[2]);
	////distancia 0
	//if (dist0 >= 0){

	//	//index é 0 e apos é incrementado
	//	//Guardo os endereços nos apontadores

	//	inside_Points[nInsidePoints++] = &in_tri.p[0];
	//}
	//else {
	//	outside_Points[nOutsidePoints++]= &in_tri.p[0];
	//}
	////distancia 1
	//if (dist1 >= 0) {
	//	//index é 0 e apos é incrementado
	//	//Guardo os endereços nos apontadores
	//	inside_Points[nInsidePoints++] = &in_tri.p[1];
	//}
	//else {
	//	outside_Points[nOutsidePoints++] = &in_tri.p[1];
	//}
	////distancia 2
	//if (dist2 >= 0) {
	//	//index é 0 e apos é incrementado
	//	//Guardo os endereços nos apontadores

	//	inside_Points[nInsidePoints++] = &in_tri.p[2];
	//}
	//else {
	//	outside_Points[nOutsidePoints++] = &in_tri.p[2];
	//}



	////Agora vamos classificar os pontos dos triangulos
	////Existe 4 resultados possiveis

	//if (nInsidePoints == 0) {
	//	//Todos os pontos estão fora do plano então são todos recortados
	//	return 0; //Não existe triangulos validos
	//}
	//if (nInsidePoints == 3) {
	//	//Todos os pontos estão dentro do plano então não são  recortados
	//	out_tri1 = in_tri;
	//	return 1; //Apenas um triangulo é valido o Original
	//}

	//if (nInsidePoints == 1 && nOutsidePoints == 2) {
	//	//O triangulo tem de ser recorttado, como tem dois pontos fora
	//	//o triangulo vira mais pequeno

	//	//Copiar a aparencia do triangulo
	//	out_tri1.col = in_tri.col;
	//	out_tri1.sym = in_tri.sym;
	//	
	//	//O ponto interior é valido então guardamos-o 
	//	//o out_tri.p é um vec3d e uma refencia agr eu guardo nele o valor que o inside_Point está a apontar
	//	out_tri1.p[0] = *inside_Points[0];

	//	//Os dois proximos pontos são os pontos das linhas originais do Triangulo  que intercetam com o plano
	//	//logo temos de camlcular as posições desses novos pontos
	//	
	//	out_tri1.p[1] = Vector_IntersectPlane(plane_p, plane_n, *inside_Points[0], *outside_Points[0]);
	//	out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_Points[0], *outside_Points[1]);

	//	return 1;//devolve um triangulo completamente novo
	//}

	//if (nInsidePoints == 2 && nOutsidePoints == 1) {
	//	//Aqui o triangul o nessecita ser recortado obvio que tem um pequeno problema
	//	//Como tem dois pontos dentro e apenas 1 fora será criado dois pontos na interceção com o plano
	//	//A qual irá gerar um poligono com 4 pontos, ai será preciso tornar esse poligono de 4 pontos em 2 triangulos

	//	//Copiamos a aparencia para os novos triangulos
	//	out_tri1.col = in_tri.col;
	//	out_tri1.sym = in_tri.sym;

	//	out_tri2.col = in_tri.col;
	//	out_tri2.sym = in_tri.sym;


	//	//O primeiro triangulo é feito a partir dos dois pontos que estejam dentro do triangulo
	//	//e um novo ponto que seja a interceção com o plano logo
	//	out_tri1.p[0] = *inside_Points[0];
	//	out_tri1.p[1] = *inside_Points[1];


	//	//o terceiro ponto sera um dos pontos de interceção    
	//	out_tri1.p[2] = Vector_IntersectPlane(plane_p, plane_n, *inside_Points[1], *outside_Points[0]);


	//	//O segundo triangulo tera de entrada o 1 ponto que esta dentro
	//	//O ponto da interceção ja calculado e o ponto de fora

	//	out_tri2.p[0] = *inside_Points[0];
	//	out_tri2.p[1] = out_tri1.p[2];
	//	out_tri2.p[0] = Vector_IntersectPlane(plane_p, plane_n, *inside_Points[0], *outside_Points[0]);

	//	return 2; //O numero de triangulos novos calculados

	//}

}