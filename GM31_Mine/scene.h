#pragma once

#include <list>
#include <vector>
#include <typeinfo>

#include "gameObject.h"
#include "cameraObject.h"
#include "field.h"
#include "player.h"
#include "polygon2D.h"
#include "enemy.h"
#include "bulletFactory.h"
#include "enemyFactory.h"
#include "wall.h"

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
		AddGameObject<Wall>(1);
		for (int i = 0; i < 10; i++) {
			Enemy* pEnemy = EnemyFactory::GetInstance()->ActiveObject();
			pEnemy->GetTransform()->m_Position=D3DXVECTOR3(-15.0f + i * 3.0f, 0.0f, 6.0f);
			pEnemy->GetTransform()->m_Rotation = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
		}

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
		BulletFactory::GetInstance()->Uninit();
		EnemyFactory::GetInstance()->Uninit();
	}


	void Update()
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

	void Draw()
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
