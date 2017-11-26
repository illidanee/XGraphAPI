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

		//三角形参数
		class _SpanParam
		{
		public:
			float _xStart;
			float _xEndl;
			float _y;
			BGRA8U _xStartColor;
			BGRA8U _xEndColor;
		public:
			_SpanParam(float xStart, float xEnd, float y, BGRA8U xStartColor, BGRA8U xEndColor)
			{
				if (xStart <= xEnd)
				{
					_xStart = xStart;
					_xEndl = xEnd;
					_y = y;
					_xStartColor = xStartColor;
					_xEndColor = xEndColor;
				}
				else
				{
					_xStart = xEnd;
					_xEndl = xStart;
					_y = y;
					_xStartColor = xEndColor;
					_xEndColor = xStartColor;
				}
			}
		};

		typedef _SpanParam SpanParam;

		class _Edge
		{
		public:
			float _x1;
			float _y1;
			float _x2;
			float _y2;
		public:
			_Edge(float x1, float y1, float x2, float y2)
			{
				if (y1 <= y2)
				{
					_x1 = x1;
					_y1 = y1;
					_x2 = x2;
					_y2 = y2;
				}
				else
				{
					_x1 = x2;
					_y1 = y2;
					_x2 = x1;
					_y2 = y1;
				}
			}
		};

		typedef _Edge Edge;

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

		void DrawSpan(const SpanParam& spanParam);
		void DrawTrianglePart(const Edge& e1, const Edge& e2);
		void DrawTriangle(Vec2f pos1, Vec2f pos2, Vec2f pos3);

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
			color._b = color1._b + (color2._b - color1._b) * lerp;
			color._g = color1._g + (color2._g - color1._g) * lerp;
			color._r = color1._r + (color2._r - color1._r) * lerp;
			color._a = color1._a + (color2._a - color1._a) * lerp;
			return color;
		}

	private:
		BGRA8U BGRA8U_RED;

		BGRA8U* _pBuffer;
		unsigned int _w;
		unsigned int _h;
	};

}