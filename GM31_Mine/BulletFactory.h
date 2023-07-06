#pragma once

#include <list>
#include "bullet.h"

class MODEL;

class BulletFactory
{
private:
	// �V���O���g���p�C���X�^���X
	static BulletFactory* m_Instance;
	// �R���X�g���N�^
	BulletFactory();
	// �R�s�[�R���X�g���N�^
	BulletFactory(const BulletFactory& manager);
	// ������Z�q
	BulletFactory& operator=(const BulletFactory& manager);

	// �������ꂽ�e�̃��X�g
	std::list<Bullet*> m_BulletList;
	// �e�I�u�W�F�N�g�Ɏg�p���郂�f��
	MODEL* m_pModel = nullptr;

	// ����������
	void Init();
public:
	// �f�X�g���N�^
	virtual ~BulletFactory();
	static BulletFactory* GetInstance();

	// �e���擾����֐�
	Bullet* GetBullet();

};