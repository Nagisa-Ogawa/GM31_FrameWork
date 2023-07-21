#pragma once

#include <list>
#include "IFactory.h"

struct MODEL;
class  Bullet;

class BulletFactory :public IFactory<BulletFactory, Bullet>
{
	friend class IFactory<BulletFactory, Bullet>;
private:
	MODEL* m_pModel = nullptr;

	BulletFactory();
	~BulletFactory();

	void Init();
	void Uninit() override;
public:
	Bullet* ActiveObject();
	void HideObject(Bullet* bullet);
};

//class BulletFactory
//{
//private:
//	// �V���O���g���p�C���X�^���X
//	static BulletFactory* m_Instance;
//	// �R���X�g���N�^
//	BulletFactory();
//	// �R�s�[�R���X�g���N�^
//	BulletFactory(const BulletFactory& manager);
//	// ������Z�q
//	BulletFactory& operator=(const BulletFactory& manager);
//
//	// �������ꂽ�e�̃��X�g
//	std::list<Bullet*> m_BulletList;
//	// �e�I�u�W�F�N�g�Ɏg�p���郂�f��
//	MODEL* m_pModel = nullptr;
//
//	// ����������
//	void Init();
//public:
//	// �f�X�g���N�^
//	virtual ~BulletFactory();
//	static BulletFactory* GetInstance();
//
//	// �e���擾����֐�
//	Bullet* GetBullet();
//	// �e���폜����֐�
//	void HideBullet(Bullet* pBullet);
//
//};

