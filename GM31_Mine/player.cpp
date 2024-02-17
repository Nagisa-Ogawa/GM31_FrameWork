
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "shader.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "quadCollision.h"
#include "animationModel.h"
#include "camera.h"
#include "cameraObject.h"
#include "Ray.h"
#include "sphereCollision.h"
#include "sphereCollisionFrame.h"
#include "boxCollisionFrame.h"
#include "meshField.h"
#include "input.h"
#include "script.h"
#include "model.h"

void Player::Init()
{
	m_transform->m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	auto coll = AddComponent<BoxCollision>(); 
	coll->SetSize(D3DXVECTOR3(0.5f, 1.0f, 0.5f));
	coll->SetOffset(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	AddComponent<Shader>()->Init("Assets\\Shaders\\vertexLightingVS.cso", "Assets\\Shaders\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("Assets\\Models\\Bot.obj",true);
	//m_Model = AddComponent<AnimationModel>();
	//m_Model->Init("asset\\model\\Bot.fbx");
	//m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	//m_Model->LoadAnimation("asset\\model\\Bot_Idle.fbx", "Idle");

	//m_AnimationName = "Idle";
	//m_NextAnimationName = "Idle";

	AddComponent<Script>()->Init("Assets\\Scripts\\testA.lua");
}

void Player::Update()
{

	float groundHeight;
	auto meshField = Manager::GetInstance()->GetScene()->GetGameObject<MeshField>();
	groundHeight = meshField->GetHeight(m_transform->m_localPosition);

	m_transform->m_localPosition.y = groundHeight;

	GameObject::Update();

}

