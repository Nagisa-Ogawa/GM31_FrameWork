#include "main.h"
#include "editor.h"
#include "renderer.h"
#include "manager.h"

#include "editorCameraObject.h"

void Editor::Init()
{
	AddEditorObject<EditorCameraObject>(0, "EditorCamera");
}

void Editor::Load()
{
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_editorObjectList[i]) {
			gameObject->Load();
		}
	}
}

void Editor::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			gameObject->Uninit();
		}
		m_editorObjectList[i].clear();
	}
}

void Editor::Update()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			// アクティブフラグがONなら更新処理をする
			if (gameObject->GetActive())
				gameObject->Update();
		}
	}
}

void Editor::Draw()
{
	// 描画をする前にローカル行列からワールド行列を作成
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			auto transform = gameObject->GetTransform();
			// 親のないオブジェクトのワールド行列作成関数を呼び、子オブジェクトへ連鎖させる
			if (transform->GetParent() == nullptr)
				transform->MakeWorldMatrix(nullptr);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i])
		{
			// アクティブフラグがONなら描画する
			if (gameObject->GetActive())
				gameObject->Draw();
		}
	}
}

void Editor::CheckDestroyedObject()
{
	for (int i = 0; i < 3; i++)
	{
		// オブジェクトの破棄フラグをチェック
		m_editorObjectList[i].remove_if([](const auto& object)
		{return object->Destroy(); });
	}
	// 破棄されないオブジェクトのコンポーネントの破棄フラグをチェック
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_editorObjectList[i]) {
			gameObject->CheckDestroyedComponent();
		}
	}
}
