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
			Vec2f _start;
			Vec2f _startUV;
			BGRA8U _startColor;
			Vec2f _end;
			Vec2f _endUV;
			BGRA8U _endColor;
			
		public:
			_SpanParam(Vec2f start, Vec2f startUV, BGRA8U startColor, Vec2f end, Vec2f endUV, BGRA8U endColor)
			{
				if (start._x <= end._x)
				{
					_start = start;
					_startUV = startUV;
					_startColor = startColor;

					_end = end;
					_endUV = endUV;
					_endColor = endColor;
				}
				else
				{
					_start = end;
					_startUV = endUV;
					_startColor = endColor;

					_end = start;
					_endUV = startUV;
					_endColor = startColor;
				}
			}
		};

		typedef _SpanParam SpanParam;

		class _EdgeParam
		{
		public:
			Vec2f _pos1;
			Vec2f _uv1;
			BGRA8U _color1;

			Vec2f _pos2;
			Vec2f _uv2;
			BGRA8U _color2;

		public:
			_EdgeParam(Vec2f pos1, Vec2f uv1, BGRA8U color1, Vec2f pos2, Vec2f uv2, BGRA8U color2)
			{
				if (pos1._y <= pos2._y)
				{
					_pos1 = pos1;
					_uv1 = uv1;
					_color1 = color1;

					_pos2 = pos2;
					_uv2 = uv2;
					_color2 = color2;
				}
				else
				{
					_pos1 = pos2;
					_uv1 = uv2;
					_color1 = color2;

					_pos2 = pos1;
					_uv2 = uv1;
					_color2 = color1;
				}
			}
		};

		typedef _EdgeParam EdgeParam;
		
	public:
		class _TriangleParam
		{
		public:
			Vec2f _pos1;
			Vec2f _uv1;
			BGRA8U _color1;

			Vec2f _pos2;
			Vec2f _uv2;
			BGRA8U _color2;

			Vec2f _pos3;
			Vec2f _uv3;
			BGRA8U _color3;

		public:
			_TriangleParam(Vec2f pos1, Vec2f uv1, BGRA8U color1, Vec2f pos2, Vec2f uv2, BGRA8U color2, Vec2f pos3, Vec2f uv3, BGRA8U color3)
			{
				_pos1 = pos1;
				_uv1 = uv1;
				_color1 = color1;

				_pos2 = pos2;
				_uv2 = uv2;
				_color2 = color2;

				_pos3 = pos3;
				_uv3 = uv3;
				_color3 = color3;
			}
		};

		typedef _TriangleParam TriangleParam;

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
		void DrawTriangle(const TriangleParam& triangle, XImage* pImage);

		void DrawImage(float x, float y, float w, float h);
		void DrawImage(float x, float y, XImage* pImage);
		void DrawImageWithColorKey(float x, float y, XImage* pImage, BGRA8U colorKey);
		void DrawImageWithAlphaTest(float x, float y, XImage* pImage, unsigned char alpha);
		void DrawImageWithAlphaBlend(float x, float y, XImage* pImage);
		void DrawImageWithAlphaBlendAndTransparent(float x, float y, XImage* pImage, float alpha);
		void DrawImageWithTransparent(float x, float y, XImage* pImage, float alpha);
		void DrawImageWithSize(float x, float y, XImage* pImage, float imageX, float imageY, float imageWidth, float imageHeight);
		void DrawImageWithScale(float x, float y, float w, float h, XImage* pImage);

	private:
		inline BGRA8U _GetPix(unsigned int x, unsigned int y)
		{
			return _pBuffer[y * _w + x];
		}

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

		inline Vec2f _LerpUV(Vec2f uv1, Vec2f uv2, float lerp)
		{
			Vec2f uv(0, 0);
			uv._u = uv1._u + (uv2._u - uv1._u) * lerp;
			uv._v = uv1._v + (uv2._v - uv1._v) * lerp;
			return uv;
		}

		void _DrawSpan(const SpanParam& spanParam, XImage* pImage);
		void _DrawTrianglePart(const EdgeParam& e1, const EdgeParam& e2, XImage* pImage);

	private:
		BGRA8U BGRA8U_RED;

		unsigned int* _pBuffer;
		unsigned int _w;
		unsigned int _h;
	};
}