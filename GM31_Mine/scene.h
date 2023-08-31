#pragma once

#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>

class Scene
{
protected:
	std::list<GameObject*> m_GameObject[3]; 
public:
	virtual void Init(){}

	virtual void Uninit() 
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


	virtual void Update()
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

	virtual void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				// アクティブフラグがONなら描画する
				if(gameObject->GetActive())
							gameObject->Draw();
			}
		}
	}

	template <typename T>
	T* AddGameObject(int layer)
	{
		GameObject* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		// transformコンポーネントは必須なためここでAddComponent
		gameObject->SetTransform(gameObject->AddComponent<Transform>());
		gameObject->Init();

		return (T*)gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))// 型を調べる(RTTI動的型情報)
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}


};
