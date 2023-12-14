#include "main.h"
#include "title.h"
#include "renderer.h"
#include "manager.h"

#include "input.h"
#include "polygon2D.h"
#include "field.h"
#include "cameraObject.h"


void Title::Init()
{
	AddGameObject<CameraObject>(0, "camera");
	AddGameObject<Field>(1, "stage");
	AddGameObject<Polygon2D>(2, "logo")->Init(D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f), D3DXVECTOR2(800, 600), "asset/texture/floor01.jpg");
}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	if (Input::GetKeyPress(VK_RETURN)) {
		Manager::GetInstance()->SetGameMode(GAME_MODE::INGAME);
	}
	Scene::Update();
}
