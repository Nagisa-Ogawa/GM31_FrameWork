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

void Game::Init()
{
	AddGameObject<CameraObject>(0);
	AddGameObject<Field>(1);
	AddGameObject<Player>(1);
	AddGameObject<Wall>(1);
	AddGameObject<Sky>(1);
	//for (int i = 0; i < 10; i++) {
	//	Enemy* pEnemy = EnemyFactory::GetInstance()->ActiveObject();
	//	pEnemy->GetTransform()->m_Position=D3DXVECTOR3(-15.0f + i * 3.0f, 0.0f, 6.0f);
	//	pEnemy->GetTransform()->m_Rotation = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	//}

	// AddGameObject<Polygon2D>(2);
}

void Game::Uninit()
{
	Scene::Uninit();
	BulletFactory::GetInstance()->Uninit();
	EnemyFactory::GetInstance()->Uninit();
}

void Game::Update()
{
	Scene::Update();
}
