//#include "main.h"
//#include "game.h"
//#include "renderer.h"
//#include "manager.h"
//
//#include "CollisionManager.h"
//#include "cameraObject.h"
//#include "field.h"
//#include "player.h"
//#include "polygon2D.h"
//#include "enemy.h"
//#include "wall.h"
//#include "sky.h"
//#include "score.h"
//#include "boxCollisionFrame.h"
//#include "sphereCollisionFrame.h"
//#include "meshField.h"
//#include "rock.h"
//#include "tree.h"
//#include "box.h"
//
//void Game::Init()
//{
//	// AddGameObject<Field>(1);
//	AddGameObject<CameraObject>(0, "Camera");
//	auto player = AddGameObject<Player>(1, "Player");
//	AddGameObject<Wall>(1,"Wall01")->Init(D3DXVECTOR3(-20.0f, 2.5f, 5.0f), D3DXVECTOR3(0.0f, 0.0f, -3.14f / 2.0f));
//	AddGameObject<Wall>(1,"Wall02")->Init(D3DXVECTOR3(-25.0f, 2.5f, 0.0f), D3DXVECTOR3(0.0f, 3.14f / 2.0f, -3.14f / 2.0f));
//
//	auto box = AddGameObject<Box>(1, "box");
//	box->GetTransform()->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 15.0f);
//	auto box1 = AddGameObject<Box>(1, "box1");
//	box1->GetTransform()->m_localPosition = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
//	box1->GetTransform()->SetParent(box->GetTransform());
//
//	AddGameObject<Sky>(1,"Sky");
//
//	
//	auto meshField = AddGameObject<MeshField>(1,"Filed");
//
//}
//
//void Game::Uninit()
//{
//	Scene::Uninit();
//	CollisionManager::GetInstance()->Uninit();
//}
//
//void Game::Update()
//{
//	Scene::Update();
//}
