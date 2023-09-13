#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "shader.h"
#include "sphereCollisionFrame.h"
#include "model.h"

void SphereCollisionFrame::Init(float radius, D3DXVECTOR3 offset)
{
	m_Transform->m_Scale = D3DXVECTOR3(radius, radius, radius);
	m_Offset = offset;
	m_Transform->m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + m_Offset;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\sphereCollision.obj");
}

void SphereCollisionFrame::Update()
{
	m_Transform->m_Position = m_collTransform->m_Position + m_Offset;
	m_Transform->m_Rotation = m_collTransform->m_Rotation;
}
