#pragma once

#include "CommonType.h"
#include "XMath.h"

namespace Smile
{
	/****************************************************************************************************************
	 *    Date    : 2017/12/14 9:51
	 *
	 *    Author  : Smile
	 *    Contact : smile@illidan.org
	 *
	 *    Brief   : 封装简单摄像机类
	 *
	 ****************************************************************************************************************/
	class XCamera
	{
	public:
		XCamera();
		void Init(XVec3f eye = XVec3f(0.0f, 20.0f, 20.0f), XVec3f aim = XVec3f(0.0f, 0.0f, 0.0f), XVec3f right = XVec3f(1.0f, 0.0f, 0.0f));
		void Update();

		void RotateX(float angle);
		void RotateY(float angle);

		XVec2f WorldToScreen(const XVec3f& world);
		XVec3f ScreenToWorld(const XVec2f& screen);

		XRayf CreateRayFromScreen(const XVec2f screen);

	private:
		bool _Project(const XVec4f& world, XVec4f& screen);
		bool _UnProject(const XVec4f& screen, XVec4f& world);

	public:
		XVec3f GetEye() { return _eye; }
		void SetEye(XVec3f eye) { _eye = eye; }

		XVec3f GetAim() { return _aim; }
		void SetAim(XVec3f aim) { _aim = aim; }

		XVec3f GetRight() { return _right; }
		void SetRight(XVec3f right) { _right = right; }

		XVec3f GetDir() { return _dir; }
		void SetDir(XVec3f dir) { _dir = dir; }

		XVec3f GetUp() { return _up; }
		void SetUp(XVec3f up) { _up = up; }

		XMat4f GetMMatrix() { return _MMatrix; }
		void SetMMatrix(const XMat4f& mMatrix) { _MMatrix = mMatrix; }

		XMat4f GetVMatrix() { return _VMatrix; }
		void SetVMatrix(const XMat4f& vMatrix) { _VMatrix = vMatrix; }

		XMat4f GetPMatrix() { return _PMatrix; }
		void SetPMatrix(const XMat4f& pMatrix) { _PMatrix = pMatrix; }

		XVec2f GetViewport() { return _viewport; }
		void SetViewport(const XVec2f& viewport) { _viewport = viewport; }

	private:
		XVec3f _eye;
		XVec3f _aim;
		XVec3f _right;

		XVec3f _dir;
		XVec3f _up;

		XMat4f _MMatrix;
		XMat4f _VMatrix;
		XMat4f _PMatrix;

		XVec2f _viewport;
	};
}