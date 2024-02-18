#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "cameraObject.h"
#include "sky.h"
#include "shader.h"
#include "model.h"
#include "script.h"

void Sky::Init()
{
	m_transform->m_localScale=D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	AddComponent<Shader>()->Init("Assets\\Shaders\\unlitTextureVS.cso", "Assets\\Shaders\\unlitTexturePS.cso");
	AddComponent<Model>()->Init("Assets\\Models\\Sky.obj",false);
}
