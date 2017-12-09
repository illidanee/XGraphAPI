#pragma once


namespace Smile
{
#define Zero 0
#define One 1

	//2维向量
	template<typename T>
	class _Vector2
	{
	public:
		_Vector2() {}
		_Vector2(T x, T y) : _x(x), _y(y) {}

		T& operator [] (unsigned int i)
		{
			return _data[i];
		}

		const T& operator [] (unsigned int i) const
		{
			return _data[i];
		}

	public:
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
			T _data[2];
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

		T& operator [] (unsigned int i)
		{
			return _data[i];
		}

		const T& operator [] (unsigned int i) const
		{
			return _data[i];
		}

	public:
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
			T _data[3];
		};
	};

	typedef _Vector3<float> Vec3f;

	//3x3矩阵
	template<typename T>
	class _XMatrix3
	{
	public:
		typedef _Vector3<T> _RowType;

		_XMatrix3()
		{
			_row[0] = _RowType(One, Zero, Zero);
			_row[1] = _RowType(Zero, One, Zero);
			_row[2] = _RowType(Zero, Zero, One);
		}

		_XMatrix3(T value)
		{
			_row[0] = _RowType(value, Zero, Zero);
			_row[1] = _RowType(Zero, value, Zero);
			_row[2] = _RowType(Zero, Zero, value);
		}

		_XMatrix3(T value11, T value12, T value13, T value21, T value22, T value23, T value31, T value32, T value33)
		{
			_row[0] = _RowType(value11, value12, value13);
			_row[1] = _RowType(value21, value22, value23);
			_row[2] = _RowType(value31, value32, value33);
		}

		_XMatrix3(const _XMatrix3<T>& that)
		{
			_row[0] = _RowType(that._row[0]);
			_row[1] = _RowType(that._row[1]);
			_row[2] = _RowType(that._row[2]);
		}

		_RowType operator [] (unsigned int i)
		{
			return _row[i];
		}

		_RowType operator [] (unsigned int i) const
		{
			return _row[i];
		}

		_Vector3<T> operator * (const _Vector3<T>& vector)
		{
			return _Vector3<T>(
				_row[0][0] * vector[0] + _row[0][1] * vector[1] + _row[0][2] * vector[2],
				_row[1][0] * vector[0] + _row[1][1] * vector[1] + _row[1][2] * vector[2],
				_row[2][0] * vector[0] + _row[2][1] * vector[1] + _row[2][2] * vector[2]
				);
		}

		_XMatrix3<T> operator * (const _XMatrix3<T>& matrix)
		{
			return _XMatrix3<T>(
				_row[0][0] * matrix._row[0][0] + _row[0][1] * matrix._row[1][0] + _row[0][2] * matrix._row[2][0],
				_row[0][0] * matrix._row[0][1] + _row[0][1] * matrix._row[1][1] + _row[0][2] * matrix._row[2][1],
				_row[0][0] * matrix._row[0][2] + _row[0][1] * matrix._row[1][2] + _row[0][2] * matrix._row[2][2],
				_row[1][0] * matrix._row[0][0] + _row[1][1] * matrix._row[1][0] + _row[1][2] * matrix._row[2][0],
				_row[1][0] * matrix._row[0][1] + _row[1][1] * matrix._row[1][1] + _row[1][2] * matrix._row[2][1],
				_row[1][0] * matrix._row[0][2] + _row[1][1] * matrix._row[1][2] + _row[1][2] * matrix._row[2][2],
				_row[2][0] * matrix._row[0][0] + _row[2][1] * matrix._row[1][0] + _row[2][2] * matrix._row[2][0],
				_row[2][0] * matrix._row[0][1] + _row[2][1] * matrix._row[1][1] + _row[2][2] * matrix._row[2][1],
				_row[2][0] * matrix._row[0][2] + _row[2][1] * matrix._row[1][2] + _row[2][2] * matrix._row[2][2]
				);
		}

		void Scale(T x, T y)
		{
			_row[0][0] = x;
			_row[1][1] = y;
		}

		void Rotate(T angle)
		{
			T rad = ANGLE2RADIAN(angle);
			T c = cosf(rad);
			T s = sinf(rad);
			_row[0][0] = c;
			_row[0][1] = -s;
			_row[1][0] = s;
			_row[1][1] = c;
		}

		void Translate(T x, T y)
		{
			_row[0][2] = x;
			_row[1][2] = y;
		}

	private:
		_RowType _row[3];
	};

	typedef _XMatrix3<float> XMatrix3f;
}