#include "main.h"
#include "scene.h"
#include "script.h"

void Scene::Uninit()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* gameObject : m_GameObject[i])
		{
			gameObject->Uninit();
			delete gameObject;
		}
		m_GameObject[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* gameObject : m_GameObject[i])
		{
			// アクティブフラグがONなら更新処理をする
			if (gameObject->GetActive())
				gameObject->Update();
		}
		m_GameObject[i].remove_if([](GameObject* object)
		{return object->Destroy(); });	// ラムダ式
	}
}

void Scene::Draw()
{
	for (int i = 0; i < 3; i++)
	{
		for (GameObject* gameObject : m_GameObject[i])
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
		count += m_GameObject[i].size();
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
		auto it = m_GameObject[i].begin();
		// すべての要素を検索し終わるまでループ
		while (true) {
			it = std::find_if(it, m_GameObject[i].end(), [](GameObject* obj) {return obj->GetActive(); });
			if (it == m_GameObject[i].end()) {
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
		for (GameObject* gameObject : m_GameObject[i])
		{
			objList.push_back(gameObject);
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
