#pragma once


#include <memory.h>
#include <math.h>
#include "CommonType.h"
#include "XImage.h"

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
	
	private:
		//三角形参数
		class _SpanParam
		{
		public:
			float _xStart;
			BGRA8U _xStartColor;
			float _xEndl;
			BGRA8U _xEndColor;
			float _y;
			
		public:
			_SpanParam(float xStart, BGRA8U xStartColor, float xEnd, BGRA8U xEndColor, float y)
			{
				if (xStart <= xEnd)
				{
					_xStart = xStart;
					_xStartColor = xStartColor;
					_xEndl = xEnd;
					_xEndColor = xEndColor;
					_y = y;
				}
				else
				{
					_xStart = xEnd;
					_xStartColor = xEndColor;
					_xEndl = xStart;
					_xEndColor = xStartColor;
					_y = y;
				}
			}
		};

		typedef _SpanParam SpanParam;

		class _EdgeParam
		{
		public:
			float _x1;
			float _y1;
			BGRA8U _color1;
			float _x2;
			float _y2;
			BGRA8U _color2;
		public:
			_EdgeParam(float x1, float y1, BGRA8U color1, float x2, float y2, BGRA8U color2)
			{
				if (y1 <= y2)
				{
					_x1 = x1;
					_y1 = y1;
					_color1 = color1;
					_x2 = x2;
					_y2 = y2;
					_color2 = color2;
				}
				else
				{
					_x1 = x2;
					_y1 = y2;
					_color1 = color2;
					_x2 = x1;
					_y2 = y1;
					_color2 = color1;
				}
			}
		};

		typedef _EdgeParam EdgeParam;

	public:
		XRaster(void* pBuffer, unsigned int w, unsigned int h);
		~XRaster();

		void Clean();
		void DrawPoint(float x, float y, BGRA8U color, POINTSIZE ps);
		void DrawPoint(Vec2f pos, BGRA8U color, POINTSIZE ps);
		void DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color);
		void DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color1, BGRA8U color2);
		void DrawArray(DRAWMODE drawMode, Vec2f* posArray, int len);
		void DrawSolidRect(float x, float y, float w, float h, BGRA8U color);
		void DrawColorRect(Vec2f* posArray, BGRA8U* colorArray);
		void DrawTriangle(Vec2f pos1, BGRA8U color1, Vec2f pos2, BGRA8U color2, Vec2f pos3, BGRA8U color3);
		
		void DrawImage(float x, float y, float w, float h);
		void DrawImage(float x, float y, XImage* pImage);
		void DrawImageWithColorKey(float x, float y, XImage* pImage, BGRA8U colorKey);

	private:
		inline void _SetPix(unsigned int x, unsigned int y, BGRA8U color)
		{
			_pBuffer[y * _w + x] = color._color;
		}

		inline void _SetPixSafe(unsigned int x, unsigned int y, BGRA8U color)
		{
			if (x >= _w || y >= _h)
				return;

			_pBuffer[y * _w + x] = color._color;
		}

		inline BGRA8U _LerpColor(BGRA8U color1, BGRA8U color2, float lerp)
		{
			BGRA8U color(0, 0, 0, 0);
			color._b = color1._b + (color2._b - color1._b) * lerp;
			color._g = color1._g + (color2._g - color1._g) * lerp;
			color._r = color1._r + (color2._r - color1._r) * lerp;
			color._a = color1._a + (color2._a - color1._a) * lerp;
			return color;
		}

		void _DrawSpan(const SpanParam& spanParam);
		void _DrawTrianglePart(const EdgeParam& e1, const EdgeParam& e2);

	private:
		BGRA8U BGRA8U_RED;

		unsigned int* _pBuffer;
		unsigned int _w;
		unsigned int _h;
	};
}