#pragma once


namespace Smile 
{
	//点的大小
	enum _POINTSIZE
	{
		MINSIZE,
		MIDSIZE,
		MAXSIZE,
	};

	typedef _POINTSIZE POINTSIZE;

	//颜色
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

	typedef _BGRA<unsigned char> BGRA8U;

	//2维向量
	template<typename T>
	class _Vector2
	{
	public:
		_Vector2(T x, T y) : _x(x), _y(y) {}
		T _x;
		T _y;
	};

	typedef _Vector2<float> Vec2f;

	//3维向量
	template<typename T>
	class _Vector3
	{
	public:
		_Vector3(T x, T y, T z) : _x(x), _y(y), _z(z) {}
		T _x;
		T _y;
		T _z;
	};

	typedef _Vector3<float> Vec3f;
}