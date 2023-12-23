// olc3DEngine.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include "olcConsoleGameEngine.h"
#include "MathFunctions.h"
#include <iostream>
#include <algorithm>




class olcEngine3D : public olcConsoleGameEngine {
public:
	olcEngine3D() {
		m_sAppName = L"3D Engine";
	}

private:
	mesh meshCube;
	mat4x4 matrizProjecao;

	vec3d vCamera;//Vetor que guarda a posição da Camara no espaco 3D 
	vec3d vLookDirection;

	//Serve para rodar o cubo apenas
	float fTheta;
	float fYaw;		// FPS Camera rotation in XZ plane

	/// <summary>
	/// função tirada da net mas o que fz e returnar uma cor
	/// </summary>
	/// <param name="lum"></param>
	/// <returns></returns>
	CHAR_INFO GetColour(float lum)
	{
		short bg_col, fg_col;
		wchar_t sym;
		int pixel_bw = (int)(13.0f * lum);
		switch (pixel_bw)
		{
		case 0: bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID; break;

		case 1: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_QUARTER; break;
		case 2: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_HALF; break;
		case 3: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_THREEQUARTERS; break;
		case 4: bg_col = BG_BLACK; fg_col = FG_DARK_GREY; sym = PIXEL_SOLID; break;

		case 5: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_QUARTER; break;
		case 6: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_HALF; break;
		case 7: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_THREEQUARTERS; break;
		case 8: bg_col = BG_DARK_GREY; fg_col = FG_GREY; sym = PIXEL_SOLID; break;

		case 9:  bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_QUARTER; break;
		case 10: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_HALF; break;
		case 11: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_THREEQUARTERS; break;
		case 12: bg_col = BG_GREY; fg_col = FG_WHITE; sym = PIXEL_SOLID; break;
		default:
			bg_col = BG_BLACK; fg_col = FG_BLACK; sym = PIXEL_SOLID;
		}

		CHAR_INFO c;
		c.Attributes = bg_col | fg_col;
		c.Char.UnicodeChar = sym;
		return c;
	}

public:
	bool OnUserCreate() override {




		meshCube.LoadFromObjectFile("teapot.obj");



		//projeção proxima
		float fNear = 0.1f;
		//longe
		float fFar = 1000.0f;
		//Campo de Visão
		float fFov = 90.0f;
		//Ratio do tamanho da janela
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		//Campo de visao em Radianos
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * Pi);

		matrizProjecao = MathFunctions::Matrix_MakeProjection(fFov, fAspectRatio, fNear, fFar);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {


		HandleMoviment(fElapsedTime);




		//Limpa a tela com preto
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		// Set up rotation matrices
		mat4x4 matRotZ, matRotX;

		fTheta += 1.0f * fElapsedTime;



		// Rotation Z
		matRotZ = MathFunctions::Matrix_MakeRotationZ(fTheta * 0.5f);

		// Rotation X
		matRotX = MathFunctions::Matrix_MakeRotationX(fTheta);


		mat4x4 matrizTranslacao;
		matrizTranslacao = MathFunctions::Matrix_MakeTranslation(0.0f, 0.0f, 5.0f);

		mat4x4 matrizMundo;
		matrizMundo = MathFunctions::Matrix_MakeIdentity();
		matrizMundo = MathFunctions::Matrix_MultiplyMatrix(matRotZ, matRotX);
		matrizMundo = MathFunctions::Matrix_MultiplyMatrix(matrizMundo, matrizTranslacao);


		vec3d vUp = { 0,1,0 };
		vec3d vTarget = { 0,0,1 };

		mat4x4 matCameraRot = MathFunctions::Matrix_MakeRotationY(fYaw);
		vLookDirection = MathFunctions::Matrix_MultiplyVector(matCameraRot, vTarget);
		vTarget = MathFunctions::Vector_Add(vCamera, vLookDirection);



		mat4x4 matCamera = MathFunctions::Matrix_PointAt(vCamera, vTarget, vUp);

		//A inversa da camera é a matriz da vista
		mat4x4 matView = MathFunctions::Matrix_QuickInverse(matCamera);

		//vetor de triangulso que irao ser desenhados no ecra
		std::vector<triangle> vetorTriangulosToRaster;


		//Desenhar triangulos
		for (auto tri : meshCube.tris) {

			//estrutura dum triangulo para guardar os vertices
			triangle triProjetado, triTransformed, triViewed;

			triTransformed.p[0] = MathFunctions::Matrix_MultiplyVector(matrizMundo, tri.p[0]);
			triTransformed.p[1] = MathFunctions::Matrix_MultiplyVector(matrizMundo, tri.p[1]);
			triTransformed.p[2] = MathFunctions::Matrix_MultiplyVector(matrizMundo, tri.p[2]);


			//Multiplicao Cruzada para obter o vetor normal
			vec3d normal, line1, line2;

			//obter as linhas laterais do triangulo
			line1 = MathFunctions::Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
			line2 = MathFunctions::Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

			//multiplicação cruzada para obter a linha normal a superfice do triangulo
			normal = MathFunctions::Vector_CrossProduct(line1, line2);

			//Normalizar o vetor
			MathFunctions::NormalizeVectorByReference(normal);


			// Cria um vetor 
			vec3d vCameraRay = MathFunctions::Vector_Sub(triTransformed.p[0], vCamera);

			//if < 0) ve a parte da frente se for <0 ve a parte interior do cubo
			if (MathFunctions::Vector_DotProduct(normal, vCameraRay) < 0.0f) {

				//iluminação
				vec3d light_direction = { 0.0f,0.0f,-1.0f };
				MathFunctions::NormalizeVectorByReference(light_direction); //Normalizar o vetor

				//Verifica  se o raio esta alinha com a camara, e se estar quer dizer que o triangulo esta visivel a camera
				float dp = max(0.1f, MathFunctions::Vector_DotProduct(light_direction, normal));


				//Merdas por causa de estarmos na consola
				CHAR_INFO c = GetColour(dp);

				triTransformed.col = c.Attributes;
				triTransformed.sym = c.Char.UnicodeChar;

				//Converte espaço mundo para espaço da vista da camera
				triViewed.p[0] = MathFunctions::Matrix_MultiplyVector(matView, triTransformed.p[0]);
				triViewed.p[1] = MathFunctions::Matrix_MultiplyVector(matView, triTransformed.p[1]);
				triViewed.p[2] = MathFunctions::Matrix_MultiplyVector(matView, triTransformed.p[2]);



				// Projeta cada vértice do triângulo utilizando a matriz de projeção.
				//basicamente transforma o triangulos 3D em 2D

				triProjetado.p[0] = MathFunctions::Matrix_MultiplyVector(matrizProjecao, triViewed.p[0]); //X
				triProjetado.p[1] = MathFunctions::Matrix_MultiplyVector(matrizProjecao, triViewed.p[1]); //Y
				triProjetado.p[2] = MathFunctions::Matrix_MultiplyVector(matrizProjecao, triViewed.p[2]); //Z
				triProjetado.col = c.Attributes;
				triProjetado.sym = c.Char.UnicodeChar;

				//Normalizamos os vetores aqui
				triProjetado.p[0] = MathFunctions::Vector_Div(triProjetado.p[0], triProjetado.p[0].w);
				triProjetado.p[1] = MathFunctions::Vector_Div(triProjetado.p[1], triProjetado.p[1].w);
				triProjetado.p[2] = MathFunctions::Vector_Div(triProjetado.p[2], triProjetado.p[2].w);


				//OffSet
				vec3d vOffSetView = { 1,1,0 };
				triProjetado.p[0] = MathFunctions::Vector_Add(triProjetado.p[0], vOffSetView);
				triProjetado.p[1] = MathFunctions::Vector_Add(triProjetado.p[1], vOffSetView);
				triProjetado.p[2] = MathFunctions::Vector_Add(triProjetado.p[2], vOffSetView);


				// Mapeia os vértices para as dimensões da tela.

				triProjetado.p[0].x *= 0.5f * (float)ScreenWidth();
				triProjetado.p[0].y *= 0.5f * (float)ScreenHeight();
				triProjetado.p[1].x *= 0.5f * (float)ScreenWidth();
				triProjetado.p[1].y *= 0.5f * (float)ScreenHeight();
				triProjetado.p[2].x *= 0.5f * (float)ScreenWidth();
				triProjetado.p[2].y *= 0.5f * (float)ScreenHeight();

				//guardo os triangulos que iram ser desenahdos
				vetorTriangulosToRaster.push_back(triProjetado);

			}

		}

		//vamos organizar os triangulos
		//lambda function
		//back to front
		sort(vetorTriangulosToRaster.begin(), vetorTriangulosToRaster.end(), [](triangle& t1, triangle& t2) {

			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
		float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
		return z1 > z2;
			});


		for (auto& triProjetado : vetorTriangulosToRaster) {
			// Rasterize triangle
			FillTriangle(triProjetado.p[0].x, triProjetado.p[0].y,
				triProjetado.p[1].x, triProjetado.p[1].y,
				triProjetado.p[2].x, triProjetado.p[2].y,
				triProjetado.sym, triProjetado.col);

			/*DrawTriangle(triProjetado.p[0].x, triProjetado.p[0].y,
			triProjetado.p[1].x, triProjetado.p[1].y,
			triProjetado.p[2].x, triProjetado.p[2].y,
			PIXEL_SOLID, FG_BLACK);*/
		}

		return true;

	}


	void HandleMoviment(float  fElapsedTime) {
		if (GetKey(VK_UP).bHeld)
			vCamera.y -= 8.0f * fElapsedTime;	// Travel Upwards

		if (GetKey(VK_DOWN).bHeld)
			vCamera.y += 8.0f * fElapsedTime;	// Travel Downwards


		// Dont use these two in FPS mode, it is confusing :P
		if (GetKey(VK_LEFT).bHeld)
			vCamera.x -= 8.0f * fElapsedTime;	// Travel Along X-Axis

		if (GetKey(VK_RIGHT).bHeld)
			vCamera.x += 8.0f * fElapsedTime;	// Travel Along X-Axis
		///////

		vec3d vForward = MathFunctions::Vector_Mul(vLookDirection, 8.0f * fElapsedTime);

		// Standard FPS Control scheme, but turn instead of strafe
		if (GetKey(L'W').bHeld)
			vCamera = MathFunctions::Vector_Add(vCamera, vForward);

		if (GetKey(L'S').bHeld)
			vCamera = MathFunctions::Vector_Sub(vCamera, vForward);

		if (GetKey(L'A').bHeld)
			fYaw -= 2.0f * fElapsedTime;

		if (GetKey(L'D').bHeld)
			fYaw += 2.0f * fElapsedTime;

	}
};

int main()
{
	olcEngine3D demo;

	if (demo.ConstructConsole(256, 240, 4, 4)) {
		demo.Start();
	}
	return 0;
}

