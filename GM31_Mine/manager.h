#pragma once

#include <string>
#include <list>
#include <memory>

enum ENGINE_MODE
{
	EDIT,
	RUN,
	PAUSE,
};

class Scene;	// �O���錾
class Editor;

class Manager
{
private:
	// �V���O���g���p�C���X�^���X
	static Manager* m_instance;
	// �G�f�B�^���p�ϐ�
	std::list<std::shared_ptr<Scene>> m_sceneList;
	Scene* m_scene = nullptr;
	Scene* m_nextScene = nullptr;
	Scene* m_editor = nullptr;

	ENGINE_MODE m_mode;
	Manager();		// �R���X�g���N�^
	Manager(const Manager& manager);	// �R�s�[�R���X�g���N�^
	Manager& operator=(const Manager& manager);		// ������Z�q

	void Init();	// ����������
	std::list<std::string> GetAllFiles(std::string dirPath);
public:
	virtual ~Manager();		// �f�X�g���N�^

	void Uninit();
	void Update();
	void Draw();

	bool CheckSceneFile();	// �V�[���t�@�C�������݂��邩���`�F�b�N����֐�
	void SaveScene();	// �V�[�����t�@�C���ɃZ�[�u����֐�
	void LoadScene();	// �V�[�����t�@�C�����烍�[�h����֐�

	void PlayScene();	// �V�[�������s����֐�
	void StopScene();	// �V�[���̎��s���~�߂�֐�
	void PauseScene();	// �V�[���̎��s���ꎞ��~����֐�




	static Manager* GetInstance();
	Scene* GetScene() { return m_scene; }
	Scene* GetEditor() { return m_editor; }
	ENGINE_MODE GetMode() { return m_mode; }

	template <typename T>
	void SetScene()
	{
		m_nextScene = new T();
	}

	void SetEngineMode(ENGINE_MODE mode) 
	{
		m_mode = mode;
	}
};