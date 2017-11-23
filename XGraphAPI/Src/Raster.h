#pragma once


#include <memory.h>
#include <math.h>
#include "CommonType.h"


namespace Smile 
{

	class XRaster
	{
	public:
		//绘制模式
		enum _DRAWMODE
		{
			_NONEDRAWMODE = 0,
			_DRAWPOINT = 1,
			_DRAWLINES = 2,
			_DRAWLINESTRIP = 3,
			_DRAWLINELOOP = 4,
		};

		typedef _DRAWMODE DRAWMODE;

		//点的大小
		enum _POINTSIZE
		{
			_NONEPOINTSIZE = 0,
			_MINSIZE = 1,
			_MIDSIZE = 2,
			_MAXSIZE = 3,
		};

		typedef _POINTSIZE POINTSIZE;

	public:
		XRaster(void* pBuffer, unsigned int w, unsigned int h);
		~XRaster();

		void Clean();
		void DrawPoint(int x, int y, BGRA8U color, POINTSIZE ps);
		void DrawPoint(Vec2f pos, BGRA8U color, POINTSIZE ps);
		void DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color);
		void DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color1, BGRA8U color2);
		void DrawArray(DRAWMODE drawMode, Vec2f* posArray, int len);
		void DrawSolidRect(int x, int y, int w, int h, BGRA8U color);

	private:
		inline void _SetPix(unsigned int x, unsigned int y, BGRA8U color)
		{
			_pBuffer[y * _w + x] = color;
		}

		inline void _SetPixSafe(unsigned int x, unsigned int y, BGRA8U color)
		{
			if (x >= _w || y >= _h)
				return;

			_pBuffer[y * _w + x] = color;
		}

		inline BGRA8U _LerpColor(BGRA8U color1, BGRA8U color2, float lerp)
		{
			BGRA8U color(0, 0, 0, 0);
			color._b = color1._b + (color1._b - color2._b) * lerp;
			color._g = color1._g + (color1._g - color2._g) * lerp;
			color._r = color1._r + (color1._r - color2._r) * lerp;
			color._a = color1._a + (color1._a - color2._a) * lerp;
			return color;
		}

	private:
		BGRA8U BGRA8U_RED;

		BGRA8U* _pBuffer;
		unsigned int _w;
		unsigned int _h;
	};

}