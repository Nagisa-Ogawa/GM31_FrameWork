#pragma once

#include <list>
#include <memory>
#include <typeinfo>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"

#include "editor.h"
#include "gameObject.h"

//-----------------------------------------------------
// シーン用クラス
//------------------------------------------------------
class Scene
{
private:
	std::list<std::unique_ptr<GameObject>> m_sceneObjectList[3];		// シーンに存在するオブジェクトのリスト
	std::unique_ptr<Editor> m_editor;
	std::string m_name;		// シーンの名前
	int m_registerID = 0;	// ゲームオブジェクトにセットするID
public:
	void Init();
	void Load();	// シーンを読み込んだ際に呼ばれる関数
	void Uninit();
	void Update();
	void Draw();

	// Get系関数
	std::string GetName() { return m_name; }
	Editor* GetEditor() { return m_editor.get(); }
	// Set系関数
	void SetName(std::string name) { m_name = name; }

	// GetGameObject系関数
	GameObject* GetGameObjectWithID(int ID);				// IDからゲームオブジェクトを取得する関数
	GameObject* GetGameObjectWithName(std::string name);	// 名前からゲームオブジェクトを取得する関数
	std::list<GameObject*> GetAllGameObjects();		// すべてのオブジェクトをリストで取得する関数
	std::list<GameObject*> GetMostParentObjects();	// 親のいないオブジェクトをリストで取得する関数

	void CreateObjectNode(GameObject* object);				// オブジェクトを作成した時にヒエラルキーウィンドウのノードを作成する関数
	void CallScriptStartFunc();				// シーンの実行時にアタッチされているスクリプトのStart関数を呼び出す関数
	void CheckSameName(std::string& name);	// 同じ名前のオブジェクトがあるかどうか調べ、同じ名前があったなら新しい名前をつける関数
	void CheckDestroyedObject();			// シーンのオブジェクトが破棄されているかをチェックする関数

	/// <summary>
	/// オブジェクトをシーンに追加する関数
	/// </summary>
	/// <typeparam name="T">追加するオブジェクトのクラス名</typeparam>
	/// <param name="layer">追加するオブジェクトのレイヤー（描画順）</param>
	/// <param name="name">追加するオブジェクトの名前</param>
	/// <returns>追加したオブジェクトのポインタ</returns>
	template <typename T>
	T* AddGameObject(int layer,std::string name)
	{
		std::unique_ptr<GameObject> gameObject = std::make_unique<T>();
		// transformコンポーネントは必須なためここでAddComponent
		gameObject->SetTransform(gameObject->AddComponent<Transform>());
		// 名前が使用されていないかチェック
		CheckSameName(name);
		// オブジェクトの名前を設定
		gameObject->SetName(name);
		// IDを設定
		gameObject->SetID(m_registerID);
		m_registerID++;
		// ゲームオブジェクトであることをセット
		gameObject->SetIsGameObject(true);
		gameObject->Init();
		T* TObject = (T*)gameObject.get();
		// 現在のシーンにオブジェクトを追加しているならヒエラルキーウィンドウにも追加
		CreateObjectNode(gameObject.get());
		m_sceneObjectList[layer].push_back(std::move(gameObject));
		return TObject;
	}

	/// <summary>
	/// オブジェクトを取得する関数
	/// </summary>
	/// <typeparam name="T">取得するオブジェクト名</typeparam>
	/// <returns>取得したオブジェクトのポインタ</returns>
	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (const auto& object : m_sceneObjectList[i])
			{
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					return (T*)object.get();
				}
			}
		}
		return nullptr;
	}

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList),
			CEREAL_NVP(m_editor),
			CEREAL_NVP(m_registerID)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList),
			CEREAL_NVP(m_editor),
			CEREAL_NVP(m_registerID)
		);
	}

	void CollsionTest();
};
