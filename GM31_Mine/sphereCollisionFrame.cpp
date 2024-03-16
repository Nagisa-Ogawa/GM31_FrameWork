#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "shader.h"
#include "sphereCollisionFrame.h"
#include "model.h"

void SphereCollisionFrame::Init(float radius, D3DXVECTOR3 offset)
{
	m_transform->m_worldScale = D3DXVECTOR3(radius, radius, radius);
	m_Offset = offset;
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + m_Offset;
	AddComponent<Shader>()->Init("Assets\\Shaders\\vertexLightingVS.cso", "Assets\\Shaders\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("Assets\\Models\\sphereCollision.obj",false);
}

void SphereCollisionFrame::Update()
{
	m_transform->m_localPosition = m_collTransform->m_localPosition + m_Offset;
	m_transform->m_localQuaternion = m_collTransform->m_localQuaternion;
}
