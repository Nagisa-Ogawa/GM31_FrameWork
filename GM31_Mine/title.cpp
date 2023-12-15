
#include "main.h"
#include "title.h"
#include "renderer.h"
#include "manager.h"

#include "input.h"
#include "polygon2D.h"
#include "field.h"
#include "titleCameraObject.h"
#include "titleSky.h"
#include "waterSurface.h"
#include "Ball.h"
#include "ballFactory.h"


void Title::Init()
{

	AddGameObject<TitleCameraObject>(0, "camera");
	AddGameObject<TitleSky>(1, "sky");
	for (int i = 0; i < 10; i++) {
		BallFactory::GetInstance()->ActiveObject("ball");
		//auto ball = AddGameObject<Ball>(1, "ball");
		//ball->Init(D3DXVECTOR3(rand() % 50, 2.0f, rand() % 30), D3DXVECTOR2(0.05f, 0.05f), 1.0f);
	}
	AddGameObject<WaterSurface>(1, "water");
	AddGameObject<Polygon2D>(2, "logo")->Init(D3DXVECTOR2(1000.0f, SCREEN_HEIGHT / 2.0f-100.0f), D3DXVECTOR2(900, 180), "asset/texture/titlelogo.png");
	AddGameObject<Polygon2D>(2, "logo")->Init(D3DXVECTOR2(950.0f, SCREEN_HEIGHT / 2.0f+ 200.0f), D3DXVECTOR2(352, 54), "asset/texture/titleEnter.png");


}

void Title::Uninit()
{
	BallFactory::GetInstance()->Uninit();
	Scene::Uninit();
}

void Title::Update()
{
	if (Input::GetKeyPress(VK_RETURN)) {
		Manager::GetInstance()->SetGameMode(GAME_MODE::INGAME);
	}
	Scene::Update();
	BallFactory::GetInstance()->CollisionBallToBall();
}
