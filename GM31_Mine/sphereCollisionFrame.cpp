#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "shader.h"
#include "sphereCollisionFrame.h"
#include "model.h"

void SphereCollisionFrame::Init(float radius, D3DXVECTOR3 offset)
{
	m_transform->m_scale = D3DXVECTOR3(radius, radius, radius);
	m_Offset = offset;
	m_transform->m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + m_Offset;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\sphereCollision.obj");
}

void SphereCollisionFrame::Update()
{
	m_transform->m_position = m_collTransform->m_position + m_Offset;
	m_transform->m_rotation = m_collTransform->m_rotation;
}
