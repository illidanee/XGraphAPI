#pragma once


namespace Smile
{
#define Zero 0
#define One 1

	//2维向量
	template<typename T>
	class _XVector2
	{
	public:
		_XVector2() {}
		_XVector2(T x, T y) : _x(x), _y(y) {}

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
			struct 
			{
				T _w;
				T _h;
			};
			T _data[2];
		};
	};

	typedef _XVector2<float> XVec2f;

	//3维向量
	template<typename T>
	class _XVector3
	{
	public:
		_XVector3() {}
		_XVector3(T x, T y, T z) : _x(x), _y(y), _z(z) {}

		T& operator [] (unsigned int i)
		{
			return _data[i];
		}

		const T& operator [] (unsigned int i) const
		{
			return _data[i];
		}

		_XVector3<T> operator - (const _XVector3<T>& that)
		{
			return _XVector3<T>(_x - that._x, _y - that._y, _z - that._z);
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

	typedef _XVector3<float> XVec3f;

	//4维向量
	template<typename T>
	class _XVector4
	{
	public:
		_XVector4() {}
		_XVector4(T x, T y, T z, T w) : _x(x), _y(y), _z(z), _w(w) {}

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
				T _w;
			};
			T _data[4];
		};
	};

	typedef _XVector4<float> XVec4f;

	//3x3矩阵
	template<typename T>
	class _XMatrix3
	{
	public:
		typedef _XVector3<T> _RowType;

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

		_XVector3<T> operator * (const _XVector3<T>& vector)
		{
			return _XVector3<T>(
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

	typedef _XMatrix3<float> XMat3f;

	template<typename T>
	class _XMatrix4
	{
	public:
		typedef _XVector4<T> _RowType;

		_XMatrix4() 
		{
			_row[0] = _RowType(One, Zero, Zero, Zero);
			_row[1] = _RowType(Zero, One, Zero, Zero);
			_row[2] = _RowType(Zero, Zero, One, Zero);
			_row[3] = _RowType(Zero, Zero, Zero, One);
		}

		_XMatrix4(T value)
		{
			_row[0] = _RowType(value, Zero, Zero, Zero);
			_row[1] = _RowType(Zero, value, Zero, Zero);
			_row[2] = _RowType(Zero, Zero, value, Zero);
			_row[3] = _RowType(Zero, Zero, Zero, value);
		}

		_XMatrix4(T value11, T value12, T value13, T value14,
			T value21, T value22, T value23, T value24,
			T value31, T value32, T value33, T value34,
			T value41, T value42, T value43, T value44)
		{
			_row[0] = _RowType(value11, value12, value13, value14);
			_row[1] = _RowType(value21, value22, value23, value24);
			_row[2] = _RowType(value31, value32, value33, value34);
			_row[3] = _RowType(value41, value42, value43, value44);
		}

		_XMatrix4(const _XMatrix4<T>& that)
		{
			_row[0] = _RowType(that._row[0]);
			_row[1] = _RowType(that._row[1]);
			_row[2] = _RowType(that._row[2]);
			_row[3] = _RowType(that._row[3]);
		}

		_RowType& operator [] (unsigned int i)
		{
			return _row[i];
		}

		const _RowType& operator [] (unsigned int i) const
		{
			return _row[i];
		}

		_XVector4<T> operator * (const _XVector4<T>& vector)
		{
			return _XVector4<T>(
				_row[0][0] * vector[0] + _row[0][1] * vector[1] + _row[0][2] * vector[2] + _row[0][3] * vector[3],
				_row[1][0] * vector[0] + _row[1][1] * vector[1] + _row[1][2] * vector[2] + _row[1][3] * vector[3],
				_row[2][0] * vector[0] + _row[2][1] * vector[1] + _row[2][2] * vector[2] + _row[2][3] * vector[3],
				_row[3][0] * vector[0] + _row[3][1] * vector[1] + _row[3][2] * vector[2] + _row[3][3] * vector[3]
			);
		}

		_XMatrix4<T> operator * (const _XMatrix4<T>& matrix)
		{
			return _XMatrix4<T>
			{
				_row[0][0] * matrix._row[0][0] + _row[0][1] * matrix._row[1][0] + _row[0][2] * matrix._row[2][0] + _row[0][3] * matrix._row[3][0],
				_row[0][0] * matrix._row[0][1] + _row[0][1] * matrix._row[1][1] + _row[0][2] * matrix._row[2][1] + _row[0][3] * matrix._row[3][1],
				_row[0][0] * matrix._row[0][2] + _row[0][1] * matrix._row[1][2] + _row[0][2] * matrix._row[2][2] + _row[0][3] * matrix._row[3][2],
				_row[0][0] * matrix._row[0][3] + _row[0][1] * matrix._row[1][3] + _row[0][2] * matrix._row[2][3] + _row[0][3] * matrix._row[3][3],

				_row[1][0] * matrix._row[0][0] + _row[1][1] * matrix._row[1][0] + _row[1][2] * matrix._row[2][0] + _row[1][3] * matrix._row[3][0],
				_row[1][0] * matrix._row[0][1] + _row[1][1] * matrix._row[1][1] + _row[1][2] * matrix._row[2][1] + _row[1][3] * matrix._row[3][1],
				_row[1][0] * matrix._row[0][2] + _row[1][1] * matrix._row[1][2] + _row[1][2] * matrix._row[2][2] + _row[1][3] * matrix._row[3][2],
				_row[1][0] * matrix._row[0][3] + _row[1][1] * matrix._row[1][3] + _row[1][2] * matrix._row[2][3] + _row[1][3] * matrix._row[3][3],

				_row[2][0] * matrix._row[0][0] + _row[2][1] * matrix._row[1][0] + _row[2][2] * matrix._row[2][0] + _row[2][3] * matrix._row[3][0],
				_row[2][0] * matrix._row[0][1] + _row[2][1] * matrix._row[1][1] + _row[2][2] * matrix._row[2][1] + _row[2][3] * matrix._row[3][1],
				_row[2][0] * matrix._row[0][2] + _row[2][1] * matrix._row[1][2] + _row[2][2] * matrix._row[2][2] + _row[2][3] * matrix._row[3][2],
				_row[2][0] * matrix._row[0][3] + _row[2][1] * matrix._row[1][3] + _row[2][2] * matrix._row[2][3] + _row[2][3] * matrix._row[3][3],

				_row[3][0] * matrix._row[0][0] + _row[3][1] * matrix._row[1][0] + _row[3][2] * matrix._row[2][0] + _row[3][3] * matrix._row[3][0],
				_row[3][0] * matrix._row[0][1] + _row[3][1] * matrix._row[1][1] + _row[3][2] * matrix._row[2][1] + _row[3][3] * matrix._row[3][1],
				_row[3][0] * matrix._row[0][2] + _row[3][1] * matrix._row[1][2] + _row[3][2] * matrix._row[2][2] + _row[3][3] * matrix._row[3][2],
				_row[3][0] * matrix._row[0][3] + _row[3][1] * matrix._row[1][3] + _row[3][2] * matrix._row[2][3] + _row[3][3] * matrix._row[3][3]
			};
		}

		void Scale(T x, T y, T z)
		{
			_row[0][0] = x;
			_row[1][1] = y;
			_row[2][2] = z;
		}

		void Rotate(T angle, _XVector3<T> vector)
		{
			T rad = ANGLE2RADIAN(angle);
			T c = cosf(rad);
			T s = sinf(rad);
			_row[0][0] = (vector._x * vector._x + (vector._y * vector._y + vector._z * vector._z) * c)														/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
			_row[0][1] = (vector._x * vector._y * (1 - c) - vector._z * sqrtf(vector._x * vector._x + vector._y * vector._y + vector._z * vector._z) * s)	/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
			_row[0][2] = (vector._x * vector._z * (1 - c) + vector._y * sqrtf(vector._x * vector._x + vector._y * vector._y + vector._z * vector._z) * s)	/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
			
			_row[1][0] = (vector._x * vector._y * (1 - c) + vector._z * sqrtf(vector._x * vector._x + vector._y * vector._y + vector._z * vector._z) * s)	/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
			_row[1][1] = (vector._y * vector._y + (vector._x * vector._x + vector._z * vector._z) * c)														/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
			_row[1][2] = (vector._y * vector._z * (1 - c) - vector._x * sqrtf(vector._x * vector._x + vector._y * vector._y + vector._z * vector._z) * s)	/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);

			_row[2][0] = (vector._x * vector._z * (1 - c) - vector._y * sqrtf(vector._x * vector._x + vector._y * vector._y + vector._z * vector._z) * s)	/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
			_row[2][1] = (vector._y * vector._z * (1 - c) + vector._x * sqrtf(vector._x * vector._x + vector._y * vector._y + vector._z * vector._z) * s)	/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
			_row[2][2] = (vector._z * vector._z + (vector._x * vector._x + vector._y * vector._y) * c)														/ (vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
		}

		void Translate(T x, T y, T z)
		{
			_row[0][3] = x;
			_row[1][3] = y;
			_row[2][3] = z;
		}

		_XMatrix4<T> Transpose()
		{
			return _XMatrix4<T>(
				_row[0][0], _row[1][0], _row[2][0], _row[3][0],
				_row[0][1], _row[1][1], _row[2][1], _row[3][1],
				_row[0][2], _row[1][2], _row[2][2], _row[3][2],
				_row[0][3], _row[1][3], _row[2][3], _row[3][3]
				);
		}

	private:
		_RowType _row[4];
	};

	typedef _XMatrix4<float> XMat4f;

//全局函数
	template<typename T>
	_XVector3<T> Normalize(_XVector3<T> vector)
	{
		T sqr = sqrtf(vector._x * vector._x + vector._y * vector._y + vector._z * vector._z);
		return _XVector3<T>(vector._x / sqr, vector._y / sqr, vector._z / sqr);
	}

	template<typename T>
	T Dot(_XVector3<T> vector1, _XVector3<T> vector2)
	{
		return vector1._x * vector2._x + vector1._y * vector2._y + vector1._z * vector2._z;
	}

	template<typename T>
	_XVector3<T> Cross(_XVector3<T> vector1, _XVector3<T> vector2)
	{
		return _XVector3<T>(
			vector1._y * vector2._z - vector2._y * vector1._z,
			vector1._z * vector2._x - vector2._z * vector1._x,
			vector1._x * vector2._y - vector2._x * vector1._y
			);
	}

	template<typename T>
	_XMatrix4<T> LookAt(_XVector3<T> pos, _XVector3<T> forward, _XVector3<T> up)
	{
		_XVector3<T> f = Normalize<T>(forward - pos);
		_XVector3<T> u = Normalize<T>(up);
		_XVector3<T> r = Normalize<T>(Cross<T>(f, u));
		u = Normalize<T>(Cross<T>(r, f));

		_XMatrix4<T> vMatrix;

		vMatrix[0][0] = r._x;
		vMatrix[0][1] = r._y;
		vMatrix[0][2] = r._z;
		vMatrix[0][3] = -Dot<T>(r, pos);

		vMatrix[1][0] = u._x;
		vMatrix[1][1] = u._y;
		vMatrix[1][2] = u._z;
		vMatrix[1][3] = -Dot<T>(u, pos);

		vMatrix[2][0] = -f._x;
		vMatrix[2][1] = -f._y;
		vMatrix[2][2] = -f._z;
		vMatrix[2][3] = Dot<T>(f, pos);

		return vMatrix;
	}

	template<typename T>
	_XMatrix4<T> Perspective(T fovy, T aspect, T zNear, T zFar)
	{
		T range = tanf(ANGLE2RADIAN(fovy * 0.5f)) * zNear;
		T left = -range * aspect;
		T right = range * aspect;
		T bottom = -range;
		T top = range;

		_XMatrix4<T> pMatrix;

		pMatrix[0][0] = (2.0f * zNear) / (right - left);
		pMatrix[0][2] = (right + left) / (right - left);

		pMatrix[1][1] = (2.0f * zNear) / (top - bottom);
		pMatrix[1][2] = (top + bottom) / (top - bottom);

		pMatrix[2][2] = -(zFar + zNear) / (zFar - zNear);
		pMatrix[2][3] = -(2.0f * zFar * zNear) / (zFar - zNear);

		pMatrix[3][2] = -1.0f;
		pMatrix[3][3] = 0.0f;

		return  pMatrix;
	}
}