#include"OpenGL.h"


void Engine::Init(HDC& hDC, HWND& hWnd)
{

	PIXELFORMATDESCRIPTOR	pfd = {
sizeof(PIXELFORMATDESCRIPTOR),
1,
PFD_DRAW_TO_WINDOW |
PFD_DOUBLEBUFFER |
PFD_SUPPORT_OPENGL |
PFD_TYPE_RGBA,
32,
0, 0, 0, 0, 0, 0,
0,0,
0,0,0,0,0,
16,
0,
0,
PFD_MAIN_PLANE,
0,
0, 0, 0
	};
	hDC = GetDC(hWnd);
	int PixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, PixelFormat, &pfd);
}

void Engine::fnInterpolation(List<List<Koordes>>& list)
{
	/*size_t Size;

	for (Node<List<Koordes>>* ptrList = list.Begin(); ptrList != nullptr;)
	{
		List<double> koordes_x;
		List<double> koordes_y;
		Size = ptrList->data.GetSize();
		if (Size < 10)
		{
			for (Node<Koordes>* ptrKoordes = ptrList->data.Begin(); ptrKoordes != nullptr;)
			{
				koordes_x.Push(ptrKoordes->data.x);
				koordes_y.Push(ptrKoordes->data.y);
				ptrKoordes = ptrKoordes->next;
			}
			ListKoord_x.Push(koordes_x);
			ListKoord_y.Push(koordes_y);
			koordes_x.Clear();
			koordes_y.Clear();
		}
		else
		{

		}
	
		ptrList = ptrList->next;
	}
	for (size_t i = 0; i < 10; i++)
	{
		KoordParam_t.Push(-1 + 0.2 * i);
	}

	SizeInterpolation = ListKoord_x.GetSize();
	interpolationX = new Intepolation<double>[SizeInterpolation];
	interpolationY = new Intepolation<double>[SizeInterpolation];
	Node<List<double>>* ptrListX = ListKoord_x.Begin();
	Node<List<double>>* ptrListY = ListKoord_y.Begin();
	for (size_t i = 0; i < SizeInterpolation; i++)
	{
		std::cout << "interpolation number =\t" << i + 1 << std::endl;
		interpolationX[i].Matrix_LU(ptrListX->data, KoordParam_t);
		interpolationY[i].Matrix_LU(ptrListY->data, KoordParam_t);
		ptrListX = ptrListX->next;
		ptrListY = ptrListY->next;
	}*/
}

void Engine::fnApproximation(List<List<Koordes>>& list, size_t n)
{
	SizeInterpolation = list.GetSize();

	approximationY = new Approximation<double>[SizeInterpolation];
	approximationX = new Approximation<double>[SizeInterpolation];
	int i = 0;
	int j;
	double delta = 0;
	for (Node<List<Koordes>>* ptrList = list.Begin(); ptrList != nullptr;)
	{
		List<double> koordes_x;
		List<double> koordes_y;
		List<double> param_t;
		j = 0;
		double dt = 0;
		
		for (Node<Koordes>* ptrKoordes = ptrList->data.Begin(); ptrKoordes != nullptr;)
		{
			delta++;
			ptrKoordes = ptrKoordes->next;
		}
		delta = (double)(2 / delta);
		for (Node<Koordes>* ptrKoordes = ptrList->data.Begin(); ptrKoordes != nullptr;)
		{
			
			koordes_x.Push(ptrKoordes->data.x);
			koordes_y.Push(ptrKoordes->data.y);
			param_t.Push(-1 + dt * delta);
			dt++;
			ptrKoordes = ptrKoordes->next;
		}
		(approximationX + i)->SetSize(n);
		(approximationY + i)->SetSize(n);
		(approximationX + i)->Matrix_LU(koordes_x, param_t);
		(approximationY + i)->Matrix_LU(koordes_y, param_t);
		i++;
		ptrList = ptrList->next;
	}

}

void Engine::DrawApproximation()
{
	glClearColor(0, 0, 0, 1);


	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glColor3f(0.0f, 1.0f, 1.0f);
	double delta;
	double param_x;
	double x;
	double y;
	for (size_t i = 0; i < SizeInterpolation; i++)
	{
		delta = 1 / 50;

		std::cout << "interpolation number =\t" << i + 1 << std::endl;
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < 100; j++)
		{
			param_x = -1 + (double)j * 1 / 50;
			x = approximationX[i].Polinom(param_x);
			y = approximationY[i].Polinom(param_x);
			glVertex3d(approximationX[i].Polinom(param_x), approximationY[i].Polinom(param_x), 0);
			
		}
		glEnd();
	}
}

void Engine::Draw( const List<Koordes>& list)
{
	glClearColor(0, 0, 0, 1);


	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_LINE_STRIP);
	for (Node<Koordes>* ptr = list.Begin(); ptr != NULL; )
	{

		glVertex3f(ptr->data.x, ptr->data.y, 0.0);
		ptr = ptr->next;
	}
	glEnd();
	
}

void Engine::TransformMatrixMonitor(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void Engine::DrawList(const List<List<Koordes>>& list)
{
	glClearColor(0, 0, 0, 1);


	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glColor3f(1.0f, 1.0f, 1.0f);


	
	for (Node<List<Koordes>>* ptrGlobal = list.Begin(); ptrGlobal != NULL; )
	{
		glBegin(GL_LINE_STRIP);
		for (Node<Koordes>*ptr = ptrGlobal->data.Begin(); ptr!=NULL;)
		{
			glVertex3f(ptr->data.x, ptr->data.y, 0.0);
			ptr = ptr->next;
		}
		glEnd();
		ptrGlobal = ptrGlobal->next;
	}
	
}
void	Engine::Draw()
{
	glClearColor(0, 0, 0, 1);


	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glColor3f(1.0f, 0.0f, 1.0f);

	for (size_t i = 0; i < SizeInterpolation; i++)
	{
	
	//	std::cout << "interpolation number =\t" << i + 1 << std::endl;
		glBegin(GL_LINE_STRIP);
		for (size_t j = 0; j < 100; j++)
		{
			glVertex3d(interpolationX[i].Polinom(-1 +(double) j / 50), interpolationY[i].Polinom(-1 + (double)j / 50), 0);

		}
		glEnd();
	}
}
void Engine::Centr(const List<List<Koordes>>& input_koordes)
{
	Koordes Vector;
	Vector.x = 0;
	Vector.y = 0;
	int Number = 0;
	for (Node<List<Koordes>>* ptrGlobal = input_koordes.Begin(); ptrGlobal != NULL;)
	{
		for (Node<Koordes>* ptr = ptrGlobal->data.Begin(); ptr != NULL;)
		{
			Vector.x += ptr->data.x;
			Vector.y += ptr->data.y;
			Number++;
			ptr = ptr->next;
		}
		Number++;
		ptrGlobal = ptrGlobal->next;
	}
	Vector.x = Vector.x / Number;
	Vector.y = Vector.y / Number;
	for (Node<List<Koordes>>* ptrGlobal = input_koordes.Begin(); ptrGlobal != NULL;)
	{
		for (Node<Koordes>* ptr = ptrGlobal->data.Begin(); ptr != NULL;)
		{
			ptr->data.x -= Vector.x;
			ptr->data.y -= Vector.y;
			Number++;
			ptr = ptr->next;
		}
		Number++;
		ptrGlobal = ptrGlobal->next;
	}

}