#pragma once

#include <list>
#include <string>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/list.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"

#include "component.h"
#include "transform.h"


//-----------------------------------------------------
// ゲームオブジェクト用親クラス
//------------------------------------------------------
// エディタ用オブジェクトもこのクラスを継承します
//------------------------------------------------------
class GameObject
{
protected:
	Transform* m_transform = nullptr;	// オブジェクトのトランスフォーム情報(座標、回転)
	std::list<std::unique_ptr<Component>> m_componentList;	// オブジェクトのコンポーネントリスト
	bool m_destroy = false;				// オブジェクトの死亡フラグ
	bool m_active = true;				// オブジェクトの表示フラグ
	std::string m_name{};				// オブジェクトの名前
	int m_id = -1;		// オブジェクトのID
	int m_registerID = 0;	// コンポーネントにセットするID
	bool m_isGameObject = true;	// ゲームで使用されるオブジェクトかどうか（falseの例:当たり判定表示用オブジェクトなど

public:
	// Get系関数
	Transform* GetTransform() { return m_transform; }
	bool GetActive() { return m_active; }
	std::string GetName() { return m_name; }
	int GetID() { return m_id; }
	bool GetIsGameObject() { return m_isGameObject; }
	// Set系関数
	void SetTransform(Transform* transform) { m_transform = transform; }
	void SetDestroy();
	void SetActive(bool active) { m_active = active; }
	void SetName(std::string name) { m_name = name; }
	void SetID(int ID) { m_id = ID; }
	void SetIsGameObject(bool flag) { m_isGameObject = flag; }

	bool Destroy();

	template <typename T>
	T* AddComponent()
	{
		std::unique_ptr<Component> component = std::make_unique<T>();
		// コンポーネントが付いているゲームオブジェクトを格納
		// IDをセット
		component->SetID(m_registerID);
		m_registerID++;
		// コンポーネント元のオブジェクトをセット
		component->SetGameObject(this);
		component->Init();
		T* TComponent = (T*)component.get();
		m_componentList.push_back(std::move(component));

		return TComponent;
	}

	template <typename T>
	T* GetComponent() 
	{
		for (const auto& pComp : m_componentList)
		{
			if (typeid(*pComp) == typeid(T))// 型を調べる(RTTI動的型情報)
			{
				return (T*)pComp.get();
			}
		}
		return nullptr;
	}

	std::list<Component*> GetAllComponent();

	virtual void Init();

	/// <summary>
	/// オブジェクトロード時に呼び出される関数
	/// </summary>
	virtual void Load();

	virtual void Uninit();

	virtual void Update();

	virtual void Draw();

	void CheckDestroyedComponent();

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_id),
			CEREAL_NVP(m_componentList),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy),
			CEREAL_NVP(m_registerID)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_id),
			CEREAL_NVP(m_componentList),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy),
			CEREAL_NVP(m_registerID)
		);
	}
};

