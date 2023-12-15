#pragma once

#include <list>
#include <string>

// �t�@�N�g���[�̃C���^�[�t�F�[�X
template<class TFactory,class TObject>
class Factory
{
protected:
	// �������ꂽ�I�u�W�F�N�g�̃��X�g
	std::list<TObject*> m_ObjectList;

	// �R���X�g���N�^�̊O���ł̎g�p���֎~
	Factory(){}
	// �f�X�g���N�^�̎g�p���֎~
	virtual ~Factory() 
	{
		delete m_Instance;
		m_Instance = nullptr;
	}

	// ����������
	virtual void Init() = 0;
private:
	// �R�s�[�R���X�g���N�^�̎g�p���֎~
	Factory(const Factory& singleton) = delete;
	Factory& operator= (const Factory& singleton) = delete;
public:
	static TFactory* GetInstance()
	{
		if (m_Instance == nullptr) {
			m_Instance = new TFactory();
			m_Instance->Init();
		}
		return m_Instance;
	}

	// �������ꂽ�I�u�W�F�N�g����肷��֐�
	virtual TObject* ActiveObject(std::string name) = 0;
	// �I�u�W�F�N�g���\���ɂ���idelete�͂��Ȃ�)�֐�
	virtual void HideObject(TObject* t) = 0;

	// �I��������
	virtual void Uninit()
	{
	}

	virtual void Clear() 
	{

	}
private:
	static TFactory* m_Instance;
};

template<class TFactory,class TObjcet>
TFactory* Factory<TFactory, TObjcet>::m_Instance = NULL;

