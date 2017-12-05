#include "XRaster.h"

#include <algorithm>

namespace Smile 
{

	XRaster::XRaster(void* pBuffer, unsigned int w, unsigned int h)
		:
		BGRA8U_RED(0, 0, 255,255)
	{
		_pBuffer = (unsigned int*)pBuffer;
		_w = w;
		_h = h;
	}

	XRaster::~XRaster()
	{

	}

	void XRaster::Clean()
	{
		memset(_pBuffer, 0, _w * _h * sizeof(BGRA8U));
	}

	void XRaster::DrawPoint(float x, float y, BGRA8U color, POINTSIZE ps)
	{
		switch (ps)
		{
		case _MINSIZE:
		{
			_SetPixSafe(x, y, color);
		}
			break;
		case _MIDSIZE:
		{
			_SetPixSafe(x + 0, y + 0, color);
			_SetPixSafe(x + 1, y + 0, color);
			_SetPixSafe(x + 0, y + 1, color);
			_SetPixSafe(x + 1, y + 1, color);
		}
			break;
		case _MAXSIZE:
		{
			_SetPixSafe(x - 1, y - 1, color);
			_SetPixSafe(x + 0, y - 1, color);
			_SetPixSafe(x + 1, y - 1, color);

			_SetPixSafe(x - 1, y + 0, color);
			_SetPixSafe(x + 0, y + 0, color);
			_SetPixSafe(x + 1, y + 0, color);

			_SetPixSafe(x - 1, y + 1, color);
			_SetPixSafe(x + 0, y + 1, color);
			_SetPixSafe(x + 1, y + 1, color);
		}
			break;
		default:
			_SetPixSafe(x, y, color);
		}
	}

	void XRaster::DrawPoint(Vec2f pos, BGRA8U color, POINTSIZE ps)
	{
		DrawPoint(pos._x, pos._y, color, ps);
	}

	void XRaster::DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color)
	{
		float xOffset = pos1._x - pos2._x;
		float yOffset = pos1._y - pos2._y;
		
		if (xOffset == 0 && yOffset == 0)
		{
			_SetPixSafe(pos1._x, pos1._y, color);
		}
		//else if (xOffset == 0)
		//{
		//	float yMin = pos1._y < pos2._y ? pos1._y : pos2._y;
		//	float yMax = pos1._y > pos2._y ? pos1._y : pos2._y;

		//	for (float y = yMin; y <= yMax; y += 1.0f)
		//	{
		//		_SetPix(pos2._x, y, color);
		//	}
		//}
		//else if (yOffset == 0)
		//{
		//	float xMin = pos1._x < pos2._x ? pos1._x : pos2._x;
		//	float xMax = pos1._x > pos2._x ? pos1._x : pos2._x;

		//	for (float x = xMin; x <= xMax; x += 1.0f)
		//	{
		//		_SetPix(x, pos2._y, color);
		//	}
		//}
		//else
		//{
			if (fabs(xOffset) > fabs(yOffset))
			{
				float xMin = pos1._x < pos2._x ? pos1._x : pos2._x;
				float xMax = pos1._x > pos2._x ? pos1._x : pos2._x;

				float slope = yOffset / xOffset;
				for (float x = xMin; x < xMax; x += 1.0f)
				{
					float y = pos1._y + (x - pos1._x) * slope;
					_SetPixSafe(x, y, color);
				}
			}
			else
			{
				float yMin = pos1._y < pos2._y ? pos1._y : pos2._y;
				float yMax = pos1._y > pos2._y ? pos1._y : pos2._y;

				float slope = xOffset / yOffset;
				for (float y = yMin; y < yMax; y += 1.0f)
				{
					float x = pos1._x + (y - pos1._y) * slope;
					_SetPixSafe(x, y, color);
				}
			}
		//}
	}

	void XRaster::DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color1, BGRA8U color2)
	{
		float xOffset = pos1._x - pos2._x;
		float yOffset = pos1._y - pos2._y;

		if (xOffset == 0 && yOffset == 0)
		{
			_SetPixSafe(pos1._x, pos1._y, _LerpColor(color1, color2, 0.5f));
		}

		if (fabs(xOffset) > fabs(yOffset))
		{
			float xMin = pos1._x < pos2._x ? pos1._x : pos2._x;
			float xMax = pos1._x > pos2._x ? pos1._x : pos2._x;

			float slope = yOffset / xOffset;
			for (float x = xMin; x < xMax; x += 1.0f)
			{
				float lerp = (x - xMin) / (xMax - xMin);
				float y = pos1._y + (x - pos1._x) * slope;
				_SetPixSafe(x, y, _LerpColor(color1, color2, lerp));
			}
		}
		else
		{
			float yMin = pos1._y < pos2._y ? pos1._y : pos2._y;
			float yMax = pos1._y > pos2._y ? pos1._y : pos2._y;

			float slope = xOffset / yOffset;
			for (float y = yMin; y < yMax; y += 1.0f)
			{
				float lerp = (y - yMin) / (yMax - yMin);
				float x = pos1._x + (y - pos1._y) * slope;
				_SetPixSafe(x, y, _LerpColor(color1, color2, lerp));
			}
		}
	}

	void XRaster::DrawArray(DRAWMODE drawMode, Vec2f* posArray, int len)
	{
		switch (drawMode)
		{
		case _DRAWPOINT:
		{
			for (int i = 0; i < len; ++i)
			{
				DrawPoint(posArray[i], BGRA8U_RED, _MINSIZE);
			}
			break;
		}
		case _DRAWLINES:
		{
			len = len / 2;
			for (int i = 0; i < len; ++i)
			{
				DrawLine(posArray[2 * i], posArray[2 * i + 1], BGRA8U_RED);
			}
			break;
		}
		case _DRAWLINESTRIP:
		{
			for (int i = 0; i < len - 1; ++i)
			{
				DrawLine(posArray[i], posArray[i + 1], BGRA8U_RED);
			}
			break;
		}
		case _DRAWLINELOOP:
		{
			for (int i = 0; i < len - 1; ++i)
			{
				DrawLine(posArray[i], posArray[i + 1], BGRA8U_RED);
			}
			DrawLine(posArray[0], posArray[len - 1], BGRA8U_RED);
			break;
		}
		}
	}

	void XRaster::DrawSolidRect(float x, float y, float w, float h, BGRA8U color)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + w, _w);
		float top = std::min<float>(y + h, _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawColorRect(Vec2f* posArray, BGRA8U* colorArray)
	{
		float left = std::max<float>(posArray[0]._x, 0);
		float bottom = std::max<float>(posArray[0]._y, 0);
		float right = std::min<float>(posArray[2]._x, _w);
		float top = std::min<float>(posArray[2]._y, _h);

		float w = right - left;
		float h = top - bottom;
		for (float x = left; x < right; ++x)
		{
			BGRA8U color1 = _LerpColor(colorArray[0], colorArray[1], (x - left) / w);
			BGRA8U color2 = _LerpColor(colorArray[3], colorArray[2], (x - left) / w);

			for (float y = bottom; y < top; ++y)
			{
				BGRA8U color = _LerpColor(color1, color2, (y - bottom) / h);
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawTriangle(const TriangleParam& triangle, XImage* pImage)
	{
		EdgeParam edges[3] = { 
			EdgeParam(triangle._pos1, triangle._uv1, triangle._color1, triangle._pos2, triangle._uv2, triangle._color2),
			EdgeParam(triangle._pos2, triangle._uv2, triangle._color2, triangle._pos3, triangle._uv3, triangle._color3),
			EdgeParam(triangle._pos3, triangle._uv3, triangle._color3, triangle._pos1, triangle._uv1, triangle._color1) };

		int maxIndex = 0;
		float maxLength = edges[0]._pos2._y - edges[0]._pos1._y;

		for (int i = 0; i < 3; ++i)
		{
			float lenth = edges[i]._pos2._y - edges[i]._pos1._y;
			if (lenth > maxLength)
			{
				maxIndex = i;
				maxLength = lenth;
			}
		}

		int minIndex1 = (maxIndex + 1) % 3;
		int minIndex2 = (maxIndex + 2) % 3;

		_DrawTrianglePart(edges[maxIndex], edges[minIndex1], pImage);
		_DrawTrianglePart(edges[maxIndex], edges[minIndex2], pImage);
	}

	void XRaster::DrawImage(float x, float y, float w, float h)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + w, _w);
		float top = std::min<float>(y + h, _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U color(rand() % 255, rand() % 255, rand() % 255, rand() %255);
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawImage(float x, float y, XImage* pImage)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + pImage->Width(), _w);
		float top = std::min<float>(y + pImage->Height(), _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U color(pImage->DataXY(x - left, y - bottom));
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawImageWithColorKey(float x, float y, XImage* pImage, BGRA8U colorKey)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + pImage->Width(), _w);
		float top = std::min<float>(y + pImage->Height(), _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U color(pImage->DataXY(x - left, y - bottom));
				if (color._color != colorKey._color)
					_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawImageWithAlphaTest(float x, float y, XImage* pImage, unsigned char alpha)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + pImage->Width(), _w);
		float top = std::min<float>(y + pImage->Height(), _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U color(pImage->DataXY(x - left, y - bottom));
				if (color._a > alpha)
					_SetPix(x, y, color);
			}
		}
	}
	
	void XRaster::DrawImageWithAlphaBlend(float x, float y, XImage* pImage)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + pImage->Width(), _w);
		float top = std::min<float>(y + pImage->Height(), _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U srcColor(pImage->DataXY(x - left, y - bottom));
				BGRA8U desColor = _GetPix(x, y);
				BGRA8U color = _LerpColor(desColor, srcColor, srcColor._a / 255.0f);
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawImageWithAlphaBlendAndTransparent(float x, float y, XImage* pImage, float alpha)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + pImage->Width(), _w);
		float top = std::min<float>(y + pImage->Height(), _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U srcColor(pImage->DataXY(x - left, y - bottom));
				BGRA8U desColor = _GetPix(x, y);
				BGRA8U color = _LerpColor(desColor, srcColor, srcColor._a / 255.0f * alpha);
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawImageWithTransparent(float x, float y, XImage* pImage, float alpha)
	{
		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + pImage->Width(), _w);
		float top = std::min<float>(y + pImage->Height(), _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U srcColor(pImage->DataXY(x - left, y - bottom));
				BGRA8U desColor = _GetPix(x, y);
				BGRA8U color = _LerpColor(desColor, srcColor, alpha);
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawImageWithSize(float x, float y, XImage* pImage, float imageX, float imageY, float imageWidth, float imageHeight)
	{
		if (imageX + imageWidth > pImage->Width() || imageY + imageHeight > pImage->Height())
			return;

		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + imageWidth, _w);
		float top = std::min<float>(y + imageHeight, _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U color(pImage->DataXY(x - left + imageX, y - bottom + imageY));
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawImageWithScale(float x, float y, float w, float h, XImage* pImage)
	{
		if (w == 0 || h == 0)
			return;

		float xScale = pImage->Width() / w;
		float yScale = pImage->Height() / h;

		float left = std::max<float>(x, 0);
		float bottom = std::max<float>(y, 0);
		float right = std::min<float>(x + w, _w);
		float top = std::min<float>(y + h, _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = bottom; y < top; ++y)
			{
				BGRA8U color(pImage->DataXY((x - left) * xScale, (y - bottom) * yScale));
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::_DrawSpan(const SpanParam& span, XImage* pImage)
	{
		//计算标准步长偏移
		float offset = span._end._x - span._start._x;
		float step = 1.0f / offset;
		float scale = 0.0f;

		//优化x方向
		float startX = std::max<float>(span._start._x, 0);
		float endX = std::min<float>(span._end._x, _w);
		scale += (startX - span._start._x) / offset;

		//for (float x = span._xStart; x < span._xEndl; ++x)
		for (float x = startX; x < endX; ++x)
		{
			Vec2f uv = _LerpUV(span._startUV, span._endUV, scale);
			//BGRA8U color = _LerpColor(span._startColor, span._endColor, scale);

			BGRA8U color = pImage->DataUV(uv._u, uv._v);

			_SetPix(x, span._start._y, color);

			scale += step;
		}
	}

	void XRaster::_DrawTrianglePart(const EdgeParam& e1, const EdgeParam& e2, XImage* pImage)
	{
		//先计算e2 - 计算标准步长偏移
		float xOffset2 = e2._pos2._x - e2._pos1._x;
		float yOffset2 = e2._pos2._y - e2._pos1._y;
		if (yOffset2 == 0)
		{
			return;
		}
		float step2 = 1.0f / yOffset2;
		float scale2 = 0;

		//优化Y方向
		float startY2 = std::max<float>(e2._pos1._y, 0);
		float endY2 = std::min<float>(e2._pos2._y, _h);
		scale2 = (startY2 - e2._pos1._y) / yOffset2;

		//在计算e1 - 计算标准步长偏移
		float xOffset1 = e1._pos2._x - e1._pos1._x;
		float yOffset1 = e1._pos2._y - e1._pos1._y;
		if (yOffset1 == 0)
		{
			return;
		}
		float step1 = 1.0f / yOffset1;
		float scale1 = (e2._pos1._y - e1._pos1._y) / yOffset1;

		//优化Y方向 - 使用 startY2 计算。
		scale1 = (startY2 - e1._pos1._y) / yOffset1;

		//for (float y = e2._y1; y < e2._y2; ++y)
		for (float y = startY2; y < endY2; ++y)
		{
			Vec2f pos1(e1._pos1._x + xOffset1 * scale1, y);
			Vec2f pos2(e2._pos1._x + xOffset2 * scale2, y);

			Vec2f uv1 = _LerpUV(e1._uv1, e1._uv2, scale1);
			Vec2f uv2 = _LerpUV(e2._uv1, e2._uv2, scale2);

			BGRA8U color1 = _LerpColor(e1._color1, e1._color2, scale1);
			BGRA8U color2 = _LerpColor(e2._color1, e2._color2, scale2);

			SpanParam span(pos1, uv1, color1, pos2, uv2, color2);
			_DrawSpan(span, pImage);

			scale1 += step1;
			scale2 += step2;
		}
	}
}

