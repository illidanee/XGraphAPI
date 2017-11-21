#pragma once


#include <memory.h>
#include "CommonType.h"


namespace Smile 
{

	class XRaster
	{
	public:
		XRaster(void* pBuffer, unsigned int w, unsigned int h);
		~XRaster();

		void Clean();
		void DrawPoint(unsigned int x, unsigned int y, BGRA8 color, POINTSIZE ps);

	private:
		void _SetPix(unsigned int x, unsigned int y, BGRA8 color);
	private:
		BGRA8* _pBuffer;
		unsigned int _w;
		unsigned int _h;
	};

}