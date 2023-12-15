#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "titleCameraObject.h"
#include "titleSky.h"
#include "shader.h"
#include "model.h"

void TitleSky::Init()
{
	m_transform->m_scale = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	AddComponent<Shader>()->Init("Shader\\unlitTextureVS.cso", "Shader\\unlitTexturePS.cso");
	AddComponent<Model>()->Init("asset\\model\\Sky.obj");
}

void TitleSky::Update()
{
	Scene* scene = Manager::GetInstance()->GetTitle();
	TitleCameraObject* camera = scene->GetGameObject<TitleCameraObject>();
	m_transform->m_position = camera->GetTransform()->m_position;
}
