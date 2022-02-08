#ifndef CCAM_H
#define CCAM_H

#include "../libs.hpp"
#include "CPlayer.hpp"
#include "CProgram.hpp"

#define PI 3.1415926535

class CCamera
{
public:
	// Macierze rzutowania i widoku
	glm::mat4 matProj;
	glm::mat4 matView;

	// Skladowe kontrolujace matView
	glm::vec3 Position;	 // polozenie kamery
	glm::vec3 Angles;	 // pitch, yaw, roll
	glm::vec3 Up;		 // domyslnie wektor (0,1,0)
	glm::vec3 Direction; // wektor kierunku obliczany z Angles

	// Skladowe kontrolujace matProj
	float Width, Height;	   // proporcje bryly obcinania
	float NearPlane, FarPlane; // plaszczyzny tnace
	float Fov;				   // kat widzenia kamery

	CCamera() {}
	CCamera(glm::vec3 position, glm::vec3 angle = glm::vec3(0, 0, 0), float fov = 60.0f)
	{

		// Domyslne wartosci skladowych
		this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->Position = position;
		this->Angles = angle;
		this->Fov = fov;

		this->NearPlane = 0.1f;
		this->FarPlane = 150.0f;

		UpdateDirectionCross();
	}

	virtual void Update() {}

	glm::vec3 ExtractCameraPos()
	{
		glm::mat4 modelViewT = transpose(this->matView);

		// Get plane normals
		glm::vec3 n1(modelViewT[0]);
		glm::vec3 n2(modelViewT[1]);
		glm::vec3 n3(modelViewT[2]);

		// Get plane distances
		float d1(modelViewT[0].w);
		float d2(modelViewT[1].w);
		float d3(modelViewT[2].w);

		// Get the intersection of these 3 planes
		// http://paulbourke.net/geometry/3planes/
		glm::vec3 n2n3 = cross(n2, n3);
		glm::vec3 n3n1 = cross(n3, n1);
		glm::vec3 n1n2 = cross(n1, n2);

		glm::vec3 top = (n2n3 * d1) + (n3n1 * d2) + (n1n2 * d3);
		float denom = dot(n1, n2n3);

		return top / -denom;
	}

	void UpdateMatView()
	{
		this->matView = glm::lookAt(this->Position, this->Position + this->Direction, this->Up);
	}

	void SetPerspective(float width, float height, float fov)
	{
		this->Width = width;
		this->Height = height;
		this->Fov = fov;
		this->matProj = glm::perspectiveFov(glm::radians(this->Fov), this->Width, this->Height, this->NearPlane, this->FarPlane);
	}

	void SetOrtho(float width, float height, float fov)
	{
		this->Width = width;
		this->Height = height;
		this->Fov = fov;
		this->matProj = glm::perspectiveFov(glm::radians(this->Fov), width, height, this->NearPlane, this->FarPlane);
	}

	void UpdateDirectionCross()
	{
		this->Direction.x = cos(this->Angles.y) * cos(this->Angles.x);
		this->Direction.y = sin(this->Angles.x);
		this->Direction.z = -sin(this->Angles.y) * cos(this->Angles.x);
	}

	void SendPVE(CProgram *program)
	{
		program->SendUniformMatrix4fv("projection", this->matProj);
		program->SendUniformMatrix4fv("view", this->matView);
		program->SendUniform3fv("eyePosition", this->ExtractCameraPos());
	}

	void SendPV(CProgram * program)
	{
		glm::mat4 matProjView = this->matProj * this->matView;
		program->SendUniformMatrix4fv("projview", matProjView);
	}

	void AddFov(GLfloat _fov)
	{
		this->Fov += _fov;
		this->SetPerspective(this->Width, this->Height, this->Fov);
	}
};

class CFreeCamera : public CCamera
{

public:
	CFreeCamera(glm::vec3 position, glm::vec3 angle = glm::vec3(0, 0, 0), float fov = 60.0f) : CCamera(position, angle, fov)
	{
	}

	void Move(float val)
	{
		this->Position += this->Direction * val;
	}

	void Rotate(float pitch, float yaw)
	{
		this->Angles.x += pitch;
		this->Angles.y += yaw;

		if (this->Angles.x > PI / 2.0)
			this->Angles.x = PI / 2.0;
		if (this->Angles.x < -PI / 2.0)
			this->Angles.x = -PI / 2.0;
		if (this->Angles.y > 2.0 * PI)
			this->Angles.y -= 2.0 * PI;
		if (this->Angles.y < -2.0 * PI)
			this->Angles.y += 2.0 * PI;

		UpdateDirectionCross();
	}
};

class PlayerCamera : public CCamera
{

public:
	// For FPS/TPS camera
	glm::vec3 ShiftUp;

	CPlayer *Player = NULL;

	int playerView = 0;

	PlayerCamera(CPlayer *player, int playerView = 0, float fov = 60.0f)
	{
		this->playerView = playerView;
		this->ShiftUp = glm::vec3(0, 0, 0);
		this->Player = player;
		this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->Position = player->position + this->ShiftUp;
		this->Angles = glm::vec3(-0.25, this->Player->rotation.y - 1.5, 2.0);
		this->Fov = fov;

		this->NearPlane = 0.1f;
		this->FarPlane = 150.0f;

		UpdateDirectionCross();
	}

	void useFirstPersonView()
	{
		this->playerView = 0;
	}
	void useThirdPersonView()
	{
		this->playerView = 1;
	}

	virtual void Update()
	{
		if (this->playerView == 0)
			this->ShiftUp = glm::vec3(0, 1.7, 0);
		else
			this->ShiftUp = glm::vec3(-4 * sin(this->Player->rotation.y), 3.2, -4 * cos(this->Player->rotation.y));

		this->Angles = glm::vec3(-0.25, this->Player->rotation.y - 1.5, 2.0);
		this->Position = this->Player->position + this->ShiftUp;

		CCamera::UpdateDirectionCross();
		CCamera::UpdateMatView();
	}
};

#endif