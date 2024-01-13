#pragma once

#include "gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>

class Scene
{
protected:
	std::list<GameObject*> m_GameObject[3];		// �V�[���ɑ��݂���I�u�W�F�N�g�̃��X�g
public:
	virtual void Init(){}
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	size_t GetGameObjectCount();	// �V�[���ɑ��݂���I�u�W�F�N�g�̌����擾����֐�
	int GetActiveGameObjectCount();	// �A�N�e�B�u�ȃI�u�W�F�N�g�̌����擾����֐�
	std::list<GameObject*> GetAllGameObjects();		// ���ׂẴI�u�W�F�N�g�����X�g�Ŏ擾����֐�

	void CallScriptStartFunc();		// �V�[���̎��s���ɃA�^�b�`����Ă���X�N���v�g��Start�֐����Ăяo���֐�


	/// <summary>
	/// �I�u�W�F�N�g���V�[���ɒǉ�����֐�
	/// </summary>
	/// <typeparam name="T">�ǉ�����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <param name="layer">�ǉ�����I�u�W�F�N�g�̃��C���[�i�`�揇�j</param>
	/// <param name="name">�ǉ�����I�u�W�F�N�g�̖��O</param>
	/// <returns>�ǉ������I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	T* AddGameObject(int layer,std::string name)
	{
		GameObject* gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		// transform�R���|�[�l���g�͕K�{�Ȃ��߂�����AddComponent
		gameObject->SetTransform(gameObject->AddComponent<Transform>());
		// �I�u�W�F�N�g�̖��O��ݒ�
		gameObject->SetName(name);
		gameObject->Init();

		return (T*)gameObject;
	}

	/// <summary>
	/// �I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
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

	/// <summary>
	/// �A�N�e�B�u�Ȏw�肳�ꂽ�I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	T* GetActiveGameObject()
	{
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (object->GetActive() == false) {
					continue;
				}
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					return (T*)object;
				}
			}
		}
		return nullptr;
	}

	/// <summary>
	/// �����̃I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
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

	/// <summary>
	/// �A�N�e�B�u�ȕ����̃I�u�W�F�N�g���擾����֐�
	/// </summary>
	/// <typeparam name="T">�擾����I�u�W�F�N�g�̃N���X��</typeparam>
	/// <returns>�擾�����I�u�W�F�N�g�̃|�C���^</returns>
	template <typename T>
	std::vector<T*> GetActiveGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 3; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (object->GetActive() == false) {
					continue;
				}
				if (typeid(*object) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
				{
					objects.push_back((T*)object);
				}
			}
		}
		return objects;
	}


};
