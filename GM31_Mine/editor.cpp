#include "main.h"
#include "editor.h"
#include "renderer.h"
#include "manager.h"

#include "editorCameraObject.h"

void Editor::Init()
{
	AddGameObject<EditorCameraObject>(0, "EditorCamera");
}

void Editor::Uninit()
{
	Scene::Uninit();
}

void Editor::Update()
{
	Scene::Update();
}
