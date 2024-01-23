#pragma once

#include <list>
#include <memory>
#include <vector>
#include <typeinfo>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"

#include "gameObject.h"

class Scene
{
protected:
	std::list<std::shared_ptr<GameObject>> m_sceneObjectList[3];		// シーンに存在するオブジェクトのリスト
	std::list<Transform*> m_parentObjectList;		// シーンに直接配置されている親のいないゲームオブジェクトのtransform
	std::string m_name;		// シーンの名前
public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	std::string GetName() { return m_name; }
	size_t GetGameObjectCount();	// シーンに存在するオブジェクトの個数を取得する関数
	int GetActiveGameObjectCount();	// アクティブなオブジェクトの個数を取得する関数
	std::list<GameObject*> GetAllGameObjects();		// すべてのオブジェクトをリストで取得する関数

	void SetName(std::string name) { m_name = name; }

	void CallScriptStartFunc();		// シーンの実行時にアタッチされているスクリプトのStart関数を呼び出す関数
	void AddParentObject(Transform* transform) { m_parentObjectList.push_back(transform); }
	void DeleteParentObject(Transform* transform) { m_parentObjectList.remove(transform); }

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
		std::shared_ptr<GameObject> gameObject = std::make_shared<T>();
		m_sceneObjectList[layer].push_back(gameObject);
		// transformコンポーネントは必須なためここでAddComponent
		gameObject->SetTransform(gameObject->AddComponent<Transform>());
		// オブジェクトの名前を設定
		gameObject->SetName(name);
		// 追加したオブジェクトは親を持たないのでシーンのオブジェクトとして登録
		m_parentObjectList.push_back(gameObject->GetTransform());
		gameObject->Init();

		return (T*)gameObject.get();
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
			for (auto object : m_sceneObjectList[i])
			{
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					return (T*)object.get();
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// アクティブな指定されたオブジェクトを取得する関数
	/// </summary>
	/// <typeparam name="T">取得するオブジェクトのクラス名</typeparam>
	/// <returns>取得したオブジェクトのポインタ</returns>
	template <typename T>
	T* GetActiveGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (auto object : m_sceneObjectList[i])
			{
				if (object->GetActive() == false) {
					continue;
				}
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					return (T*)object.get();
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// 複数のオブジェクトを取得する関数
	/// </summary>
	/// <typeparam name="T">取得するオブジェクトのクラス名</typeparam>
	/// <returns>取得したオブジェクトのポインタ</returns>
	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 3; i++)
		{
			for (auto object : m_sceneObjectList[i])
			{
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					objects.push_back((T*)object.get());
				}
			}
		}
		return objects;
	}

	/// <summary>
	/// アクティブな複数のオブジェクトを取得する関数
	/// </summary>
	/// <typeparam name="T">取得するオブジェクトのクラス名</typeparam>
	/// <returns>取得したオブジェクトのポインタ</returns>
	template <typename T>
	std::vector<T*> GetActiveGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 3; i++)
		{
			for (auto object : m_sceneObjectList[i])
			{
				if (object->GetActive() == false) {
					continue;
				}
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					objects.push_back((T*)object.get());
				}
			}
		}
		return objects;
	}

	template <class Archive>
	void save(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_sceneObjectList)
		);
	}

};
