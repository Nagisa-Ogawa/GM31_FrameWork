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
	// ---------------------
	// デバッグ用
	// ---------------------
	AddGameObject<CameraObject>(0, "Camera");
	auto player = AddGameObject<Player>(1, "Player");
	auto box = AddGameObject<Box>(1, "box");
	box->GetTransform()->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 15.0f);
	auto box1 = AddGameObject<Box>(1, "box1");
	box1->GetTransform()->m_localPosition = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	box1->GetTransform()->SetParent(box->GetTransform());

	AddGameObject<Sky>(1, "Sky");


	auto meshField = AddGameObject<MeshField>(1, "Filed");

}

void Scene::Load()
{
	for (int i = 0; i < 3; i++) {
		for (auto gameObject : m_sceneObjectList[i]){
			gameObject->Load();
		}
	}
}

void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (auto gameObject : m_sceneObjectList[i])
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
		for (auto gameObject : m_sceneObjectList[i])
		{
			// アクティブフラグがONなら更新処理をする
			if (gameObject->GetActive())
				gameObject->Update();
		}
		m_sceneObjectList[i].remove_if([](std::shared_ptr<GameObject> object)
		{return object->Destroy(); });	// ラムダ式
	}
}

void Scene::Draw()
{
	// 描画をする前にローカル行列からワールド行列を作成
	for (Transform* transform : m_parentObjectList)
	{
		// 親のないオブジェクトのワールド行列作成関数を呼び子オブジェクトへ連鎖させる
		transform->MakeWorldMatrix(nullptr);
	}
	for (int i = 0; i < 3; i++)
	{
		for (auto gameObject : m_sceneObjectList[i])
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
			it = std::find_if(it, m_sceneObjectList[i].end(), [](std::shared_ptr<GameObject> obj) {return obj->GetActive(); });
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
		for (auto gameObject : m_sceneObjectList[i])
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

