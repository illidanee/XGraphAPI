#include <stdio.h>
#include <complex>
#include <tchar.h>
#include <windows.h>
#include <Windowsx.h>

#include "./Src/CommonType.h"
#include "./Src/XImage.h"
#include "./Src/XTimer.h"
#include "./Src/XCamera.h"
#include "./Src/XRaster.h"

//Window Size
const int _gWindowWidth = 1200;
const int _gWindowHeight = 900;

//Camera
Smile::XCamera _gCamera;

Smile::XVec2f _gLButtonLastPosition;
Smile::XVec2f _gRButtonLastPosition;

bool _gLButtonFlag = false;
bool _gRButtonFlag = false;

//Timer
Smile::XTimer timer;

Smile::XVec3f CalcIntersectPoint(Smile::XRayf& ray)
{
	Smile::XVec3f origin = ray.GetOrigin();
	Smile::XVec3f dir = ray.GetDir();
	float time = abs(origin._y) / abs(dir._y);
	Smile::XVec3f point = ray.GetPoint(time);
	return Smile::XVec3f(point._x, 0.0f, point._z);
}

//Callback Function
LRESULT CALLBACK WindowProc(_In_ HWND   hwnd, _In_ UINT   uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	{
		_gLButtonLastPosition = Smile::XVec2f(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		_gLButtonFlag = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		_gLButtonFlag = false;
		break;
	}
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	{
		Smile::XVec2f pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		if (_gLButtonFlag)
		{
			Smile::XRayf ray0 = _gCamera.CreateRayFromScreen(pos);
			Smile::XRayf ray1 = _gCamera.CreateRayFromScreen(_gLButtonLastPosition);
			
			Smile::XVec3f pos0 = CalcIntersectPoint(ray0);
			Smile::XVec3f pos1 = CalcIntersectPoint(ray1);

			Smile::XVec3f offset = pos0 - pos1;

			Smile::XVec3f eye = _gCamera.GetEye() + offset;
			Smile::XVec3f aim = _gCamera.GetAim() + offset;

			_gCamera.SetEye(eye);
			_gCamera.SetAim(aim);
			_gCamera.Update();

			_gLButtonLastPosition = pos;
		}
		break;
	}
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
	bmpInfo.bmiHeader.biHeight = _gWindowHeight;//bmpԭ�������£�����ԭ�������ϣ�ȡ����bmpԶ��ʹ���Զ��һ��Ϊ���ϡ�
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

	//�������
	Smile::XMat4f modelMatrix = Smile::XMat4f();
	Smile::XMat4f projectMatrix = Smile::Perspective<float>(90.0f, (float)_gWindowWidth / _gWindowHeight, 0.01f, 30.0f);
	_gCamera.Init();
	_gCamera.SetMMatrix(modelMatrix);
	_gCamera.SetPMatrix(projectMatrix);
	_gCamera.SetViewport(Smile::XVec2f(_gWindowWidth, _gWindowHeight));

	//������Ⱦ��
	Smile::XRaster raster(pBuffer, _gWindowWidth, _gWindowHeight);

	//����ͼƬ
	Smile::XImage* pImage = Smile::XImage::LoadFromFile("../Resources/floor.jpg");
	pImage->SetWrapType(Smile::XImage::_WT_REPEAT);

	//��������
	struct DATA 
	{
		Smile::XVec3f pos;
		Smile::XVec2f uv;
		Smile::BGRA8U color;
	};

	DATA data[] = { 
		{ Smile::XVec3f(-15.0f, 0.0f, -15.0f), Smile::XVec2f(0.0f, 0.0f), Smile::BGRA8U(0, 0, 0, 0) },
		{ Smile::XVec3f(15.0f, 0.0f, -15.0f), Smile::XVec2f(100.0f, 0.0f), Smile::BGRA8U(0, 0, 0, 0) },
		{ Smile::XVec3f(15.0f, 0.0f, 15.0f), Smile::XVec2f(100.0f, 100.0f), Smile::BGRA8U(0, 0, 0, 0) },

		{ Smile::XVec3f(-15.0f, 0.0f, -15.0f), Smile::XVec2f(0.0f, 0.0f), Smile::BGRA8U(0, 0, 0, 0) },
		{ Smile::XVec3f(-15.0f, 0.0f, 15.0f), Smile::XVec2f(0.0f, 100.0f), Smile::BGRA8U(0, 0, 0, 0) },
		{ Smile::XVec3f(15.0f, 0.0f, 15.0f), Smile::XVec2f(100.0f, 100.0f), Smile::BGRA8U(0, 0, 0, 0) },
	};

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
		raster.Clean();
		raster.LoadMMatrix(_gCamera.GetMMatrix());
		raster.LoadVMatrix(_gCamera.GetVMatrix());
		raster.LoadPMatrix(_gCamera.GetPMatrix());

		raster.VertexPointer(2, Smile::XRaster::_DT_FLOAT, sizeof(DATA), &data[0].pos);
		raster.UVPointer(2, Smile::XRaster::_DT_FLOAT, sizeof(DATA), &data[0].uv);
		raster.ColorPointer(4, Smile::XRaster::_DT_UNSIGNEDCHAR, sizeof(DATA), &data[0].color);
		raster.BindTexture(pImage);

		//Timer Begin
		timer.Begin();

		//����
		raster.DrawArray(Smile::XRaster::_DRAWTRIANGLES, 0, 6);

		//Timer End
		timer.End();
		char timeBuffer[256] = {0};
		sprintf(timeBuffer, "time: %f ns", timer.GetElapsedTime());
		TextOutA(hMem, 10, 10, timeBuffer, strlen(timeBuffer));
	
		//Render to DC
		BitBlt(hDC, 0, 0, _gWindowWidth, _gWindowHeight, hMem, 0, 0, SRCCOPY);
	}

	delete pImage;

	return 0;
}