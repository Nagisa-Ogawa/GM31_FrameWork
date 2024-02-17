#pragma once
#include <list>
#include <memory>
#include <typeinfo>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"

#include "gameObject.h"

class Editor {

private:
	std::list<std::unique_ptr<GameObject>> m_editorObjectList[3];		// エディタに存在するオブジェクトのリスト
	std::string m_name;		// エディタの名前
	int m_registerID = 0;	// エディタオブジェクトにセットするID
public:
	void Init();
	void Load();	// エディタを読み込んだ際に呼ばれる関数
	void Uninit();
	void Update();
	void Draw();

	std::string GetName() { return m_name; }
	void CheckDestroyedObject();


	/// <summary>
/// エディタ用オブジェクトをエディタに追加する関数
/// </summary>
/// <typeparam name="T">追加するオブジェクトのクラス名</typeparam>
/// <param name="layer">追加するオブジェクトのレイヤー（描画順）</param>
/// <param name="name">追加するオブジェクトの名前</param>
/// <returns>追加したオブジェクトのポインタ</returns>
	template <typename T>
	T* AddEditorObject(int layer, std::string name)
	{
		std::unique_ptr<GameObject> editorObject = std::make_unique<T>();
		// transformコンポーネントは必須なためここでAddComponent
		editorObject->SetTransform(editorObject->AddComponent<Transform>());
		// オブジェクトの名前を設定
		editorObject->SetName(name);
		// IDを設定
		editorObject->SetID(m_registerID);
		m_registerID++;
		// エディタ用オブジェクトであることをセット
		editorObject->SetIsGameObject(false);
		editorObject->Init();
		T* TObject = (T*)editorObject.get();
		m_editorObjectList[layer].push_back(std::move(editorObject));

		return TObject;
	}

	/// <summary>
	/// オブジェクトを取得する関数
	/// </summary>
	/// <typeparam name="T">取得するオブジェクト名</typeparam>
	/// <returns>取得したオブジェクトのポインタ</returns>
	template <typename T>
	T* GetEditorObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (const auto& object : m_editorObjectList[i])
			{
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					return (T*)object.get();
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	T* GetEditorObjectWithID(int ID)
	{
		// IDが-1なら無効
		if (ID == -1) return nullptr;
		// オブジェクトのリストからIDが同じオブジェクトを探す
		for (int i = 0; i < 3; i++) {
			auto it = std::find_if(m_editorObjectList[i].begin(), m_editorObjectList[i].end(),
				[&ID](const auto& obj) {return obj->GetID() == ID; });
			if (it != m_editorObjectList[i].end()) {
				// 一致したオブジェクトがあったなら返す
				return (T*)(it->get());
			}
		}
		// ないならnullを返す
		return nullptr;
	}

	template <typename T>
	T* GetEditorObjectWithName(std::string name)
	{
		// 名前が空なら無効
		if (name == "") return nullptr;
		// オブジェクトのリストからIDが同じオブジェクトを探す
		for (int i = 0; i < 3; i++) {
			auto it = std::find_if(m_editorObjectList[i].begin(), m_editorObjectList[i].end(),
				[&name](const auto& obj) {return obj->GetName() == name; });
			if (it != m_editorObjectList[i].end()) {
				// 一致したオブジェクトがあったなら返す
				return (T*)(it->get());
			}
		}
		// ないならnullを返す
		return nullptr;
	}


	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_editorObjectList),
			CEREAL_NVP(m_registerID)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_editorObjectList),
			CEREAL_NVP(m_registerID)
		);
	}

};
