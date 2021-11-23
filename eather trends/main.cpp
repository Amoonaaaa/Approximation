#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include"ApproximationPolinom.h"
#include"WinApiWindow.h"
#include<gl/GL.h>
#include"ExcelFile.h"
#include<sstream>
#include<string>

// функции обратного вызова
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndChild(HWND, UINT, WPARAM, LPARAM);
// класс дочернего окна
IWindowCore* childWindowCore = nullptr;
HDC hDC;
HGLRC hRC;
PAINTSTRUCT ps;
HWND hNamePath;
static HWND hInputPath;
HWND hNameSizePolinom;
HWND hButtonInitializeFile;
HWND hGlobalSizePolinom;
HWND hScrollBarRed;
HWND hScrollBarGreen;
HWND hScrollBarBlue;
HWND hColorRed;
HWND hColorGreen;
HWND hColorBlue;
HWND hTextRed;
HWND hTextGreen;
HWND hTextBlue;

HMENU hMenuBar;
HMENU hFile;
HMENU hExit;
#define INPUT_PATH 1
#define BUTTON_INITIALIZE_FILE 2
#define PATH_SAVE_FILE 3
#define EXIT 4
#define TEXT_COLOR_RED 5
#define TEXT_COLOR_GREEN 6
#define TEXT_COLOR_BLUE 7


int redPos = 128;
int bluePos = 128;
int greenPos = 128;
const int sizeTextPos = 4;
 //char textRedPos[sizeTextPos];
std::string textRedPos;
std::string textGreenPos;
std::string textBluePos;
 char* suppText = nullptr;
RECT rectRed;
ExcelFile exFile;
uint32_t globalSizePolinom = 2;
ApproximationPolinom<double> appP;
ApproximationPolinom<double> appP1;
ApproximationPolinom<double> appP2;
bool activePolinom = false;
double input_x[] = { -1.0, -0.5, 0.0, 0.5, 1.0 };
double input_y[] = { -1.0, -0.3, 0.0, 0.7, 1.0 };
// контекст opengl
void DrawTreangles(double* input_x, double* input_y, uint32_t sizeInput);
void DrawPolinom( ApproximationPolinom<double>&, double red, double green, double blue);
void InitializeOpenGL(HDC& hDC, HWND& hWnd);
HWND CreateAHorizontalScrollBar(HWND hParentWnd, int sbHeight);

int main()
{
	IWindowCore* MainWindowCore = new WinApiWindow(WHITE_BRUSH,L"Class weather trends", L"weather trends",1024, 800,
		WndProc,0,0,GetModuleHandle(nullptr), CS_HREDRAW|CS_VREDRAW|CS_OWNDC);
	MainWindowCore->Initialize();
	MainWindowCore->MainLoop();
	return 0;
}
const uint32_t sizePathFile = 500;
char pathFile[sizePathFile];


std::stringstream s;

double data_y;
//std::vector<double> prev_pos_y(96 * 13);
std::vector<double> pos_x(96);
std::vector<double> pos_y(96);
double min, max;
std::string** dataOut = nullptr;
const uint32_t numberSizePol = 2;
char sizePolinomText[numberSizePol];
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		textRedPos.resize(sizeTextPos);
		textGreenPos.resize(sizeTextPos);
		textBluePos.resize(sizeTextPos);
		childWindowCore = new WinApiWindow(WHITE_BRUSH, L"Class opengl", L"", 512, 400,
			WndChild, 0, 0, GetModuleHandle(nullptr), CS_HREDRAW | CS_VREDRAW | CS_OWNDC, WS_VISIBLE | WS_CHILD | WS_BORDER, hWnd);
		childWindowCore->Initialize();
		hNamePath = CreateWindow(L"static", L"Введите путь и название файла с расширением:", WS_VISIBLE | WS_CHILD,
			0, 450, 350, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hNameSizePolinom = CreateWindow(L"static", L"Введите количество членов:", WS_VISIBLE | WS_CHILD,
			355, 450, 200, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hInputPath = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL/*| ES_AUTOVSCROLL|ES_LEFT |
			ES_MULTILINE*/, 0, 470, 350, 60, hWnd, reinterpret_cast<HMENU>(INPUT_PATH), GetModuleHandle(nullptr), nullptr);
		hButtonInitializeFile = CreateWindow(L"button", L"загрузить", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 530,
			140, 25, hWnd, reinterpret_cast<HMENU>(BUTTON_INITIALIZE_FILE), GetModuleHandle(nullptr), nullptr);
		hGlobalSizePolinom = CreateWindow(L"edit", L"2", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER ,
			355, 470, 25, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hButtonInitializeFile = CreateWindow(L"button", L"загрузить", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 530,
			140, 25, hWnd, reinterpret_cast<HMENU>(BUTTON_INITIALIZE_FILE), GetModuleHandle(nullptr), nullptr);
		hNamePath = CreateWindow(L"static", L"Введите путь и название файла с расширением:", WS_VISIBLE | WS_CHILD,
			0, 450, 350, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);



		hColorRed = CreateWindow(L"static", L"диапазон красного цвета:", WS_VISIBLE | WS_CHILD,
			600, 0, 200, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hScrollBarRed = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE,
			600, 20, 200, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);
		SetScrollRange(hScrollBarRed, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hScrollBarRed, SB_CTL, redPos, TRUE);
		hTextRed = CreateWindow(L"static", L"128", WS_VISIBLE | WS_CHILD, 600, 40, 50, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);
		


		hColorGreen = CreateWindow(L"static", L"диапазон зеленого цвета:", WS_VISIBLE | WS_CHILD,
			600, 65, 200, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hScrollBarGreen = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE,
			600, 85, 200, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);
		SetScrollRange(hScrollBarGreen, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hScrollBarGreen, SB_CTL, greenPos, TRUE);
		hTextGreen = CreateWindow(L"static", L"128", WS_VISIBLE | WS_CHILD, 600, 105, 50, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);



		hColorBlue = CreateWindow(L"static", L"диапазон синего цвета:", WS_VISIBLE | WS_CHILD,
			600, 120, 200, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hScrollBarBlue = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE,
			600, 140, 200, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);
		SetScrollRange(hScrollBarBlue, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hScrollBarBlue, SB_CTL, bluePos, TRUE);
		hTextBlue = CreateWindow(L"static", L"128", WS_VISIBLE | WS_CHILD, 600, 160, 50, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);

		hMenuBar = CreateMenu();
		hFile = CreateMenu();
		AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"Файл");
		AppendMenu(hFile, MF_STRING, PATH_SAVE_FILE, L"Сохранение файла");
		AppendMenu(hFile, MF_STRING, EXIT, L"Выход из программы");
		SetMenu(hWnd, hMenuBar);

		/*hColorGreen = CreateWindow(L"static", L"диапазон зеленого цвета:", WS_VISIBLE | WS_CHILD,
			600, 60, 200, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hScrollBarGreen = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE,
			600, 80, 200, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);
		SetScrollRange(hScrollBarGreen, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hScrollBarGreen, SB_CTL, bluePos, TRUE);


		hColorBlue = CreateWindow(L"static", L"диапазон синего цвета цвета:", WS_VISIBLE | WS_CHILD,
			600, 120, 200, 20, hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
		hScrollBarBlue = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE,
			600, 140, 200, 20, hWnd, NULL, GetModuleHandle(nullptr), NULL);
		SetScrollRange(hScrollBarBlue, SB_CTL, 0, 255, TRUE);
		SetScrollPos(hScrollBarBlue, SB_CTL, greenPos, TRUE);*/
		return 0;
	case WM_HSCROLL:
	
		if ((HWND)lp == hScrollBarRed)
		{
			switch (LOWORD(wp))
			{
			case SB_PAGERIGHT:
			{redPos += 10; break; }
			case SB_LINERIGHT:
			{redPos += 1; break; }
			case SB_PAGELEFT:
			{redPos -= 10; break; }
			case SB_LINELEFT:
			{redPos -= 1; break; }
			case SB_TOP:
			{redPos = 0; break; }
			case SB_BOTTOM:
			{redPos = 255; break; }
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
			{redPos = HIWORD(wp); break; }
			}
			if (redPos > 255)
				redPos = 255;
			if (redPos < 0)
				redPos = 0;
			SetScrollPos(hScrollBarRed, SB_CTL, redPos, TRUE);
			//std::cout << redPos << std::endl;


			GetWindowTextA(hTextRed, suppText, sizeTextPos);
			//suppText = const_cast<char*>(s.str().c_str());
			textRedPos = std::to_string(redPos);
			SetWindowTextA(hTextRed, textRedPos.c_str());
		}

		if ((HWND)lp == hScrollBarGreen)
		{
			switch (LOWORD(wp))

			{
			case SB_PAGERIGHT:
			{greenPos += 10; break; }
			case SB_LINERIGHT:
			{greenPos += 1; break; }
			case SB_PAGELEFT:
			{greenPos -= 10; break; }
			case SB_LINELEFT:
			{greenPos -= 1; break; }
			case SB_TOP:
			{greenPos = 0; break; }
			case SB_BOTTOM:
			{greenPos = 255; break; }
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
			{greenPos = HIWORD(wp); break; }

			}
			if (greenPos > 255)
				greenPos = 255;
			if (greenPos < 0)
				greenPos = 0;
			
			SetScrollPos(hScrollBarGreen, SB_CTL, greenPos, TRUE);
			GetWindowTextA(hTextGreen, suppText, sizeTextPos);
			textGreenPos = std::to_string(greenPos);
			SetWindowTextA(hTextGreen, textGreenPos.c_str());
		}

		if ((HWND)lp == hScrollBarBlue)
		{
			switch (LOWORD(wp))

			{
			case SB_PAGERIGHT:
			{bluePos += 10; break; }
			case SB_LINERIGHT:
			{bluePos += 1; break; }
			case SB_PAGELEFT:
			{bluePos -= 10; break; }
			case SB_LINELEFT:
			{bluePos -= 1; break; }
			case SB_TOP:
			{bluePos = 0; break; }
			case SB_BOTTOM:
			{bluePos = 255; break; }
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
			{bluePos = HIWORD(wp); break; }

			}
			if (bluePos > 255)
				bluePos = 255;
			if (bluePos < 0)
				bluePos = 0;
			
			SetScrollPos(hScrollBarBlue, SB_CTL, bluePos, TRUE);
			GetWindowTextA(hTextBlue, suppText, sizeTextPos);
			textBluePos = std::to_string(bluePos);
			SetWindowTextA(hTextBlue, textBluePos.c_str());
		}
	
	break;

	

		return 0;
	case WM_COMMAND:

		switch (wp)
		{

		case BUTTON_INITIALIZE_FILE:
			GetWindowTextA(hInputPath, pathFile, sizePathFile);
			GetWindowTextA(hGlobalSizePolinom, sizePolinomText, numberSizePol);
			std::cout <<"Name file:\t"  << pathFile << std::endl;
			globalSizePolinom = std::atoi(sizePolinomText);
			std::cout << "size polinom =\t" << globalSizePolinom << std::endl;
				exFile.read(pathFile);


				dataOut = exFile.getVecDataOut();

				for (int i = 0; i < 96; i++)
				{

					s << dataOut[i][12];
					s >> pos_y.data()[i];
					s.clear();
				}
				min = max = pos_y.data()[0];
				for (int i = 1; i < 96; i++)
				{
					if (pos_y.data()[i] > max)
						max = pos_y.data()[i];
					if (pos_y.data()[i] < min)
						min = pos_y.data()[i];
				}


				for (int i = 0; i < 96; i++)
				{
					pos_y.data()[i] = -1.0 + pos_y.data()[i] * (2.0 / (max - min));
					pos_x.data()[i] = -1.0 + (double)i * (1.0 / 48.0);

				}


				appP.setSizePolinom(globalSizePolinom);
				

			
				appP.Matrix_Transform(pos_x.data(), pos_y.data(), pos_x.size());
				appP.Matrix_LU();
				

				activePolinom = true;
			
			break;
		case PATH_SAVE_FILE:
			appP.SaveTextPolinom("Result Polinom.txt");
			break;
		case EXIT:
			
			PostQuitMessage(0);
			break;
	
		default:
			break;
		}

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
LRESULT CALLBACK WndChild(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		InitializeOpenGL(hDC, hWnd);
		ReleaseDC(hWnd, hDC);
		hRC = wglCreateContext(hDC);

		wglMakeCurrent(hDC, hRC);

		SetTimer(hWnd, 1, 16, NULL);
		return 0;
	case WM_PAINT:
		glClear(GL_COLOR_BUFFER_BIT);
	
		BeginPaint(hWnd, &ps);
		{

			DrawTreangles(pos_x.data(), pos_y.data(), pos_x.size());
			DrawPolinom(appP, 0.0 + (double)redPos/255.0, 0.0 + (double)greenPos/255.0, 0.0 + (double)bluePos/255.0);
		}
		
		EndPaint(hWnd, &ps);
		SwapBuffers(hDC);

		ValidateRect(hWnd, NULL);
		return 0;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 101);
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}


void DrawTreangles(double* input_x, double* input_y, uint32_t sizeInput)
{
	glColor3d(1.0, 0.0, 1.0);
	glBegin(GL_TRIANGLES);
	
	for (int i = 0; i < sizeInput; i++)
	{
		glVertex3d(input_x[i] - 0.0125, input_y[i], 0);
		glVertex3d(input_x[i], input_y[i] + 0.025, 0);
		glVertex3d(input_x[i] + 0.0125, input_y[i], 0);
	}

	glEnd();
}

void DrawPolinom( ApproximationPolinom<double>& app, double red, double green, double blue)
{
	glColor3d(red, green, blue);
	uint32_t density = 200;
	double dt = 2 / (double)density;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < density; i++)
		glVertex3d(-1.0 +(double)i * dt, app.Polinom(-1.0 + (double)i * dt),0);
	glEnd();
}

void InitializeOpenGL(HDC& hDC, HWND& hWnd)
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

HWND CreateAHorizontalScrollBar(HWND hParentWnd, int sbHeight)
{
	if (!GetClientRect(hParentWnd, &rectRed))
		return NULL;
	return(CreateWindowEx(
		0,                      // no extended styles 
		L"SCROLLBAR",           // scroll bar control class 
		(PTSTR)NULL,           // no window text 
		WS_CHILD | WS_VISIBLE   // window styles  
		| SBS_HORZ,         // horizontal scroll bar style 
		rectRed.left,              // horizontal position 
		rectRed.bottom - sbHeight, // vertical position 
		rectRed.right,             // width of the scroll bar 
		sbHeight,               // height of the scroll bar
		hParentWnd,             // handle to main window 
		(HMENU)NULL,           // no menu 
		GetModuleHandle(nullptr),                // instance owning this window 
		(PVOID)NULL            // pointer not needed 
	));
}


