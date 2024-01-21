#include "main.h"
#include "renderer.h"
#include "model.h"
#include "shader.h"
#include "box.h"
#include "boxCollision.h"

void Box::Init()
{
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform->m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\box.obj");
	AddComponent<BoxCollision>()->Init(m_transform->m_localScale, D3DXVECTOR3(0.0f, 1.0f, 0.0f), false);
}
