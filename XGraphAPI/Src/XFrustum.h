#pragma once

#include "XMath.h"


namespace Smile
{
	/****************************************************************************************************************
	 *    Date    : 2017/12/12 14:19
	 *
	 *    Author  : Smile
	 *    Contact : smile@illidan.org
	 *
	 *    Brief   : 平面抽象
	 *
	 ****************************************************************************************************************/
	template<typename T>
	class _XPlane
	{
	public:
		_XPlane() {}
		_XPlane(T a, T b, T c, T d) : _a(a), _b(b), _c(c), _d(d) {}

		T Compute(T x, T y, T z)
		{
			return _a * x + _b * y + _c * z + _d;
		}

		T Compute(_XVector3<T> vector)
		{
			return _a * vector._x + _b * vector._y + _c * vector._z + _d;
		}

	public:
		union
		{
			struct  
			{
				T _a;
				T _b;
				T _c;
				T _d;
			};
			T _data[4];
		};

	};

	typedef _XPlane<float> XPlanef;


	/****************************************************************************************************************
	 *    Date    : 2017/12/12 14:19
	 *
	 *    Author  : Smile
	 *    Contact : smile@illidan.org
	 *
	 *    Brief   : 视椎体抽象
	 *
	 ****************************************************************************************************************/
	template<typename T>
	class _XFrustum
	{
	private:
		enum
		{
			TopFace = 0,
			DownFace = 1,
			LeftFace = 2,
			RightFace = 3,
			FrontFace = 4,
			BackFace = 5
		};

	public:
		_XFrustum() {}

		void Init(_XMatrix4<T> pvMat)
		{
			_planes[TopFace] = _XPlane<T>(pvMat[3][0] - pvMat[1][0], pvMat[3][1] - pvMat[1][1], pvMat[3][2] - pvMat[1][2], pvMat[3][3] - pvMat[1][3]);
			_planes[DownFace] = _XPlane<T>(pvMat[3][0] + pvMat[1][0], pvMat[3][1] + pvMat[1][1], pvMat[3][2] + pvMat[1][2], pvMat[3][3] + pvMat[1][3]);

			_planes[LeftFace] = _XPlane<T>(pvMat[3][0] + pvMat[0][0], pvMat[3][1] + pvMat[0][1], pvMat[3][2] + pvMat[0][2], pvMat[3][3] + pvMat[0][3]);
			_planes[RightFace] = _XPlane<T>(pvMat[3][0] - pvMat[0][0], pvMat[3][1] - pvMat[0][1], pvMat[3][2] - pvMat[0][2], pvMat[3][3] - pvMat[0][3]);

			_planes[FrontFace] = _XPlane<T>(pvMat[3][0] + pvMat[2][0], pvMat[3][1] + pvMat[2][1], pvMat[3][2] + pvMat[2][2], pvMat[3][3] + pvMat[2][3]);
			_planes[BackFace] = _XPlane<T>(pvMat[3][0] - pvMat[2][0], pvMat[3][1] - pvMat[2][1], pvMat[3][2] - pvMat[2][2], pvMat[3][3] - pvMat[2][3]);
		}

		bool PointInFrustum(_XVector3<T> point)
		{
			for (int i = TopFace; i <= BackFace; ++i)
			{
				if (_planes[i].Compute(point) <= 0)
					return false;
			}
			return true;
		}


	private:
		_XPlane<T> _planes[6];
	};

	typedef _XFrustum<float> XFrustumf;
}