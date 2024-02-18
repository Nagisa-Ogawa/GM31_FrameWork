
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "shader.h"
#include "boxCollision.h"
#include "fieldObject.h"
#include "meshField.h"
#include "script.h"

void Humanoid::Init()
{
	m_transform->m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	auto coll = AddComponent<BoxCollision>(); 
	coll->SetSize(D3DXVECTOR3(0.5f, 1.0f, 0.5f));
	coll->SetOffset(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	AddComponent<Shader>()->Init("Assets\\Shaders\\vertexLightingVS.cso", "Assets\\Shaders\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("Assets\\Models\\Bot.obj",true);
	AddComponent<Script>()->Init("Assets\\Scripts\\move.lua");
}

