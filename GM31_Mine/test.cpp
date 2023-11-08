#include "main.h"
#include "manager.h"
#include "test.h"
#include "game.h"
#include "renderer.h"

#include "polygon2D.h"

void Test::Init()
{
	AddGameObject<Polygon2D>(0)->Init(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), D3DXVECTOR2(512, 512));
}

void Test::Update()
{
}
