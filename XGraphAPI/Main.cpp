#include <tchar.h>
#include <windows.h>

#include "./Src/CommonType.h"
#include "./Src/Raster.h"

//Window Size
const int _gWindowWidth = 800;
const int _gWindowHeight = 600;

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
	bmpInfo.bmiHeader.biHeight = _gWindowHeight;
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
		////Draw Rectangle
		//Smile::Vec2f rectangle[] = { Smile::Vec2f(100, 100), Smile::Vec2f(700, 100), Smile::Vec2f(700, 500), Smile::Vec2f(100, 500) };
		//raster.DrawArray(Smile::XRaster::_DRAWLINELOOP, rectangle, sizeof(rectangle) / sizeof(Smile::Vec2f));

		////Draw Circle
		//Smile::Vec2f center(400.0f, 300.0f);
		//Smile::Vec2f circle[360];
		//for (int i = 0; i < 360; ++i)
		//{
		//	circle[i] = Smile::Vec2f(cos(ANGLE2RADIAN(i)) * 200 + center._x, sin(ANGLE2RADIAN(i)) * 200 + center._y);
		//}
		//raster.DrawArray(Smile::XRaster::_DRAWLINELOOP, circle, sizeof(circle) / sizeof(Smile::Vec2f));

		//Draw Bessel
		Smile::Vec2f controls[4] = { Smile::Vec2f(100, 100), Smile::Vec2f(150, 300), Smile::Vec2f(650, 300), Smile::Vec2f(700, 500) };
		Smile::Vec2f bessel[2];
		for (float t = 0.0f; t < 1.0f; t += 0.01f)
		{
			float x = controls[0]._x * pow(1 - t, 3) + controls[1]._x * 3 * pow(1 - t, 2) * t + controls[2]._x * 3 * (1 - t) * pow(t, 2) + controls[3]._x * pow(t, 3);
			float y = controls[0]._y * pow(1 - t, 3) + controls[1]._y * 3 * pow(1 - t, 2) * t + controls[2]._y * 3 * (1 - t) * pow(t, 2) + controls[3]._y * pow(t, 3);
			if (t == 0.0f)
			{
				bessel[0] = Smile::Vec2f(x, y);
			}
			else
			{
				bessel[1] = Smile::Vec2f(x, y);
				raster.DrawArray(Smile::XRaster::_DRAWLINES, bessel, sizeof(bessel) / sizeof(Smile::Vec2f));
				bessel[0] = Smile::Vec2f(x, y);
			}
		}

		BitBlt(hDC, 0, 0, _gWindowWidth, _gWindowHeight, hMem, 0, 0, SRCCOPY);
	}

	return 0;
}