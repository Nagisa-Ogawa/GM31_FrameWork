#include "main.h"
#include "manager.h"
#include "title.h"
#include "game.h"
#include "renderer.h"

#include "input.h"
#include "polygon2D.h"
void Title::Init()
{
	Input::Init();
	auto logo = AddGameObject<Polygon2D>(0);
	logo->Init(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "asset\\texture\\title.png");
}

void Title::Update()
{
	Scene::Update();
	if (Input::GetKeyPress(VK_SPACE))
	{
		Manager::GetInstance()->SetScene<Game>();
	}
}
