#pragma once


namespace Smile 
{
#define PI 3.1415926f
#define ANGLE2RADIAN(angle) ( 2.0f * PI / 360.0f * (angle))

	//颜色
	template<typename T>
	class _BGRA
	{
	public:
		_BGRA() {}
		_BGRA(T b, T g, T r, T a) : _b(b), _g(g), _r(r), _a(a) { }
		_BGRA(unsigned int color) : _color(color) { }

		union
		{
			struct 
			{
				T _b;
				T _g;
				T _r;
				T _a;
			};
			unsigned int _color;
		};
	};

	typedef _BGRA<unsigned char> BGRA8U;

//全局函数
	template<typename T>
	_BGRA<T> operator + (const _BGRA<T>& left, const _BGRA<T>& right)
	{
		return  _BGRA<T>(
			left._b + (255 - left._b) * ((float)right._b / 255),
			left._g + (255 - left._g) * ((float)right._g / 255),
			left._r + (255 - left._r) * ((float)right._r / 255),
			left._a + (255 - left._a) * ((float)right._a / 255)
			);
	}
}