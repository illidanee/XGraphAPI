#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#include "./Src/CommonType.h"
#include "./Src/XTimer.h"
#include "./Src/XImage.h"
#include "./Src/XRaster.h"

//Window Size
const int _gWindowWidth = 1200;
const int _gWindowHeight = 900;

//Callback Function
LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//Main Function
int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//Register Window
	WNDCLASSEX wc;
	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = 0;
	wc.hCursor = 0;
	wc.hIcon = 0;
	wc.hIconSm = 0;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = TEXT("WINAPI");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	RegisterClassEx(&wc);

	//Adjust and Create Window
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT rect = { (screenWidth - _gWindowWidth) / 2, (screenHeight - _gWindowHeight) / 2, (screenWidth - _gWindowWidth) / 2 + _gWindowWidth, (screenHeight - _gWindowHeight) / 2 + _gWindowHeight };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
	HWND hWnd = CreateWindowEx(0, TEXT("WINAPI"), TEXT("illidan"), WS_OVERLAPPEDWINDOW, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//Create and Show BMP
	HDC hDC = GetDC(hWnd);
	HDC hMem = CreateCompatibleDC(hDC);

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = _gWindowWidth;
	bmpInfo.bmiHeader.biHeight = _gWindowHeight;//bmp原点在左下，窗口原点在左上，取反后bmp远点和窗口远点一致为左上。
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;

	void* pBuffer = NULL;
	HBITMAP hBmp = CreateDIBSection(hMem, &bmpInfo, DIB_RGB_COLORS, (void**)&pBuffer, 0, 0);
	SelectObject(hMem, hBmp);

	Smile::XTimer timer;
	Smile::XImage* pImage = Smile::XImage::LoadFromFile("../Resources/bgWhite.png");
	Smile::XImage* pImageTrain = Smile::XImage::LoadFromFile("../Resources/train.png");
	Smile::XImage* pImageGrass = Smile::XImage::LoadFromFile("../Resources/grass.png");

	//Msg Loop
	MSG msg = {};
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Draw
		Smile::XRaster raster(pBuffer, _gWindowWidth, _gWindowHeight);		raster.Clean();
		//Timer Begin
		timer.Begin();

		//Draw Image
		raster.DrawImage(0, 0, pImage);
		
		Smile::XRaster::TriangleParam triangle1 =
		{
			Smile::Vec2f(300, 300),
			Smile::Vec2f(0.0f, 0.0f),
			Smile::BGRA8U(0, 0, 0, 0),

			Smile::Vec2f(812, 300),
			Smile::Vec2f(1.0f, 0.0f),
			Smile::BGRA8U(0, 0, 0, 0),

			Smile::Vec2f(812, 812),
			Smile::Vec2f(1.0f, 1.0f),
			Smile::BGRA8U(0, 0, 0, 0),
		};

		Smile::XRaster::TriangleParam triangle2 =
		{
			Smile::Vec2f(300, 300),
			Smile::Vec2f(0.0f, 0.0f),
			Smile::BGRA8U(0, 0, 0, 0),

			Smile::Vec2f(812, 812),
			Smile::Vec2f(1.0f, 1.0f),
			Smile::BGRA8U(0, 0, 0, 0),

			Smile::Vec2f(300, 812),
			Smile::Vec2f(0.0f, 1.0f),
			Smile::BGRA8U(0, 0, 0, 0),
		};

		raster.DrawTriangle(triangle1, pImageGrass);
		raster.DrawTriangle(triangle2, pImageGrass);

		//Timer End
		timer.End();
		char timeBuffer[256] = {0};
		sprintf(timeBuffer, "time: %f ns", timer.GetElapsedTime());
		TextOutA(hMem, 10, 10, timeBuffer, strlen(timeBuffer));
	
		//Render to DC
		BitBlt(hDC, 0, 0, _gWindowWidth, _gWindowHeight, hMem, 0, 0, SRCCOPY);
	}

	delete pImage;
	delete pImageTrain;
	delete pImageGrass;

	return 0;
}