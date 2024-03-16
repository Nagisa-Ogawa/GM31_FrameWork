#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "luaManager.h"
#include "myImGuiManager.h"
#include "hierarchyGui.h"
#include "script.h"
#include "renderer.h"
#include "cameraObject.h"
#include "sky.h"
#include "CollisionManager.h"
#include "boxCollision.h"

void Scene::Init()
{
	m_editor = std::make_unique<Editor>();
	m_editor->Init();
	AddGameObject<CameraObject>(0, "MainCamera");
	AddGameObject<Sky>(1, "Sky");

	// ヒエラルキーウィンドウのオブジェクト木構造を初期化
	auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
	hierarchy->InitObjectTree();
}

void Scene::Load()
{
	// シーンにあるオブジェクトのLoad関数を呼び出す
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_sceneObjectList[i]){
			gameObject->Load();
		}
	}
	// ゲームオブジェクトが全てロードされたなら親オブジェクトをセット
	for (int i = 0; i < 3; i++) {
		for (const auto& gameObject : m_sceneObjectList[i]) {
			gameObject->GetTransform()->LoadParent();
		}
	}
	// エディタ用シーンのLoad関数を呼び出す
	m_editor->Load();
	// ヒエラルキーウィンドウのオブジェクト木構造を初期化
	auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
	hierarchy->InitObjectTree();
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
	m_editor->Uninit();
	LuaManager::GetInstance()->ClearScriptList();
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
	// CollsionTest();
}


void Scene::CollsionTest() {
	auto player = GetGameObjectWithName("Humanoid");
	auto enemy = GetGameObjectWithName("Enemy");
	auto playerColl = player->GetComponent<BoxCollision>();
	auto enemyColl = enemy->GetComponent<BoxCollision>();
	float l = -1;
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (CollisionManager::GetInstance()->Collision_BoxToBox(playerColl, enemyColl, &l, &vec)) {
		
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
/// IDからゲームオブジェクトを取得する関数
/// </summary>
/// <param name="ID"></param>
/// <returns></returns>
GameObject* Scene::GetGameObjectWithID(int ID)
{
	// IDが-1なら無効
	if (ID == -1) return nullptr;
	// オブジェクトのリストからIDが同じオブジェクトを探す
	for (int i = 0; i < 3; i++) {
		auto it = std::find_if(m_sceneObjectList[i].begin(), m_sceneObjectList[i].end(),
			[&ID](const auto& obj) {return obj->GetID() == ID; });
		if (it != m_sceneObjectList[i].end()) {
			// 一致したオブジェクトがあったなら返す
			return (it->get());
		}
	}
	// ないならnullを返す
	return nullptr;
}


/// <summary>
/// 名前からゲームオブジェクトを取得する関数
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
GameObject* Scene::GetGameObjectWithName(std::string name)
{
	// 名前が空なら無効
	if (name == "") return nullptr;
	// オブジェクトのリストからIDが同じオブジェクトを探す
	for (int i = 0; i < 3; i++) {
		auto it = std::find_if(m_sceneObjectList[i].begin(), m_sceneObjectList[i].end(),
			[&name](const auto& obj) {return obj->GetName() == name; });
		if (it != m_sceneObjectList[i].end()) {
			// 一致したオブジェクトがあったなら返す
			return it->get();
		}
	}
	// ないならnullを返す
	return nullptr;
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
/// 親がいない（一番親）オブジェクトをリストにして取得する関数
/// </summary>
/// <returns></returns>
std::list<GameObject*> Scene::GetMostParentObjects()
{
	std::list<GameObject*> objList;
	for (int i = 0; i < 3; i++)
	{
		for (const auto& gameObject : m_sceneObjectList[i])
		{
			// 親がいないなら（一番親なら）
			if (gameObject->GetTransform()->GetParent() == nullptr) {
				objList.push_back(gameObject.get());
			}
		}
	}
	return objList;
}


void Scene::CreateObjectNode(GameObject* object)
{
	if (Manager::GetInstance()->GetScene() == this) {
		auto hierarchy = MyImGuiManager::GetInstance()->GetImGui<HierarchyGui>();
		hierarchy->AddObjectNode(object);
	}
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

/// <summary>
/// 同じ名前のオブジェクトがあるかどうか調べ、同じ名前があったなら新しい名前をつける関数
/// </summary>
/// <param name="name">オブジェクトの名前</param>
void Scene::CheckSameName(std::string& name)
{
	auto objectList = GetAllGameObjects();
	// 同じ名前のオブジェクトない名前になるまでループ
	std::string newName = name;
	int count = 0;
	while (true) {
		// 同じ名前のオブジェクトがあるか探す
		auto it = std::find_if(objectList.begin(), objectList.end(), 
			[&newName](GameObject* object) { return object->GetName() == newName; });
		if (it != objectList.end()) {
			count++;
			// 同じ名前があったなら名前を変える
			newName = name + "(" + std::to_string(count) + ")";
		}
		else {
			// 同じ名前のオブジェクトはないので終了
			name = newName;
			break;
		}
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


