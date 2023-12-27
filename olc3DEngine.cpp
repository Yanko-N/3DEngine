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




		meshCube.LoadFromObjectFile("mountains.obj");



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

		/*fTheta += 1.0f * fElapsedTime;*/



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
			triangle triProjected, triTransformed, triViewed;

			// World Matrix Transform
			triTransformed.p[0] = MathFunctions::Matrix_MultiplyVector(matrizMundo, tri.p[0]);
			triTransformed.p[1] = MathFunctions::Matrix_MultiplyVector(matrizMundo, tri.p[1]);
			triTransformed.p[2] = MathFunctions::Matrix_MultiplyVector(matrizMundo, tri.p[2]);

			// Calculate triangle Normal
			vec3d normal, line1, line2;

			// Get lines either side of triangle
			line1 = MathFunctions::Vector_Sub(triTransformed.p[1], triTransformed.p[0]);
			line2 = MathFunctions::Vector_Sub(triTransformed.p[2], triTransformed.p[0]);

			// Take cross product of lines to get normal to triangle surface
			normal = MathFunctions::Vector_CrossProduct(line1, line2);

			// You normally need to normalise a normal!
			normal = MathFunctions::Vector_Normalise(normal);

			// Get Ray from triangle to camera
			vec3d vCameraRay = MathFunctions::Vector_Sub(triTransformed.p[0], vCamera);

			// If ray is aligned with normal, then triangle is visible
			if (MathFunctions::Vector_DotProduct(normal, vCameraRay) < 0.0f)
			{
				// Illumination
				vec3d light_direction = { 0.0f, 1.0f, -1.0f };
				light_direction = MathFunctions::Vector_Normalise(light_direction);

				// How "aligned" are light direction and triangle surface normal?
				float dp = max(0.1f, MathFunctions::Vector_DotProduct(light_direction, normal));

				// Choose console colours as required (much easier with RGB)
				CHAR_INFO c = GetColour(dp);
				triTransformed.col = c.Attributes;
				triTransformed.sym = c.Char.UnicodeChar;

				// Convert World Space --> View Space
				triViewed.p[0] = MathFunctions::Matrix_MultiplyVector(matView, triTransformed.p[0]);
				triViewed.p[1] = MathFunctions::Matrix_MultiplyVector(matView, triTransformed.p[1]);
				triViewed.p[2] = MathFunctions::Matrix_MultiplyVector(matView, triTransformed.p[2]);
				triViewed.sym = triTransformed.sym;
				triViewed.col = triTransformed.col;

				// Clip Viewed Triangle against near plane, this could form two additional
				// additional triangles. 
				int nClippedTriangles = 0;
				triangle clipped[2];
				nClippedTriangles = MathFunctions::Triangle_ClipAgainstPlain({ 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.0f }, triViewed, clipped[0], clipped[1]);

				// We may end up with multiple triangles form the clip, so project as
				// required
				for (int n = 0; n < nClippedTriangles; n++)
				{
					// Project triangles from 3D --> 2D
					triProjected.p[0] = MathFunctions::Matrix_MultiplyVector(matrizProjecao, clipped[n].p[0]);
					triProjected.p[1] = MathFunctions::Matrix_MultiplyVector(matrizProjecao, clipped[n].p[1]);
					triProjected.p[2] = MathFunctions::Matrix_MultiplyVector(matrizProjecao, clipped[n].p[2]);
					triProjected.col = clipped[n].col;
					triProjected.sym = clipped[n].sym;

					// Scale into view, we moved the normalising into cartesian space
					// out of the matrix.vector function from the previous videos, so
					// do this manually
					triProjected.p[0] = MathFunctions::Vector_Div(triProjected.p[0], triProjected.p[0].w);
					triProjected.p[1] = MathFunctions::Vector_Div(triProjected.p[1], triProjected.p[1].w);
					triProjected.p[2] = MathFunctions::Vector_Div(triProjected.p[2], triProjected.p[2].w);

					// X/Y are inverted so put them back
					triProjected.p[0].x *= -1.0f;
					triProjected.p[1].x *= -1.0f;
					triProjected.p[2].x *= -1.0f;
					triProjected.p[0].y *= -1.0f;
					triProjected.p[1].y *= -1.0f;
					triProjected.p[2].y *= -1.0f;

					// Offset verts into visible normalised space
					vec3d vOffsetView = { 1,1,0 };
					triProjected.p[0] = MathFunctions::Vector_Add(triProjected.p[0], vOffsetView);
					triProjected.p[1] = MathFunctions::Vector_Add(triProjected.p[1], vOffsetView);
					triProjected.p[2] = MathFunctions::Vector_Add(triProjected.p[2], vOffsetView);
					triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
					triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
					triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
					triProjected.p[2].y *= 0.5f * (float)ScreenHeight();

					// Store triangle for sorting
					vetorTriangulosToRaster.push_back(triProjected);
				}
			}

		}

		// Sort triangles from back to front
		sort(vetorTriangulosToRaster.begin(), vetorTriangulosToRaster.end(), [](triangle& t1, triangle& t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
		float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
		return z1 > z2;
			});

		// Clear Screen
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		// Loop through all transformed, viewed, projected, and sorted triangles
		for (auto& triToRaster : vetorTriangulosToRaster)
		{
			// Clip triangles against all four screen edges, this could yield
			// a bunch of triangles, so create a queue that we traverse to 
			//  ensure we only test new triangles generated against planes
			triangle clipped[2];
			std::list<triangle> listTriangles;

			// Add initial triangle
			listTriangles.push_back(triToRaster);
			int nNewTriangles = 1;

			for (int p = 0; p < 4; p++)
			{
				int nTrisToAdd = 0;
				while (nNewTriangles > 0)
				{
					// Take triangle from front of queue
					triangle test = listTriangles.front();
					listTriangles.pop_front();
					nNewTriangles--;

					// Clip it against a plane. We only need to test each 
					// subsequent plane, against subsequent new triangles
					// as all triangles after a plane clip are guaranteed
					// to lie on the inside of the plane. I like how this
					// comment is almost completely and utterly justified
					switch (p)
					{
					case 0:	nTrisToAdd = MathFunctions::Triangle_ClipAgainstPlain({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 1:	nTrisToAdd = MathFunctions::Triangle_ClipAgainstPlain({ 0.0f, (float)ScreenHeight() - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 2:	nTrisToAdd = MathFunctions::Triangle_ClipAgainstPlain({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					case 3:	nTrisToAdd = MathFunctions::Triangle_ClipAgainstPlain({ (float)ScreenWidth() - 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
					}

					// Clipping may yield a variable number of triangles, so
					// add these new ones to the back of the queue for subsequent
					// clipping against next planes
					for (int w = 0; w < nTrisToAdd; w++)
						listTriangles.push_back(clipped[w]);
				}
				nNewTriangles = listTriangles.size();
			}


			// Draw the transformed, viewed, clipped, projected, sorted, clipped triangles
			for (auto& t : listTriangles)
			{
				FillTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, t.sym, t.col);
				DrawTriangle(t.p[0].x, t.p[0].y, t.p[1].x, t.p[1].y, t.p[2].x, t.p[2].y, PIXEL_SOLID, FG_BLACK);
			}
		}

		return true;

	}


	void HandleMoviment(float  fElapsedTime) {
		if (GetKey(VK_UP).bHeld)
			vCamera.y += 8.0f * fElapsedTime;	// Travel Upwards

		if (GetKey(VK_DOWN).bHeld)
			vCamera.y -= 8.0f * fElapsedTime;	// Travel Downwards


		

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

