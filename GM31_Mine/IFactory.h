#pragma once

#include <list>

// �t�@�N�g���[�̃C���^�[�t�F�[�X
template<class TFactory,class TObject>
class IFactory
{
protected:
	// �������ꂽ�I�u�W�F�N�g�̃��X�g
	std::list<TObject*> m_ObjectList;

	// �R���X�g���N�^�̊O���ł̎g�p���֎~
	IFactory(){}
	// �f�X�g���N�^�̎g�p���֎~
	vrtual ~IFactory() { std::atexit(Uninit); }

	// ����������
	virtual void Init() = 0;
	// �I��������
	virtual void Uninit() 
	{
		// �C���X�^���X�����
		delete m_Instance;
	}
private:
	// �R�s�[�R���X�g���N�^�̎g�p���֎~
	IFactory(const IFactory& singleton) = delete;
	IFactory& operator= (const IFactory& singleton) = delete;
public:
	static IFactory* GetInstance()
	{
		if (m_Instance == nullptr) {
			m_Instance = new TFactory();
			m_Instance->Init();
		}
	}

	// �������ꂽ�I�u�W�F�N�g����肷��֐�
	TObject* ActiveObject() = 0;
	// �I�u�W�F�N�g���\���ɂ���idelete�͂��Ȃ�)�֐�
	void HideObject(TObject* t) = 0;
private:
	static TFactory* m_Instance;
};

template<class TFactory,class TObjcet>
TFactory* IFactory<TFactory, TObjcet>::m_Instance = NULL;

