#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"
#include "Utility.h"

Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	magnitude = Math::FAbs(target.z - pos.z);


}

void Camera2::Update(double dt, TRS player)
{
	static const float CAMERA_SPEED = 10.f;
	static const float ROTATE_SPEED = 135.f;

	Mtx44 rotation;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	Vector3 actualup = Vector3(0, 1, 0);

	up = right.Cross(view).Normalized();
	float yaw = (float)(ROTATE_SPEED * dt);
	rotation.SetToRotation(player.RotateY.degree,0,1,0);
	target = player.translate + Vector3(0, 5, 0);
	position = player.translate + Vector3(magnitude * sin(Math::DegreeToRadian(-player.RotateY.degree)),
		8, magnitude * -cos(Math::DegreeToRadian(player.RotateY.degree)));
	view = (target - position).Normalized();


	// OLD VARIABLES
	//static const float CAMERA_SPEED = 30.f;
	//static const float ROTATE_SPEED = 50.f;

	//Mtx44 rotation;
	//Vector3 view = (target - position).Normalized();
	//Vector3 right = view.Cross(up);
	//right.y = 0;
	//Vector3 actualup = Vector3(0, 1, 0);

	//if (Application::IsKeyPressed('F'))
	//{
	//	position = position - right * (float)(CAMERA_SPEED * dt);
	//	target = position + view * (float)(CAMERA_SPEED * dt);
	//}
	//else if (Application::IsKeyPressed('H'))
	//{
	//	position = position + right * (float)(CAMERA_SPEED * dt);
	//	target = position + view * (float)(CAMERA_SPEED * dt);
	//}
	//if (Application::IsKeyPressed('Q'))
	//{
	//	position = position + up * (float)(CAMERA_SPEED * dt);
	//	target = position + view * (float)(CAMERA_SPEED * dt);
	//}
	//else if (Application::IsKeyPressed('E'))
	//{
	//	position = position - up * (float)(CAMERA_SPEED * dt);
	//	target = position + view * (float)(CAMERA_SPEED * dt);
	//}
	//if (Application::IsKeyPressed('T'))
	//{
	//	position = position + view * (float)(CAMERA_SPEED * dt);
	//	target = position + view * (float)(CAMERA_SPEED * dt);
	//}
	//else if (Application::IsKeyPressed('G'))
	//{
	//	position = position - view * (float)(CAMERA_SPEED * dt);
	//	target = position + view * (float)(CAMERA_SPEED * dt);
	//}

	//if (Application::IsKeyPressed(VK_LEFT))
	//{
	//	up = right.Cross(view).Normalized();
	//	float yaw = (float)(ROTATE_SPEED * dt);
	//	rotation.SetToRotation(yaw, actualup.x, actualup.y, actualup.z);
	//	target = player.translate + Vector3(0, 5, 0);
	//	position = player.translate + Vector3(magnitude * sin(Math::DegreeToRadian(-player.RotateY.degree)) ,
	//		8 , magnitude * -cos(Math::DegreeToRadian(player.RotateY.degree)));
	//	view = (target - position).Normalized();
	//}
	//else if (Application::IsKeyPressed(VK_RIGHT))
	//{
	//	up = right.Cross(view).Normalized();
	//	float yaw = (float)(-ROTATE_SPEED * dt);
	//	target = player.translate + Vector3(0, 5, 0);
	//	position = player.translate + Vector3(magnitude * sin(Math::DegreeToRadian(-player.RotateY.degree)),
	//		8, magnitude * -cos(Math::DegreeToRadian(player.RotateY.degree)));
	//	view = (target - position).Normalized();
	//}
	//else if (Application::IsKeyPressed(VK_UP))
	//{
	//	float pitch = (float)(ROTATE_SPEED * dt);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}
	//else if (Application::IsKeyPressed(VK_DOWN))
	//{
	//	float pitch = (float)(-ROTATE_SPEED * dt);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}