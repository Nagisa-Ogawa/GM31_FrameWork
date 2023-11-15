#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "shader.h"
#include "boxCollisionFrame.h"
#include "model.h"

void BoxCollisionFrame::Init(D3DXVECTOR3 scale, D3DXVECTOR3 offset)
{
	m_transform->m_scale = scale;
	m_Offset = offset;
	m_transform->m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + m_Offset;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\boxCollision.obj");
}

void BoxCollisionFrame::Update()
{
	m_transform->m_position = m_collTransform->m_position + m_Offset;
	m_transform->m_rotation = m_collTransform->m_rotation;
	if (m_collTransform->GetGameObject()->GetActive() == false&&GetActive()==true) {
		SetActive(false);
	}
	
}
