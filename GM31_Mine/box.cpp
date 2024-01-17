#include "main.h"
#include "renderer.h"
#include "model.h"
#include "shader.h"
#include "box.h"

void Box::Init()
{
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\box.obj");
}
