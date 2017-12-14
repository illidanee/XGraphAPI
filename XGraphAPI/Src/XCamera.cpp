#include "XCamera.h"


namespace Smile
{
	XCamera::XCamera()
	{

	}

	void XCamera::Init(XVec3f eye, XVec3f aim, XVec3f right)
	{
		_eye = eye;
		_aim = aim;
		_right = right;

		_dir = Normalize<float>(_aim - _eye);
		_up = Normalize<float>(Cross<float>(_right, _dir));

		_VMatrix = LookAt<float>(_eye, _aim, _up);
	}

	void XCamera::Update()
	{
		_VMatrix = LookAt<float>(_eye, _aim, _up);
	}

	void XCamera::RotateX(float angle)
	{
		XMat4f mat;
		mat.Rotate(angle, _right);

		XVec4f vectorDir(_dir._x, _dir._y, _dir._z, 1.0f);
		XVec4f vectorUp(_dir._x, _dir._y, _dir._z, 1.0f);

		vectorDir = mat * vectorDir;
		vectorUp = mat * vectorUp;

		_dir = XVec3f(vectorDir._x, vectorDir._y, vectorDir._z);
		_up = XVec3f(vectorUp._x, vectorUp._y, vectorUp._z);

		_right = Normalize<float>(Cross<float>(_dir, _up));
		float len = Length<float>(_aim - _eye);
		_eye = _aim - _dir * len;

		_VMatrix = LookAt<float>(_eye, _aim, _up);
	}

	void XCamera::RotateY(float angle)
	{
		XVec3f yAxis(0.0f, 1.0f, 0.0f);

		XMat4f mat;
		mat.Rotate(angle, yAxis);

		XVec4f vectorDir(_dir._x, _dir._y, _dir._z, 1.0f);
		XVec4f vectorUp(_dir._x, _dir._y, _dir._z, 1.0f);

		vectorDir = mat * vectorDir;
		vectorUp = mat * vectorUp;

		_dir = XVec3f(vectorDir._x, vectorDir._y, vectorDir._z);
		_up = XVec3f(vectorUp._x, vectorUp._y, vectorUp._z);

		_right = Normalize<float>(Cross<float>(_dir, _up));
		float len = Length<float>(_aim - _eye);
		_eye = _aim - _dir * len;

		_VMatrix = LookAt<float>(_eye, _aim, _up);
	}

	XVec2f XCamera::WorldToScreen(const XVec3f& world)
	{
		XVec4f tempWorld(world._x, world._y, world._z, 1.0f);
		XVec4f tempScreen;
		_Project(tempWorld, tempScreen);
		return XVec2f(tempScreen._x, tempScreen._y);
	}

	XVec3f XCamera::ScreenToWorld(const XVec2f& screen)
	{
		XVec4f tempScreen(screen._x, screen._y, 0.0f, 1.0f);
		XVec4f tempWorld;
		_UnProject(tempScreen, tempWorld);
		return XVec3f(tempWorld._x, tempWorld._y, tempWorld._z);
	}

	XRayf XCamera::CreateRayFromScreen(const XVec2f screen)
	{
		XVec4f screenNear(screen._x, screen._y, 0.0f, 1.0f);
		XVec4f screenFar(screen._x, screen._y, 1.0f, 1.0f);

		XVec4f worldNear;
		XVec4f worldFar;

		_UnProject(screenNear, worldNear);
		_UnProject(screenFar, worldFar);

		XVec4f direction = worldFar - worldNear;

		XVec3f origin(worldNear._x, worldNear._y, worldNear._z);
		XVec3f dir(direction._x, direction._y, direction._z);

		return XRayf(origin, dir);
	}

	bool XCamera::_Project(const XVec4f& world, XVec4f& screen)
	{
		screen = _PMatrix * _VMatrix * _MMatrix * world;
		if (screen._w == 0)
			return false;

		screen._x /= screen._w;
		screen._y /= screen._w;
		screen._z /= screen._w;

		screen._x = screen._x * 0.5f + 0.5f;
		screen._y = screen._y * 0.5f + 0.5f;

		screen._x *= _viewport._w;
		screen._y *= _viewport._h;

		return true;
	}

	bool XCamera::_UnProject(const XVec4f& screen, XVec4f& world)
	{
		world._x = screen._x / _viewport._w;
		world._y = screen._y / _viewport._h;
		world._z = screen._z;
		world._w = screen._w;

		world._x = 2.0f * world._x - 1.0f;
		world._y = 2.0f * world._y - 1.0f;
		world._z = 2.0f * world._z - 1.0f;

		XMat4f inverse = (_PMatrix * _VMatrix * _MMatrix).Inverse();

		world = inverse * world;

		if (world._w == 0.0f)
			return false;

		world = world * (1.0f / world._w);

		return true;
	}
}