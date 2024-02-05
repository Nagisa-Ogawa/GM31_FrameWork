#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "shader.h"
#include "boxCollisionFrame.h"
#include "boxCollision.h"
#include "model.h"


void BoxCollisionFrame::Init(BoxCollision* collision)
{
	m_collision = collision;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\boxCollision.obj",false);
}


void BoxCollisionFrame::Draw()
{
	GetTransform()->SetWorldMatrix(m_collision->GetWorldMatrix());
	GameObject::Draw();
}

