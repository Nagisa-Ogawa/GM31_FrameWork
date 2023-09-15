#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "shader.h"
#include "boxCollisionFrame.h"
#include "model.h"

void BoxCollisionFrame::Init(D3DXVECTOR3 scale, D3DXVECTOR3 offset)
{
	m_Transform->m_Scale = scale;
	m_Offset = offset;
	m_Transform->m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + m_Offset;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\boxCollision.obj");
}

void BoxCollisionFrame::Update()
{
	m_Transform->m_Position = m_collTransform->m_Position + m_Offset;
	m_Transform->m_Rotation = m_collTransform->m_Rotation;
	if (m_collTransform->GetGameObject()->GetActive() == false&&GetActive()==true) {
		SetActive(false);
	}
	
}
