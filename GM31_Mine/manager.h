#pragma once

enum EDITOR_MODE
{
	EDIT,
	RUN,
};

class Scene;	// �O���錾
class Editor;

class Manager
{
private:
	// �V���O���g���p�C���X�^���X
	static Manager* m_instance;
	Scene* m_scene = nullptr;
	Scene* m_nextScene = nullptr;
	Scene* m_editor = nullptr;
	EDITOR_MODE m_mode;
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
	Scene* GetScene() { return m_scene; }
	Scene* GetEditor() { return m_editor; }
	EDITOR_MODE GetMode() { return m_mode; }

	template <typename T>
	void SetScene()
	{
		m_nextScene = new T();
	}

	void SetEditorMode(EDITOR_MODE mode) 
	{
		m_mode = mode;
	}
};