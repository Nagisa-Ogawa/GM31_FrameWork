#pragma once

class Scene;	// �O���錾

class Manager
{
private:
	// �V���O���g���p�C���X�^���X
	static Manager* m_Instance;
	Scene* m_Scene = nullptr;
	Scene* m_NextScene = nullptr;

	// �R���X�g���N�^
	Manager();
	// �R�s�[�R���X�g���N�^
	Manager(const Manager& manager);
	// ������Z�q
	Manager& operator=(const Manager& manager);

	// ����������
	void Init();
public:
	// �f�X�g���N�^
	virtual ~Manager();

	void Uninit();
	void Update();
	void Draw();

	static Manager* GetInstance();
	Scene* GetScene() { return m_Scene; }
	template <typename T>
	void SetScene()
	{
		m_NextScene = new T();
	}
};