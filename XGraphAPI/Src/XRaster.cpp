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

		memset(&_vertex, 0, sizeof(_vertex));
		memset(&_uv, 0, sizeof(_uv));
		memset(&_color, 0, sizeof(_color));

		_uvDefault._size = 2;
		_uvDefault._type = _DT_FLOAT;
		_uvDefault._stride = sizeof(XVec2f);
		_uvDefault._pData = _defaultUVArray;

		_colorDefault._size = 4;
		_colorDefault._type = _DT_FLOAT;
		_colorDefault._stride = sizeof(BGRA8U);
		_colorDefault._pData = _defaultColorArray;

		_pImage = 0;

		_viewport._w = _w;
		_viewport._h = _h;
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

	void XRaster::DrawPoint(XVec2f pos, BGRA8U color, POINTSIZE ps)
	{
		DrawPoint(pos._x, pos._y, color, ps);
	}

	void XRaster::DrawLine(XVec2f pos1, XVec2f pos2, BGRA8U color)
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

	void XRaster::DrawLine(XVec2f pos1, XVec2f pos2, BGRA8U color1, BGRA8U color2)
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

	void XRaster::DrawArray(DRAWMODE drawMode, XVec2f* posArray, int len)
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

	void XRaster::DrawColorRect(XVec2f* posArray, BGRA8U* colorArray)
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

	void XRaster::VertexPointer(int size, DATATYPE type, int stride, void* pData)
	{
		_vertex._size = size;
		_vertex._type = type;
		_vertex._stride = stride;
		_vertex._pData = pData;
	}

	void XRaster::UVPointer(int size, DATATYPE type, int stride, void* pData)
	{
		_uv._size = size;
		_uv._type = type;
		_uv._stride = stride;
		_uv._pData = pData;
	}

	void XRaster::ColorPointer(int size, DATATYPE type, int stride, void* pData)
	{
		_color._size = size;
		_color._type = type;
		_color._stride = stride;
		_color._pData = pData;
	}

	void XRaster::BindTexture(XImage* pImage)
	{
		_pImage = pImage;
	}

	void XRaster::DrawArray(DRAWMODE drawMode, int start, int end)
	{
		if (_vertex._pData == 0)
		{
			return;
		}

		Element uvTemp = _uv;
		if (_uv._pData == 0)
		{
			uvTemp = _uvDefault;
		}

		Element colorTemp = _color;
		if (_color._pData == 0)
		{
			colorTemp = _colorDefault;
		}

		char* pPosData = (char*)_vertex._pData;
		char* pUVData = (char*)uvTemp._pData;
		char* pColorData = (char*)colorTemp._pData;

		//计算pv矩阵
		_pvMatrix = _pMatrix * _vMatrix;

		//生成视椎体
		_frustum.Init(_pvMatrix);

		end = end / 3 * 3;
		for (int i = start; i < end; i += 3)
		{
//带有视景体裁剪
			////Vertex
			////char* pPosData = (char*)_vertex._pData;
			//float* pPos = (float*)pPosData;
			//XVec4f pos1Temp4(pPos[0], pPos[1], pPos[2], 1.0f);
			//pPosData += _vertex._stride;
			//pPos = (float*)pPosData;
			//XVec4f pos2Temp4(pPos[0], pPos[1], pPos[2], 1.0f);
			//pPosData += _vertex._stride;
			//pPos = (float*)pPosData;
			//XVec4f pos3Temp4(pPos[0], pPos[1], pPos[2], 1.0f);

			////将顶点从模型本地坐标系转换到世界坐标系。
			//pos1Temp4 = _mMatrix * pos1Temp4;
			//pos2Temp4 = _mMatrix * pos2Temp4;
			//pos3Temp4 = _mMatrix * pos3Temp4;

			//XVec3f pos1Temp3(pos1Temp4._x, pos1Temp4._y, pos1Temp4._z);
			//XVec3f pos2Temp3(pos2Temp4._x, pos2Temp4._y, pos2Temp4._z);
			//XVec3f pos3Temp3(pos3Temp4._x, pos3Temp4._y, pos3Temp4._z);

			////裁剪坐标
			//if (!_frustum.PointInFrustum(pos1Temp3) && !_frustum.PointInFrustum(pos2Temp3) && !_frustum.PointInFrustum(pos3Temp3))
			//{
			//	return;
			//}

			////固定管线计算。
			//pos1Temp3 = _Pipeline(pos1Temp3);
			//pos2Temp3 = _Pipeline(pos2Temp3);
			//pos3Temp3 = _Pipeline(pos3Temp3);

			////以下是二维绘制。
			//XVec2f pos1(pos1Temp3._x, pos1Temp3._y);
			//XVec2f pos2(pos2Temp3._x, pos2Temp3._y);
			//XVec2f pos3(pos3Temp3._x, pos3Temp3._y);

//没有视景体裁剪
			float* pPos = (float*)pPosData;
			XVec3f pos1Temp3(pPos[0], pPos[1], pPos[2]);
			pPosData += _vertex._stride;
			pPos = (float*)pPosData;
			XVec3f pos2Temp3(pPos[0], pPos[1], pPos[2]);
			pPosData += _vertex._stride;
			pPos = (float*)pPosData;
			XVec3f pos3Temp3(pPos[0], pPos[1], pPos[2]);

			pos1Temp3 = _Pipeline(pos1Temp3);
			pos2Temp3 = _Pipeline(pos2Temp3);
			pos3Temp3 = _Pipeline(pos3Temp3);

			//以下是二维绘制。
			XVec2f pos1(pos1Temp3._x, pos1Temp3._y);
			XVec2f pos2(pos2Temp3._x, pos2Temp3._y);
			XVec2f pos3(pos3Temp3._x, pos3Temp3._y);

			//UV
			float* pUV = (float*)pUVData;
			XVec2f uv1(pUV[0], pUV[1]);
			pUVData += uvTemp._stride;
			pUV = (float*)pUVData;
			XVec2f uv2(pUV[0], pUV[1]);
			pUVData += uvTemp._stride;
			pUV = (float*)pUVData;
			XVec2f uv3(pUV[0], pUV[1]);

			//Color
			BGRA8U* pColor = (BGRA8U*)pColorData;
			BGRA8U color1(pColor[0]);
			pColorData += colorTemp._stride;
			pColor = (BGRA8U*)pColorData;
			BGRA8U color2(pColor[0]);
			pColorData += colorTemp._stride;
			pColor = (BGRA8U*)pColorData;
			BGRA8U color3(pColor[0]);

			EdgeParam edges[3] = {
				EdgeParam(pos1, uv1, color1, pos2, uv2, color2),
				EdgeParam(pos2, uv2, color2, pos3, uv3, color3),
				EdgeParam(pos3, uv3, color3, pos1, uv1, color1) 
			};

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

			_DrawTrianglePart(edges[maxIndex], edges[minIndex1], _pImage);
			_DrawTrianglePart(edges[maxIndex], edges[minIndex2], _pImage);

			if (_uv._pData == 0)
				pUVData = (char*)uvTemp._pData;

			if (_color._pData == 0)
				pColorData = (char*)colorTemp._pData;
		}
	}

	void XRaster::LoadMIdentity()
	{
		_mMatrix = XMat4f();
	}

	void XRaster::LoadMMatrix(XMat4f mMatrix)
	{
		_mMatrix = mMatrix;
	}

	void XRaster::LoadVIdentity()
	{
		_vMatrix = XMat4f();
	}

	void XRaster::LoadVMatrix(XMat4f vMatrix)
	{
		_vMatrix = vMatrix;
	}

	void XRaster::LoadPIdentity()
	{
		_pMatrix = XMat4f();
	}

	void XRaster::LoadPMatrix(XMat4f pMatrix)
	{
		_pMatrix = pMatrix;
	}

	void XRaster::LoadPVIdentity()
	{
		_pvMatrix = XMat4f();
	}

	void XRaster::LoadPVMatrix(XMat4f pvMatrix)
	{
		_pvMatrix = pvMatrix;
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
			XVec2f uv = _LerpUV(span._startUV, span._endUV, scale);
			BGRA8U colorUV = pImage->DataUV(uv._u, uv._v);

			BGRA8U colorXY = _LerpColor(span._startColor, span._endColor, scale);

			BGRA8U color = colorUV + colorXY;

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
			XVec2f pos1(e1._pos1._x + xOffset1 * scale1, y);
			XVec2f pos2(e2._pos1._x + xOffset2 * scale2, y);

			XVec2f uv1 = _LerpUV(e1._uv1, e1._uv2, scale1);
			XVec2f uv2 = _LerpUV(e2._uv1, e2._uv2, scale2);

			BGRA8U color1 = _LerpColor(e1._color1, e1._color2, scale1);
			BGRA8U color2 = _LerpColor(e2._color1, e2._color2, scale2);

			SpanParam span(pos1, uv1, color1, pos2, uv2, color2);
			_DrawSpan(span, pImage);

			scale1 += step1;
			scale2 += step2;
		}
	}

	void XRaster::_DrawTriangle(const TriangleParam& triangle, XImage* pImage)
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

	XVec3f XRaster::_Pipeline(XVec3f vector)
	{
		//转换4维齐次坐标
		XVec4f worldPos(vector._x, vector._y, vector._z, 1.0f);

		//坐标转换
		XVec4f screenPos = _pMatrix * _vMatrix * _mMatrix * worldPos;

		if (screenPos._w == 0)
			return XVec3f(0, 0, 0);

		//转换到（-1 ~ +1）
		screenPos._x /= screenPos._w;
		screenPos._y /= screenPos._w;
		screenPos._z /= screenPos._w;

		//转换到（0 ~ +1）
		screenPos._x = screenPos._x * 0.5f + 0.5f;
		screenPos._y = screenPos._y * 0.5f + 0.5f;
		screenPos._z = screenPos._z * 0.5f + 0.5f;

		//转换到屏幕坐标
		screenPos._x = screenPos._x * _viewport._w;
		screenPos._y = screenPos._y * _viewport._h;

		return XVec3f(screenPos._x, screenPos._y, screenPos._z);
   	}
}

