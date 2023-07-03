
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"

#include "transform.h"
#include "player.h"


void TransForm::Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Scale = scale;
}

void TransForm::Uninit()
{
}

void TransForm::Update()
{
}

void TransForm::Draw()
{
	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	Scene* scene = Manager::GetInstance()->GetScene();
	Player* player = scene->GetGameObject<Player>();
	if (m_GameObject == player)
	{
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	}
	else
	{
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	}
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);
}
