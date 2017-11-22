#pragma once


#include <memory.h>
#include <math.h>
#include "CommonType.h"


namespace Smile 
{

	class XRaster
	{
	public:
		XRaster(void* pBuffer, unsigned int w, unsigned int h);
		~XRaster();

		void Clean();
		void DrawPoint(unsigned int x, unsigned int y, BGRA8U color, POINTSIZE ps);
		void DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color);
		void DrawLine(Vec2f pos1, Vec2f pos2, BGRA8U color1, BGRA8U color2);

	private:
		void _SetPix(unsigned int x, unsigned int y, BGRA8U color);
		BGRA8U _LerpColor(BGRA8U color1, BGRA8U color2, float lerp);
	private:
		BGRA8U* _pBuffer;
		unsigned int _w;
		unsigned int _h;
	};

}