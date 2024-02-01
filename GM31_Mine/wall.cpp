//#include "main.h"
//#include "renderer.h"
//#include "wall.h"
//#include "plane.h"
//#include "shader.h"
//#include "quadCollision.h"
//
//void Wall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
//{
//	AddComponent<Shader>()->Init("Shader\\unlitTextureVS.cso", "Shader\\unlitTexturePS.cso");
//	AddComponent<Plane>()->Init(m_transform->m_worldPosition, D3DXVECTOR2(5.0f, 5.0f), "asset/texture/wall.jpg");
//	m_transform->m_worldPosition = pos;
//	m_transform->m_worldRotation = rot;
//	AddComponent<QuadCollision>()->Init(m_transform->GetUp(), D3DXVECTOR2(5.0f, 5.0f), false);
//}
//
//void Wall::Uninit()
//{
//	GameObject::Uninit();
//}
//
//void Wall::Update()
//{
//	GameObject::Update();
//}
//
//void Wall::Draw()
//{
//	GameObject::Draw();
//}
