
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "shader.h"
#include "bulletFactory.h"
#include "bullet.h"


void Player::Init()
{

	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\player.obj");
}

void Player::Update()
{
	// ŽOlÌ—p
	if (Input::GetKeyPress('A'))
	{
		m_Transform->m_Position -= m_Transform->GetRight()*0.1f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Transform->m_Position += m_Transform->GetRight()*0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Transform->m_Position -= m_Transform->GetForward()*0.1f;
	}
	if (Input::GetKeyPress('W'))
	{
		m_Transform->m_Position += m_Transform->GetForward()*0.1f;
	}
	if (Input::GetKeyPress('Q'))
	{
		m_Transform->m_Rotation.y -= 0.1f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Transform->m_Rotation.y += 0.1f;
	}

	if (Input::GetKeyTrigger(VK_SPACE)) {
		Bullet* pBullet = BulletFactory::GetInstance()->GetBullet();
		pBullet->SetDirection(m_Transform->GetForward());
		pBullet->GetTransform()->m_Position = m_Transform->m_Position;
		pBullet->SetStartPos(m_Transform->m_Position);
	}
}

