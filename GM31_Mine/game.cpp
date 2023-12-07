#include "main.h"
#include "game.h"
#include "renderer.h"
#include "manager.h"

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
#include "treeFactory.h"

void Game::Init()
{
	AddGameObject<CameraObject>(0,"Camera");
	// AddGameObject<Field>(1);
	AddGameObject<Wall>(1,"Wall01")->Init(D3DXVECTOR3(-20.0f, 2.5f, 5.0f), D3DXVECTOR3(0.0f, 0.0f, -3.14f / 2.0f));
	AddGameObject<Wall>(1,"Wall02")->Init(D3DXVECTOR3(-25.0f, 2.5f, 0.0f), D3DXVECTOR3(0.0f, 3.14f / 2.0f, -3.14f / 2.0f));

	auto enemy = EnemyFactory::GetInstance()->ActiveObject("Enemy");
	enemy->GetTransform()->m_position = D3DXVECTOR3(0.0f, 0.0f, 15.0f);

	AddGameObject<Sky>(1,"Sky");

	
	auto meshField = AddGameObject<MeshField>(1,"Filed");

	for (int i = 0; i < 20; i++) {
		auto tree = TreeFactory::GetInstance()->ActiveObject("tree"+std::to_string(i));

		D3DXVECTOR3 pos;
		pos.x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.z = (float)rand() / RAND_MAX * 100.0f - 50.0f;
		pos.y = meshField->GetHeight(pos)-1.5f;
		tree->GetTransform()->m_position = pos;
	}
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
}
