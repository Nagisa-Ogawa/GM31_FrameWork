#include "main.h"
#include "editor.h"
#include "renderer.h"
#include "manager.h"

#include "cameraObject.h"

void Editor::Init()
{
	AddGameObject<CameraObject>(0, "Camera");
}

void Editor::Uninit()
{
	Scene::Uninit();
}

void Editor::Update()
{
	Scene::Update();
}
