#include "main.h"
#include "renderer.h"
#include "model.h"
#include "shader.h"
#include "box.h"
#include "boxCollision.h"
#include "rigidBody.h"

void Box::Init()
{
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_transform->m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	AddComponent<Shader>()->Init("Assets\\Shaders\\vertexLightingVS.cso", "Assets\\Shaders\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("Assets\\Models\\box.obj", true);
	auto coll = AddComponent<BoxCollision>();
	coll->SetOffset(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	AddComponent<RigidBody>();
}
