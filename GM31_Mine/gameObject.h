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

class GameObject
{
protected:
	Transform* m_transform = nullptr;	// オブジェクトのトランスフォーム情報(座標、回転)
	std::list<std::shared_ptr<Component>> m_componentList;	// オブジェクトのコンポーネントリスト
	bool m_destroy = false;				// オブジェクトの死亡フラグ
	bool m_active = true;				// オブジェクトの表示フラグ
	std::string m_name{};				// オブジェクトの名前
	int m_ID = -1;		// オブジェクトのID

public:
	// Get系関数
	Transform* GetTransform() { return m_transform; }
	bool GetActive() { return m_active; }
	std::string GetName() { return m_name; }
	int GetID() { return m_ID; }
	// Set系関数
	void SetTransform(Transform* transform) { m_transform = transform; }
	void SetDestroy() { m_destroy = true; }
	void SetActive(bool active) { m_active = active; }
	void SetName(std::string name) { m_name = name; }
	void SetID(int ID) { m_ID = ID; }

	bool Destroy()
	{
		if (m_destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	T* AddComponent()
	{
		std::shared_ptr<Component> component = std::make_shared<T>();
		// コンポーネントが付いているゲームオブジェクトを格納
		m_componentList.push_back(component);
		component->SetGameObject(this);
		component->Init();

		return (T*)component.get();
	}

	template <typename T>
	T* GetComponent() 
	{
		for (auto pComp : m_componentList)
		{
			if (typeid(*pComp) == typeid(T))// 型を調べる(RTTI動的型情報)
			{
				return (T*)pComp.get();
			}
		}
		return nullptr;
	}

	template <typename T>
	bool DeleteComponent(T* comp)
	{
		for (auto pComp : m_componentList) {
			if (*(pComp).get()==comp)// 型を調べる(RTTI動的型情報)
			{
				
			}
		}
		return false;
	}

	std::list<Component*> GetAllComponent()
	{
		std::list<Component*> list;
		for (auto component : m_componentList) {
			list.push_back(component.get());
		}
		return list;
	}

	virtual void Init() 
	{
		for (auto component : m_componentList) {
			component->Init();
		}
	}

	/// <summary>
	/// オブジェクトロード時に呼び出される関数
	/// </summary>
	virtual void Load()
	{
		m_transform = GetComponent<Transform>();
		for (auto component : m_componentList) {
			component->SetGameObject(this);
			component->Load();
		}
	}

	virtual void Uninit()
	{
		for (auto component : m_componentList) {
			component->Uninit();
		}
		m_componentList.clear();
	}

	virtual void Update()
	{
		for (auto component : m_componentList) {
			component->Update();
		}
	}

	virtual void Draw()
	{
		for (auto component : m_componentList) {
			component->Draw();
		}
	}


	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_ID),
			CEREAL_NVP(m_componentList),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			CEREAL_NVP(m_name),
			CEREAL_NVP(m_ID),
			CEREAL_NVP(m_componentList),
			CEREAL_NVP(m_active),
			CEREAL_NVP(m_destroy)
		);
	}
};

