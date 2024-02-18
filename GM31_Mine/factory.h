#pragma once

#include <list>
#include <string>
#include <memory>

//-----------------------------------------------------
// �e���v���[�g�Ńt���C�E�F�C�g�p�^�[�������p�e�N���X
//------------------------------------------------------
template<class TFactory,class TObject>
class Factory
{
protected:
	// �������ꂽ�I�u�W�F�N�g�̃��X�g
	std::list<TObject*> m_objectList;

	// �R���X�g���N�^�̊O���ł̎g�p���֎~
	Factory(){}
	// �f�X�g���N�^�̎g�p���֎~
	virtual ~Factory() 
	{
		m_instance = nullptr;
	}

	// ����������
	virtual void Init() = 0;
private:
	// �R�s�[�R���X�g���N�^�̎g�p���֎~
	Factory(const Factory& singleton) = delete;
	Factory& operator= (const Factory& singleton) = delete;
public:
	static Factory* GetInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new TFactory();
			m_instance->Init();
		}
		return m_instance;
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
	static TFactory* m_instance;
};

template<class TFactory,class TObjcet>
TFactory* Factory<TFactory, TObjcet>::m_instance = NULL;

