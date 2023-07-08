#pragma once

#include <list>

struct MODEL;
class  Enemy;

class EnemyFactory
{
private:
	// �V���O���g���p�C���X�^���X
	static EnemyFactory* m_Instance;
	// �R���X�g���N�^
	EnemyFactory();
	// �R�s�[�R���X�g���N�^
	EnemyFactory(const EnemyFactory& manager);
	// ������Z�q
	EnemyFactory& operator=(const EnemyFactory& manager);

	// �������ꂽ�e�̃��X�g
	std::list<Enemy*> m_EnemyList;
	// �e�I�u�W�F�N�g�Ɏg�p���郂�f��
	MODEL* m_pModel = nullptr;

	// ����������
	void Init();
public:
	// �f�X�g���N�^
	virtual ~EnemyFactory();
	static EnemyFactory* GetInstance();

	// �e���擾����֐�
	Enemy* GetEnemy();
	// �e���폜����֐�
	void HideEnemy(Enemy* pEnemy);

};