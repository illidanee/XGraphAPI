#pragma once



namespace Smile
{
	class XImage
	{
	public:
		enum _WRAPTYPE
		{
			_WT_REPEAT = 0,
			_WT_EDGE = 1,
		};

		typedef _WRAPTYPE WRAPTYPE;

	public:
		static XImage* LoadFromFile(const char* pFile);

	public:
		XImage(void* pBuffer, unsigned int w, unsigned int h);
		~XImage();
		
		unsigned int Width() const { return _w; }
		unsigned int Height() const { return _h; }
		unsigned int DataXY(unsigned int x, unsigned int y) const { return _pBuffer[y * _w + x]; }
		unsigned int DataUV(float u, float v);
		void SetWrapType(WRAPTYPE wrapType) { _wrapType = wrapType; }

	private:
		unsigned int* _pBuffer;
		unsigned int _w;
		unsigned int _h;

		WRAPTYPE _wrapType;
	};
}