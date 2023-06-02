
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "shader.h"


void Player::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\torus.obj");
}

void Player::Uninit()
{
	GameObject::Uninit();
}

void Player::Update()
{
	if (Input::GetKeyPress('A'))
	{
		m_Position.x -= 0.1f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Position.x += 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position.z -= 0.1f;
	}
	if (Input::GetKeyPress('W'))
	{
		m_Position.z += 0.1f;
	}
	GameObject::Update();
}


void Player::Draw()
{
	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	GameObject::Draw();
}