#include "main.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "rock.h"

void Rock::Init()
{
	AddComponent<Shader>()->Init("Shader\\unlitTextureVS.cso", "Shader\\unlitTexturePS.cso");
	AddComponent<Model>()->Init("asset\\model\\rock_base_LP.obj");
	m_transform->m_Position = D3DXVECTOR3(2.0f, 0.0f, 2.0f);
	m_transform->m_Scale = D3DXVECTOR3(0.005f, 0.005f, 0.005f);
}

void Rock::Update()
{
}
