#pragma once


namespace Smile 
{
	enum _POINTSIZE
	{
		MINSIZE,
		MIDSIZE,
		MAXSIZE,
	};

	typedef _POINTSIZE POINTSIZE;


	template<typename T>
	class _BGRA
	{
	public:
		_BGRA(T r, T g, T b, T a) : _r(r), _g(g), _b(b), _a(a) { }
		T _b;
		T _g;
		T _r;
		T _a;
	};

	typedef _BGRA<char> BGRA8;
}