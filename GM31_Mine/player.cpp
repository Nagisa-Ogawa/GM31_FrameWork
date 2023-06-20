
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "shader.h"


void Player::Init()
{

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
		m_Transform->m_Position.x -= 0.1f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Transform->m_Position.x += 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Transform->m_Position.z -= 0.1f;
	}
	if (Input::GetKeyPress('W'))
	{
		m_Transform->m_Position.z += 0.1f;
	}
	GameObject::Update();
}


void Player::Draw()
{
	GameObject::Draw();
}