#pragma once


namespace Smile 
{
#define PI 3.1415926
#define ANGLE2RADIAN(angle) ( 2 * PI / 360 * (angle))

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

	//2维向量
	template<typename T>
	class _Vector2
	{
	public:
		_Vector2() {}
		_Vector2(T x, T y) : _x(x), _y(y) {}
		union
		{
			struct  
			{
				T _x;
				T _y;
			};
			struct
			{
				T _u;
				T _v;
			};
		};
	};

	typedef _Vector2<float> Vec2f;

	//3维向量
	template<typename T>
	class _Vector3
	{
	public:
		_Vector3() {}
		_Vector3(T x, T y, T z) : _x(x), _y(y), _z(z) {}
		union
		{
			struct  
			{
				T _x;
				T _y;
				T _z;
			};
			struct
			{
				T _s;
				T _t;
				T _r;
			};
		};
	};

	typedef _Vector3<float> Vec3f;
}