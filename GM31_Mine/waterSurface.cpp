#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "cameraObject.h"
#include "waterSurface.h"
#include "waterSurfaceShader.h"
#include "model.h"


void WaterSurface::Init()
{
	m_transform->m_position = D3DXVECTOR3(0.0f, 0.0f, 20.0f);
	m_transform->m_scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	m_transform->m_rotation = D3DXVECTOR3(3.14159f, 0.0f, 0.0f);
	AddComponent<WaterSurfaceShader>()->Init("Shader\\waterSurfaceVS.cso", "Shader\\waterSurfacePS.cso");
	AddComponent<Model>()->Init("asset\\model\\WaterSurface.obj");
}

void WaterSurface::Update()
{
	GameObject::Update();
}
