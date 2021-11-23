#pragma once
#include<Windows.h>
#include<gl/GL.h>
#include"List.h"
#include"Interpolation.h"
#include"Approximation.h"
struct Koordes
{
	double x;
	double y;
};


class Engine
{
	List<List<double>> ListKoord_x;
	List<List<double>> ListKoord_y;
	Approximation<double>* approximationX;
	Approximation<double>* approximationY;
	size_t SizeInterpolation;
	size_t SizeNumberApproximation;
public:
	void Init(HDC& hDC, HWND& hWnd);
	void TransformMatrixMonitor(int width, int height);
	void Paint(GLdouble x)
	{
		glBegin(GL_LINE_LOOP);

		glVertex3f(-x, -0.95f, 0.0f);
		glVertex3f(x, -0.95f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();
	}

	Intepolation<double>* interpolationX;
	Intepolation<double>* interpolationY;
	
	void fnInterpolation(List<List<Koordes>>& list);
	void fnApproximation(List<List<Koordes>>& list, size_t n);
	void DrawApproximation();
	void Draw();
	void Draw( const List<Koordes>& list);
	void DrawList(const List<List<Koordes>>& list);


	void Centr(const List<List<Koordes>>& input_Koordes);
	
	
	void Clear()
	{
	
		ListKoord_x.Clear();
		ListKoord_y.Clear();
		/*for (int i = 0; i < SizeInterpolation; i++)
		{
			interpolationX[i].Clear();
			interpolationY[i].Clear();
		}
		delete[]interpolationX;
		delete[]interpolationY;*/
	}
};