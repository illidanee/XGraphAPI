#pragma once


#include <memory.h>
#include <math.h>
#include "CommonType.h"
#include "XMath.h"
#include "XFrustum.h"
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
			_DRAWTRIANGLES = 5,
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
	
		//数据类型
		enum _DATATYPE
		{
			_DT_NONE = 0,
			_DT_CHAR = 1,
			_DT_UNSIGNEDCHAR = 2,
			_DT_FLOAT = 3,
			_DT_DOUBLE = 4,
		};

		typedef _DATATYPE DATATYPE;

	private:
		//三角形参数
		class _SpanParam
		{
		public:
			XVec2f _start;
			XVec2f _startUV;
			BGRA8U _startColor;
			XVec2f _end;
			XVec2f _endUV;
			BGRA8U _endColor;
			
		public:
			_SpanParam(XVec2f start, XVec2f startUV, BGRA8U startColor, XVec2f end, XVec2f endUV, BGRA8U endColor)
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
			XVec2f _pos1;
			XVec2f _uv1;
			BGRA8U _color1;

			XVec2f _pos2;
			XVec2f _uv2;
			BGRA8U _color2;

		public:
			_EdgeParam(XVec2f pos1, XVec2f uv1, BGRA8U color1, XVec2f pos2, XVec2f uv2, BGRA8U color2)
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
		
		class _TriangleParam
		{
		public:
			XVec2f _pos1;
			XVec2f _uv1;
			BGRA8U _color1;

			XVec2f _pos2;
			XVec2f _uv2;
			BGRA8U _color2;

			XVec2f _pos3;
			XVec2f _uv3;
			BGRA8U _color3;

		public:
			_TriangleParam(XVec2f pos1, XVec2f uv1, BGRA8U color1, XVec2f pos2, XVec2f uv2, BGRA8U color2, XVec2f pos3, XVec2f uv3, BGRA8U color3)
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
		void DrawPoint(XVec2f pos, BGRA8U color, POINTSIZE ps);
		void DrawLine(XVec2f pos1, XVec2f pos2, BGRA8U color);
		void DrawLine(XVec2f pos1, XVec2f pos2, BGRA8U color1, BGRA8U color2);
		void DrawArray(DRAWMODE drawMode, XVec2f* posArray, int len);
		void DrawSolidRect(float x, float y, float w, float h, BGRA8U color);
		void DrawColorRect(XVec2f* posArray, BGRA8U* colorArray);
		
		void DrawImage(float x, float y, float w, float h);
		void DrawImage(float x, float y, XImage* pImage);
		void DrawImageWithColorKey(float x, float y, XImage* pImage, BGRA8U colorKey);
		void DrawImageWithAlphaTest(float x, float y, XImage* pImage, unsigned char alpha);
		void DrawImageWithAlphaBlend(float x, float y, XImage* pImage);
		void DrawImageWithAlphaBlendAndTransparent(float x, float y, XImage* pImage, float alpha);
		void DrawImageWithTransparent(float x, float y, XImage* pImage, float alpha);
		void DrawImageWithSize(float x, float y, XImage* pImage, float imageX, float imageY, float imageWidth, float imageHeight);
		void DrawImageWithScale(float x, float y, float w, float h, XImage* pImage);

		//使用状态机模式
		void VertexPointer(int size, DATATYPE type, int stride, void* pData);
		void UVPointer(int size, DATATYPE type, int stride, void* pData);
		void ColorPointer(int size, DATATYPE type, int stride, void* pData);
		void BindTexture(XImage* pImage);
		void DrawArray(DRAWMODE drawMode, int start, int end);

		//矩阵操作
		void LoadMIdentity();
		void LoadMMatrix(XMat4f mMatrix);
		void LoadVIdentity();
		void LoadVMatrix(XMat4f vMatrix);
		void LoadPIdentity();
		void LoadPMatrix(XMat4f pMatrix);
		void LoadPVIdentity();
		void LoadPVMatrix(XMat4f pvMatrix);

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

		inline XVec2f _LerpUV(XVec2f uv1, XVec2f uv2, float lerp)
		{
			XVec2f uv(0, 0);
			uv._u = uv1._u + (uv2._u - uv1._u) * lerp;
			uv._v = uv1._v + (uv2._v - uv1._v) * lerp;
			return uv;
		}

		//绘制三角形
		void _DrawSpan(const SpanParam& spanParam, XImage* pImage);
		void _DrawTrianglePart(const EdgeParam& e1, const EdgeParam& e2, XImage* pImage);
		void _DrawTriangle(const TriangleParam& triangle, XImage* pImage);

		//固定管线
		XVec3f _Pipeline(XVec3f vector);

	private:	//基本数据
		BGRA8U BGRA8U_RED;

		unsigned int* _pBuffer;
		unsigned int _w;
		unsigned int _h;

	private:	//状态机数据
		//状态机内部结构
		class _Element
		{
		public:
			int _size;
			DATATYPE _type;
			int _stride;
			void* _pData;
		};

		typedef _Element Element;
		Element _vertex;
		Element _uv;
		Element _color;

		Element _uvDefault;
		Element _colorDefault;

		XVec2f _defaultUVArray[3];
		BGRA8U _defaultColorArray[3];

		XImage* _pImage;

		XMat4f _mMatrix;
		XMat4f _vMatrix;
		XMat4f _pMatrix;
		XMat4f _pvMatrix;

		XVec2f _viewport;

		XFrustumf _frustum;
	};
}