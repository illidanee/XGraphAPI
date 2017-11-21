#include "Raster.h"


namespace Smile 
{

	XRaster::XRaster(void* pBuffer, unsigned int w, unsigned int h)
	{
		_pBuffer = (BGRA8*)pBuffer;
		_w = w;
		_h = h;
	}

	XRaster::~XRaster()
	{

	}

	void XRaster::Clean()
	{
		memset(_pBuffer, 0, _w * _h * sizeof(BGRA8));
	}

	void XRaster::DrawPoint(unsigned int x, unsigned int y, BGRA8 color, POINTSIZE ps)
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

	void XRaster::_SetPix(unsigned int x, unsigned int y, BGRA8 color)
	{
		if (y < 10)
			return;

		if (x >= _w || y >= _h)
			return;

		_pBuffer[y * _w + x] = color;
	}

}