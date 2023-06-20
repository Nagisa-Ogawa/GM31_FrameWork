#pragma once

#include <list>
#include <vector>
#include <typeinfo>

#include "gameObject.h"
#include "cameraObject.h"
#include "field.h"
#include "player.h"
#include "polygon2D.h"

class Scene
{
protected:
	std::list<GameObject*> m_GameObject[3]; 

public:
	void Init()
	{
		AddGameObject<CameraObject>(0);
		AddGameObject<Field>(1);
		AddGameObject<Player>(1);
		AddGameObject<Polygon2D>(2);
	}

	void Uninit() 
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

	void Update()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Update();
			}
			m_GameObject[i].remove_if([](GameObject* object)
			{return object->Destroy(); });	// ƒ‰ƒ€ƒ_Ž®
		}
	}

	void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				gameObject->Draw();
			}
		}
	}

	template <typename T>
	T* AddGameObject(int layer)
	{
		GameObject* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		// transformƒRƒ“ƒ|[ƒlƒ“ƒg‚Í•K{‚È‚½‚ß‚±‚±‚ÅAddComponent
		gameObject->SetTransform(gameObject->AddComponent<TransForm>());
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
				if (typeid(*object) == typeid(T))// Œ^‚ð’²‚×‚é(RTTI“®“IŒ^î•ñ)
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
				if (typeid(*object) == typeid(T))// Œ^‚ð’²‚×‚é(RTTI“®“IŒ^î•ñ)
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}


};
