#pragma once

#include <list>
#include <vector>
#include <typeinfo>

#include "gameObject.h"
#include "cameraObject.h"
#include "field.h"
#include "player.h"
#include "polygon2D.h"

#include "bullet.h"
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
				// �A�N�e�B�u�t���O��ON�Ȃ�X�V����������
				if (gameObject->GetActive())
							gameObject->Update();
			}
			m_GameObject[i].remove_if([](GameObject* object)
			{return object->Destroy(); });	// �����_��
		}
	}

	void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])
			{
				// �A�N�e�B�u�t���O��ON�Ȃ�`�悷��
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
		// transform�R���|�[�l���g�͕K�{�Ȃ��߂�����AddComponent
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
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
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
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}


};
