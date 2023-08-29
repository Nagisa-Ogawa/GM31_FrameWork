#pragma once

class MyImGuiManager
{
private:
	// �V���O���g���p�C���X�^���X
	static MyImGuiManager* m_Instance;

	// �R���X�g���N�^
	MyImGuiManager();
	// �R�s�[�R���X�g���N�^
	MyImGuiManager(const MyImGuiManager& manager);
	// ������Z�q
	MyImGuiManager& operator=(const MyImGuiManager& manager);

	// ����������
	void Init();
public:
	// �f�X�g���N�^
	virtual ~MyImGuiManager();

	void Uninit();
	void Update();
	void Draw();

	static MyImGuiManager* GetInstance();
};