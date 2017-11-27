#include "Raster.h"

#include <algorithm>

namespace Smile 
{

	XRaster::XRaster(void* pBuffer, unsigned int w, unsigned int h)
		:
		BGRA8U_RED(0, 0, 255,255)
	{
		_pBuffer = (BGRA8U*)pBuffer;
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
		float top = std::max<float>(y, 0);
		float right = std::min<float>(x + w, _w);
		float bottom = std::min<float>(y + h, _h);

		for (float x = left; x < right; ++x)
		{
			for (float y = top; y < bottom; ++y)
			{
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawColorRect(Vec2f* posArray, BGRA8U* colorArray)
	{
		float left = std::max<float>(posArray[0]._x, 0);
		float top = std::max<float>(posArray[0]._y, 0);
		float right = std::min<float>(posArray[2]._x, _w);
		float bottom = std::min<float>(posArray[2]._y, _h);

		float w = right - left;
		float h = bottom - top;
		for (float x = left; x < right; ++x)
		{
			BGRA8U color1 = _LerpColor(colorArray[0], colorArray[1], (x - left) / w);
			BGRA8U color2 = _LerpColor(colorArray[3], colorArray[2], (x - left) / w);

			for (float y = top; y < bottom; ++y)
			{
				BGRA8U color = _LerpColor(color1, color2, (y - top) / h);
				_SetPix(x, y, color);
			}
		}
	}

	void XRaster::DrawSpan(const SpanParam& span)
	{
		float offset = span._xEndl - span._xStart;
		for (float x = span._xStart; x < span._xEndl; ++x)
		{
			BGRA8U color = _LerpColor(span._xStartColor, span._xEndColor, offset == 0 ? 0.5f : (x - span._xStart) / offset);
			_SetPixSafe(x, span._y, color);
		}
	}

	void XRaster::DrawTrianglePart(const Edge& e1, const Edge& e2)
	{
		float xOffset1 = e1._x2 - e1._x1;
		float yOffset1 = e1._y2 - e1._y1;
		if (yOffset1 == 0)
		{
			return;
		}
		float step1 = 1.0f / yOffset1;
		float scale1 = (e2._y1 - e1._y1) / yOffset1;


		float xOffset2 = e2._x2 - e2._x1;
		float yOffset2 = e2._y2 - e2._y1;
		if (yOffset2 == 0)
		{
			return;
		}
		float step2 = 1.0f / yOffset2;
		float scale2 = 0;

		for (float y = e2._y1; y < e2._y2; ++y)
		{
			float x1 = e1._x1 + xOffset1 * scale1;
			float x2 = e2._x1 + xOffset2 * scale2;

			SpanParam span(x1, x2, y, Smile::BGRA8U(255, 0, 0, 255), Smile::BGRA8U(255, 0, 0, 255));
			DrawSpan(span);

			scale1 += step1;
			scale2 += step2;
		}
	}

	void XRaster::DrawTriangle(Vec2f pos1, Vec2f pos2, Vec2f pos3)
	{
		Edge edges[3] = { Edge(pos1._x, pos1._y, pos2._x, pos2._y), Edge(pos2._x, pos2._y, pos3._x, pos3._y), Edge(pos3._x, pos3._y, pos1._x, pos1._y) };

		int maxIndex = 0;
		float maxLength = edges[0]._y2 - edges[0]._y1;

		for (int i = 0; i < 3; ++i)
		{
			float lenth = edges[i]._y2 - edges[i]._y1;
			if (lenth > maxLength)
			{
				maxIndex = i;
				maxLength = lenth;
			}
		}

		int minIndex1 = (maxIndex + 1) % 3;
		int minIndex2 = (maxIndex + 2) % 3;

		DrawTrianglePart(edges[maxIndex], edges[minIndex1]);
		DrawTrianglePart(edges[maxIndex], edges[minIndex2]);
	}
}

