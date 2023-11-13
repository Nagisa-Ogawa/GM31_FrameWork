#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "cameraObject.h"
#include "sky.h"
#include "shader.h"
#include "model.h"

void Sky::Init()
{
	m_transform->m_Scale=D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	AddComponent<Shader>()->Init("Shader\\unlitTextureVS.cso", "Shader\\unlitTexturePS.cso");
	AddComponent<Model>()->Init("asset\\model\\Sky.obj");
}

void Sky::Update() 
{
	//Scene* scene = Manager::GetInstance()->GetScene();
	//CameraObject* camera = scene->GetGameObject<CameraObject>();
	//m_Transform->m_Position = camera->GetTransform()->m_Position;
}
