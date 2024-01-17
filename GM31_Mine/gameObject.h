#pragma once

#include <list>
#include <string>

#include "component.h"
#include "transform.h"

class GameObject
{
protected:
	Transform* m_transform = nullptr;	// オブジェクトのトランスフォーム情報(座標、回転)
	std::list<Component*> m_component;	// オブジェクトのコンポーネントリスト
	bool m_destroy = false;				// オブジェクトの死亡フラグ
	bool m_active = true;				// オブジェクトの表示フラグ
	std::string m_name{};				// オブジェクトの名前

public:
	// Get系関数
	Transform* GetTransform() { return m_transform; }
	bool GetActive() { return m_active; }
	std::string GetName() { return m_name; }
	// Set系関数
	void SetTransform(Transform* transform) { m_transform = transform; }
	void SetDestroy() { m_destroy = true; }
	void SetActive(bool active) { m_active = active; }
	void SetName(std::string name) { m_name = name; }

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

	template <typename TFactory>
	TFactory* AddComponent()
	{
		Component* component = new TFactory();
		// コンポーネントが付いているゲームオブジェクトを格納
		m_component.push_back(component);
		component->SetGameObject(this);
		component->Init();

		return (TFactory*)component;
	}

	template <typename T>
	T* GetComponent() 
	{
		for (Component* pComp : m_component)
		{
			if (typeid(*pComp) == typeid(T))// 型を調べる(RTTI動的型情報)
			{
				return (T*)pComp;
			}
		}
		return nullptr;
	}

	std::list<Component*> GetAllComponent()
	{
		return m_component;
	}

	virtual void Init() 
	{
		for (Component* component : m_component) {
			component->Init();
		}
	}
	virtual void Uninit()
	{
		for (Component* component : m_component) {
			component->Uninit();
			delete component;
		}
		m_component.clear();
	}
	virtual void Update()
	{
		for (Component* component : m_component) {
			component->Update();
		}
	}
	virtual void Draw()
	{
		for (Component* component : m_component) {
			component->Draw();
		}
	}
};
