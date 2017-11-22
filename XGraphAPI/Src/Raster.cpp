#include "Raster.h"


namespace Smile 
{

	XRaster::XRaster(void* pBuffer, unsigned int w, unsigned int h)
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

	void XRaster::DrawPoint(unsigned int x, unsigned int y, BGRA8U color, POINTSIZE ps)
	{
		switch (ps)
		{
		case MINSIZE:
		{
			_SetPix(x, y, color);
		}
			break;
		case MIDSIZE:
		{
			_SetPix(x + 0, y + 0, color);
			_SetPix(x + 1, y + 0, color);
			_SetPix(x + 0, y + 1, color);
			_SetPix(x + 1, y + 1, color);
		}
			break;
		case MAXSIZE:
		{
			_SetPix(x - 1, y - 1, color);
			_SetPix(x + 0, y - 1, color);
			_SetPix(x + 1, y - 1, color);

			_SetPix(x - 1, y + 0, color);
			_SetPix(x + 0, y + 0, color);
			_SetPix(x + 1, y + 0, color);

			_SetPix(x - 1, y + 1, color);
			_SetPix(x + 0, y + 1, color);
			_SetPix(x + 1, y + 1, color);
		}
			break;
		default:
			_SetPix(x, y, color);
		}
	}

	void XRaster::DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color)
	{
		float xOffset = pos1._x - pos2._x;
		float yOffset = pos1._y - pos2._y;
		
		if (xOffset == 0 && yOffset == 0)
		{
			_SetPix(pos1._x, pos1._y, color);
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
					_SetPix(x, y, color);
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
					_SetPix(x, y, color);
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
			_SetPix(pos1._x, pos1._y, _LerpColor(color1, color2, 0.5f));
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
				_SetPix(x, y, _LerpColor(color1, color2, lerp));
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
				_SetPix(x, y, _LerpColor(color1, color2, lerp));
			}
		}
	}

	void XRaster::_SetPix(unsigned int x, unsigned int y, BGRA8U color)
	{
		if (y < 10)
			return;

		if (x >= _w || y >= _h)
			return;

		_pBuffer[y * _w + x] = color;
	}

	BGRA8U XRaster::_LerpColor(BGRA8U color1, BGRA8U color2, float lerp)
	{
		BGRA8U color(0, 0, 0, 0);
		color._b = color1._b + (color1._b - color2._b) * lerp;
		color._g = color1._g + (color1._g - color2._g) * lerp;
		color._r = color1._r + (color1._r - color2._r) * lerp;
		color._a = color1._a + (color1._a - color2._a) * lerp;
		return color;
	}
}

