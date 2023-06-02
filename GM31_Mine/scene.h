#pragma once

#include "gameObject.h"
#include <list>

#include "camera.h"
#include "field.h"
#include "player.h"
#include "polygon2D.h"

class Scene
{
protected:
	std::list<GameObject*> m_GameObject; 

public:
	void Init()
	{
		AddGameObject<Camera>();
		AddGameObject<Field>();
		AddGameObject<Player>();
		AddGameObject<Polygon2D>();
	}

	void Uninit() 
	{
		for (GameObject* gameObject : m_GameObject) {
			gameObject->Uninit();
			delete gameObject;
		}
		m_GameObject.clear();
	}

	void Update()
	{
		for (GameObject* gameObject : m_GameObject) {
			gameObject->Update();
		}
	}

	void Draw()
	{
		for (GameObject* gameObject : m_GameObject) {
			gameObject->Draw();
		}
	}

	template <typename T>
	T* AddGameObject()
	{
		T* gameObject = new T();
		m_GameObject.push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}


	template <typename T>
	T* GetGameObject()
	{
		for (GameObject* object : m_GameObject)
		{
			if (typeid(*object) == typeid(T))// Œ^‚ğ’²‚×‚é(RTTI“®“IŒ^î•ñ)
			{
				return (T*)object;
			}
		}
		return nullptr;
	}


};
