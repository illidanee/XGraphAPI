#pragma once



namespace Smile
{
	class XImage
	{
	public:
		static XImage* LoadFromFile(const char* pFile);

	public:
		XImage(void* pBuffer, unsigned int w, unsigned int h);
		~XImage();
		
		unsigned int Width() const { return _w; }
		unsigned int Height() const { return _h; }
		unsigned int Data(unsigned int x, unsigned int y) const { return _pBuffer[y * _w + x]; }

	private:
		unsigned int* _pBuffer;
		unsigned int _w;
		unsigned int _h;
	};
}