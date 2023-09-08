#include "main.h"
#include "manager.h"
#include "result.h"
#include "title.h"
#include "renderer.h"

#include "input.h"
#include "polygon2D.h"
void Result::Init()
{
	auto logo = AddGameObject<Polygon2D>(0);
	logo->Init(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT), "asset\\texture\\result.png");
}

void Result::Update()
{
	Scene::Update();
	if (Input::GetKeyPress(VK_SPACE))
	{
		Manager::GetInstance()->SetScene<Title>();
	}
}
