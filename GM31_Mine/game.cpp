#include "main.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"
#include "result.h"

#include "cameraObject.h"
#include "field.h"
#include "player.h"
#include "polygon2D.h"
#include "enemy.h"
#include "bulletFactory.h"
#include "enemyFactory.h"
#include "wall.h"
#include "sky.h"
#include "score.h"
#include "boxCollisionFrame.h"
#include "sphereCollisionFrame.h"
#include "meshField.h"
#include "rock.h"
#include "tree.h"

void Game::Init()
{
	AddGameObject<CameraObject>(0);
	// AddGameObject<Field>(1);
	auto player=AddGameObject<Player>(1);
	AddGameObject<Wall>(1)->Init(D3DXVECTOR3(-20.0f,2.5f,5.0f), D3DXVECTOR3(0.0f, 0.0f, -3.14f / 2.0f));
	AddGameObject<Wall>(1)->Init(D3DXVECTOR3(-25.0f, 2.5f, 0.0f), D3DXVECTOR3(0.0f, 3.14f / 2.0f, -3.14f / 2.0f));

	AddGameObject<Sky>(1);

	Enemy* pEnemy1 = EnemyFactory::GetInstance()->ActiveObject();
	pEnemy1->GetTransform()->m_Position = D3DXVECTOR3(5.0f, 0.0f, 15.0f);
	pEnemy1->GetTransform()->m_Rotation = D3DXVECTOR3(0.0f, 3.14f/3.0f, 0.0f);

	Enemy* pEnemy2 = EnemyFactory::GetInstance()->ActiveObject();
	pEnemy2->GetTransform()->m_Position = D3DXVECTOR3(0.0f, 0.0f, 14.0f);
	pEnemy2->GetTransform()->m_Rotation = D3DXVECTOR3(0.0f, 1.1f, 0.0f);

	Enemy* pEnemy3 = EnemyFactory::GetInstance()->ActiveObject();
	pEnemy3->GetTransform()->m_Position = D3DXVECTOR3(-3.0f, 0.0f, 16.0f);
	pEnemy3->GetTransform()->m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	auto meshField = AddGameObject<MeshField>(1);

	for (int i = 0; i < 20; i++) {
		Tree* tree = AddGameObject<Tree>(1);
		tree->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(4.0f, 8.0f));

		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = meshField->GetHeight(pos);
		tree->GetTransform()->m_Position = pos;
	}
	//for (int i = 0; i < 20; i++) {
	//	Rock* rock = AddGameObject<Rock>(1);

	//	D3DXVECTOR3 pos;
	//	pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
	//	pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
	//	pos.y = meshField->GetHeight(pos);
	//	rock->GetTransform()->m_Position = pos;
	//}
	AddGameObject<Score>(2);
	// AddGameObject<Polygon2D>(2);
}

void Game::Uninit()
{
	Scene::Uninit();
	EnemyFactory::GetInstance()->Clear();
	BulletFactory::GetInstance()->Clear();
}

void Game::Update()
{
	Scene::Update();
	if (GetGameObject<Score>()->GetScore() >= 3) {
		Manager::GetInstance()->SetScene<Result>();
	}
}
