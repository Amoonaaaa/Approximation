#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")*/
#include<iostream>
#include<math.h>
#include"WinApiWindow.h"
#include"Renderer.h"
#include"ChildWindow.h"
#include"ImageLib.h"
#include"DirectoryPath.h"
#include"CoderFileMachinLearning.h"
#include<random>

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
LRESULT WINAPI  WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI WndChild(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI PathImageChildProc(HWND, UINT, WPARAM, LPARAM);
LRESULT WINAPI PathSaveNeuralNetworkProc(HWND, UINT, WPARAM, LPARAM);

ImageLib imageLib(0,0);
IWindowCore* windowCoreChild = nullptr;

IWindowCore* windowCorePathImage = nullptr;
IWindowCore* windowCoreNeuralNetwork = nullptr;
std::vector<GLfloat> ConvectorVector(std::vector<ColorInfo> vectorImage);
HWND hTextOpenFile;
HWND hEditOpenFile;
HWND hButton_OK;
HINSTANCE hInst = GetModuleHandle(nullptr);
char pathOpenFile[200];
Renderer renderer;
HWND hEditPath;
HWND hStaticText;

std::string pathDirectory;
DirectoryPath directoryPath(pathDirectory);
CoderFileMachineLearning fileMachineLearning;
HDC hDC;
PAINTSTRUCT ps;
HGLRC hGLRC;
std::vector<GLfloat> vecColor =
{ 0.8f, 0.8f, 0.8f,
	0.8f, 0.8f, 0.8f,
	0.8f, 0.8f, 0.8f,
	0.8f, 0.8f, 0.8f
};
std::vector<float> position =
{
	-1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
};

std::vector<float> txImage =
{
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f


};

std::vector<float> GenetationPosition(float width, float heinght);
std::vector<float> vec;

HMENU hMenuBar;
HMENU hFile;
HMENU hExit;
HMENU hSaveFile;
int main()
{
	try
	{
		//imageLib.Read("image2.bmp");
		//vecColor.resize(3 * 4);
		// vecColor = ConvectorVector(imageLib.GetColorInfo());
		// position.resize(2 * 10* 10);

			// position = GenetationPosition(10, 10);
		IWindowCore* windowCore = new WinApiWindow(WHITE_BRUSH, L"Class name", L"Jnana 1.0", 1024, 768, WndProc, 0, 0, hInst);
		windowCore->Initialize();
		windowCore->MainLoop();
	
	}
	catch (std::exception& excep)
	{
		std::cout << excep.what() << std::endl;
	}
}

std::vector<GLfloat> GenetationPosition(float width, float height)
{
	std::vector<float> gPos(2 * width * height);


	for (float i = 0; i < height; i++)
	{
		for (float j = 0; j < width; j++)
		{
			gPos[i * width + 2 * j] = -1.0f + 2.0f * (i / height);
			gPos[i * height + 2 * j + 1] = -1.0f + 2.0f * (j / width);
		}
	}
	return gPos;
}



GLuint animation;
HWND hWndImage;
HBITMAP image;
HDC wndDC;
HBITMAP hBmp;
HDC bmpDC;
#define EDIT_TEXT 1
#define BUTTON_INITIALIZE_IMAGE 2

std::vector<GLfloat> ConvectorVector(std::vector<ColorInfo> vectorImage)
{

	std::vector<GLfloat> vector_(3 * vectorImage.size());
	std::cout << "size pixel =\t" << vectorImage.size() << std::endl;
	std::cout << "Init!" << std::endl;
	for (int i = 0; i < vectorImage.size(); i++)
	{
		

		vector_[3 * i] = vectorImage[i].r;

		vector_[3 * i + 1] = vectorImage[i].g;

		vector_[3 * i + 2] = vectorImage[i].b;


		
	}
	std::cout << "Finish!" << std::endl;
	return vector_;
}
#define EXIT 3
#define PATH_OPEN_IMAGE 4
#define PATH_SAVE_FILE 5
LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:

	
		 windowCoreChild = new WinApiWindow(WHITE_BRUSH, L"Class name child", L"", 640, 480, WndChild, 0, 0, GetModuleHandle(nullptr),
			CS_VREDRAW | CS_HREDRAW | CS_OWNDC, WS_CHILD | WS_VISIBLE, hWnd);
		windowCoreChild->Initialize();
		hTextOpenFile = CreateWindow(L"static", L"Укажите путь к изображению:", WS_CHILD | WS_VISIBLE  , 20, 490, 400, 20,
			hWnd, NULL, hInst, NULL);
		ShowWindow(hTextOpenFile, SW_SHOWNORMAL);
		hEditOpenFile = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 20, 510, 400, 20,
			hWnd, reinterpret_cast<HMENU>(EDIT_TEXT), hInst, NULL);
		ShowWindow(hEditOpenFile, SW_SHOWNORMAL);
		hButton_OK = CreateWindow(L"button", L"Загрузка изображения", WS_CHILD | WS_VISIBLE  | BS_PUSHBUTTON,
			20, 550, 250, 25, 
			hWnd, reinterpret_cast<HMENU>(BUTTON_INITIALIZE_IMAGE), hInst, NULL);
		ShowWindow(hButton_OK, SW_SHOWNORMAL);

		hMenuBar = CreateMenu();
		hFile = CreateMenu();
		AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");

		AppendMenu(hFile, MF_STRING, PATH_OPEN_IMAGE, L"Open File Image");
		AppendMenu(hFile, MF_STRING, PATH_SAVE_FILE, L"Save is..");
		AppendMenu(hFile, MF_STRING, EXIT, L"Exit");
		SetMenu(hWnd, hMenuBar);

		SetTimer(hWnd, 1, 8, NULL);
		return 0;

	case WM_SIZE:
		SetWindowPos(hEditOpenFile, HWND_TOP, 2, 510, LOWORD(lp) - 370, 20, SWP_SHOWWINDOW);
		SetWindowPos(hTextOpenFile, HWND_TOP, 2, 490, LOWORD(lp) - 370, 20, SWP_SHOWWINDOW);
		return 0;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, false);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case EDIT_TEXT:
			GetWindowTextA(hEditOpenFile, pathOpenFile, 200);
			

			break;
		case BUTTON_INITIALIZE_IMAGE:
			imageLib.Read(pathOpenFile);
			vec = ConvectorVector(imageLib.GetColorInfo());
			renderer.CreateImage(vec, 640, 480);
			break;
		case EXIT:
		case WM_TIMER:
			InvalidateRect(hWnd, NULL, false);
			return 0;
			PostQuitMessage(0);
			break;
			case PATH_OPEN_IMAGE:
				windowCorePathImage = new WinApiWindow(WHITE_BRUSH, L"Class name path image", L"Path image", 480, 240, PathImageChildProc, 100,
					100, GetModuleHandle(nullptr), CS_VREDRAW | CS_HREDRAW , WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME, hWnd);
				windowCorePathImage->Initialize();
			break;
			case PATH_SAVE_FILE:
				windowCoreNeuralNetwork = new WinApiWindow(WHITE_BRUSH, L"Class name neural network", L"Path neural network", 480, 240, PathSaveNeuralNetworkProc, 150,
					100, GetModuleHandle(nullptr), CS_VREDRAW | CS_HREDRAW, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME, hWnd);
				windowCoreNeuralNetwork->Initialize();
				break;
		}
		return DefWindowProc(hWnd, msg, wp, lp);




	case WM_DESTROY:
		
		KillTimer(hWnd, 101);
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}


LRESULT WINAPI WndChild(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		renderer.Init(hDC, hWnd, hGLRC);
		gladLoadGL();
		renderer.GetVertShader("TestShader.vert");
		renderer.GetFragShader("TestShader.frag");

		
		renderer.CreateBuffers(position, vecColor, txImage);
		renderer.setInt("tex", 0);
		renderer.LinkShader();
	
		return 0;
	case WM_PAINT:
		//BeginPaint(hWnd, &ps);		
		glClear(GL_COLOR_BUFFER_BIT);
		//x = x + 0.1;
		//std::cout << x << std::endl;
		
		renderer.render(4);
	
		ValidateRect(hWnd, NULL);
		SwapBuffers(hDC);
		return 0;
	case WM_DESTROY:
		renderer.Clear();
		PostQuitMessage(0);
		return 0;
	default:

		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
HWND hTextOpen;
HWND hEditOpen;
#define PATH_EDIT_TEXT 6
std::vector<std::string> vecFile;
char pathImage[200];
HWND hButtonPath;
#define BUTTON_PATH_IMAGE 7
LRESULT WINAPI PathImageChildProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		hTextOpen = CreateWindow(L"static", L"Укажите путь к списку изображений:", WS_CHILD | WS_VISIBLE, 20, 100, 400, 20,
			hWnd, NULL, hInst, NULL);
		ShowWindow(hTextOpen, SW_SHOWNORMAL);
		hEditOpen = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 20, 120, 400, 20,
			hWnd, reinterpret_cast<HMENU>(PATH_EDIT_TEXT), hInst, NULL);
		ShowWindow(hEditOpen, SW_SHOWNORMAL);
		hButtonPath = CreateWindow(L"button", L"Загрузка изображения", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 140, 200, 20,
			hWnd, reinterpret_cast<HMENU>(BUTTON_PATH_IMAGE), hInst, NULL);
		ShowWindow(hButtonPath, SW_SHOWNORMAL);
		return 0;

	case WM_SIZE:

		return 0;
	case WM_COMMAND:

		switch (LOWORD(wp))
		{
		case PATH_EDIT_TEXT:
			GetWindowTextA(hEditOpen, pathImage, 200);
			
			break;
		case BUTTON_PATH_IMAGE:
			directoryPath.SetPath(pathImage);
			directoryPath.SetNumberFile();
			directoryPath.searchFile();
			vecFile = directoryPath.GetVecFile();

	
			break;
		}
		return 0;
	case WM_PAINT:

		return 0;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}
HWND hTexNeuralNetwork;
HWND hEditNeuralNetwork;
HWND hButtonSave;
HWND hButtonRead;
#define PATH_EDIT_TEXT_NEURAL_NETWORK 7
#define PATH_BUTTON_SAVE 8
#define PATH_BUTTON_READ 9
char pathNeuralNetwork[200];
std::vector<unsigned int> ArraySizeKnot{ 10,2};
std::vector<float> coef(20);
LRESULT WINAPI PathSaveNeuralNetworkProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		hTexNeuralNetwork = CreateWindow(L"static", L"Укажите путь к сохранению или чтению файла:", WS_CHILD | WS_VISIBLE, 20, 100, 400, 20,
			hWnd, NULL, hInst, NULL);
		ShowWindow(hTexNeuralNetwork, SW_SHOWNORMAL);

		hEditNeuralNetwork = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 20, 120, 400, 20,
			hWnd, reinterpret_cast<HMENU>(PATH_EDIT_TEXT_NEURAL_NETWORK), hInst, NULL);
		ShowWindow(hEditNeuralNetwork, SW_SHOWNORMAL);

		hButtonSave = CreateWindow(L"button", L"сохранить", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20, 140, 140, 20,
			hWnd, reinterpret_cast<HMENU>(PATH_BUTTON_SAVE), hInst, NULL);
		ShowWindow(hButtonSave, SW_SHOWNORMAL);

		hButtonRead = CreateWindow(L"button", L"читать", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			165, 140, 140, 20,
			hWnd, reinterpret_cast<HMENU>(PATH_BUTTON_READ), hInst, NULL);
		ShowWindow(hButtonRead, SW_SHOWNORMAL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wp))
		{
		case PATH_EDIT_TEXT_NEURAL_NETWORK:
			GetWindowTextA(hEditNeuralNetwork, pathNeuralNetwork, 200);
			break;
		case PATH_BUTTON_SAVE:
			fileMachineLearning.SetArraySizeKnot(ArraySizeKnot);
			fileMachineLearning.SetSizeLayer(2);
			for (int i = 0; i < 20; i++)
			{
				coef[i] = rand() % 20 - 10;
				std::cout << coef[i] << std::endl;
			}
			fileMachineLearning.SetCoefficients(coef);
			fileMachineLearning.Export(pathNeuralNetwork);
			break;
		case PATH_BUTTON_READ:
			fileMachineLearning.Read(pathNeuralNetwork);
			break;
		}
		return 0;
	default:

		return DefWindowProc(hWnd, msg, wp, lp);
	}
}