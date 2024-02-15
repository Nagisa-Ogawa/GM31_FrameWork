#include "main.h"
#include "scene.h"
#include "script.h"

#include "renderer.h"
#include "cameraObject.h"
#include "player.h"
#include "box.h"
#include "meshField.h"
#include "sky.h"


void Scene::Init()
{
	m_editor = std::make_unique<Editor>();
	m_editor->Init();
	// ---------------------
	// デバッグ用
	// ---------------------
	AddGameObject<CameraObject>(0, "Camera");
	//auto player = AddGameObject<Player>(1, "Player");
	//auto box = AddGameObject<Box>(1, "box");
	//box->GetTransform()->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 15.0f);
	//auto box1 = AddGameObject<Box>(1, "box1");
	//box1->GetTransform()->m_localPosition = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	//box1->GetTransform()->SetParent(box->GetTransform());
	//auto meshField = AddGameObject<MeshField>(1, "Filed");

	AddGameObject<Sky>(1, "Sky");

}

void Scene::Load()
{
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_sceneObjectList[i]){
			gameObject->Load();
		}
	}
	m_editor->Load();
}

void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			gameObject->Uninit();
		}
		m_sceneObjectList[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			// アクティブフラグがONなら更新処理をする
			if (gameObject->GetActive())
				gameObject->Update();
		}
	}
}

void Scene::Draw()
{
	// 描画をする前にローカル行列からワールド行列を作成
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			auto transform = gameObject->GetTransform();
			// 親のないオブジェクトのワールド行列作成関数を呼び、子オブジェクトへ連鎖させる
			if (transform->GetParent() == nullptr)
				transform->MakeWorldMatrix(nullptr);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			// アクティブフラグがONなら描画する
			if (gameObject->GetActive())
				gameObject->Draw();
		}
	}
}


/// <summary>
/// シーンに存在するオブジェクトの個数を取得する関数
/// </summary>
/// <returns>オブジェクトの個数</returns>
size_t Scene::GetGameObjectCount()
{
	size_t count = 0;
	for (int i = 0; i < 3; i++) {
		count += m_sceneObjectList[i].size();
	}
	return count;
}


/// <summary>
/// アクティブなオブジェクトの個数を取得する関数
/// </summary>
/// <returns>オブジェクトの個数</returns>
int Scene::GetActiveGameObjectCount()
{
	int count = 0;
	for (int i = 0; i < 3; i++) {
		auto it = m_sceneObjectList[i].begin();
		// すべての要素を検索し終わるまでループ
		while (true) {
			it = std::find_if(it, m_sceneObjectList[i].end(), [](const auto& obj) {return obj->GetActive(); });
			if (it == m_sceneObjectList[i].end()) {
				break;
			}
			count++;
			it++;
		}
	}
	return count;
}


/// <summary>
/// すべてのオブジェクトをリストで取得する関数
/// </summary>
/// <returns>オブジェクトのリスト</returns>
std::list<GameObject*> Scene::GetAllGameObjects()
{
	std::list<GameObject*> objList;
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			objList.push_back(gameObject.get());
		}
	}
	return objList;
}


/// <summary>
/// シーンの実行時にスクリプトのStart関数を呼び出す関数
/// </summary>
void Scene::CallScriptStartFunc()
{
	auto objectList = GetAllGameObjects();
	for (auto object : objectList) {
		Script* script = object->GetComponent<Script>();
		if (script == nullptr)	continue;
		script->Start();
	}
}

void Scene::CheckDestroyedObject()
{
	for (int i = 0; i < 3; i++)
	{
		// オブジェクトの破棄フラグをチェック
		m_sceneObjectList[i].remove_if([](const auto& object)
		{return object->Destroy(); });
	}
	// 破棄されないオブジェクトのコンポーネントの破棄フラグをチェック
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i]) {
			gameObject->CheckDestroyedComponent();
		}
	}
	m_editor->CheckDestroyedObject();
}


