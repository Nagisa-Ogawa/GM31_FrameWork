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
	AddComponent<Shader>()->Init("Assets\\Shaders\\vertexLightingVS.cso", "Assets\\Shaders\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("Assets\\Models\\boxCollision.obj",false);
}


void BoxCollisionFrame::Draw()
{
	// ���[���h�ϊ��s��𓖂��蔻��̂��Ă���I�u�W�F�N�g�Ɠ����s��ɂ���
	GetTransform()->SetWorldMatrix(m_collision->GetWorldMatrix());
	GameObject::Draw();
}

