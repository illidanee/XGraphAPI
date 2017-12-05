#include "XImage.h"

#include <memory.h>
#include "../FreeImage/FreeImage.h"


namespace Smile
{
	XImage* XImage::LoadFromFile(const char* pFile)
	{
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(pFile, 0);
		if (fifmt == FIF_UNKNOWN)
		{
			return 0;
		}

		FIBITMAP* dib = FreeImage_Load(fifmt, pFile, 0);
		FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);
		FreeImage_Unload(dib);

		BYTE* pixels = (BYTE*)FreeImage_GetBits(dib32);
		unsigned int width = FreeImage_GetWidth(dib32);
		unsigned int height = FreeImage_GetHeight(dib32);
		XImage* image = new XImage(pixels, width, height);
		FreeImage_Unload(dib32);
		return image;
	}

	XImage::XImage(void* pBuffer, unsigned int w, unsigned int h)
	{
		_pBuffer = new unsigned int[w * h];
		_w = w;
		_h = h;
		memcpy(_pBuffer, pBuffer, w * h * sizeof(unsigned int));
	}

	XImage::~XImage()
	{
		delete[] _pBuffer;
	}

	unsigned int XImage::DataUV(float u, float v)
	{
		float x = u * _w;
		float y = v * _h;
		return DataXY(x, y);
	}
}