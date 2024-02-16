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
	std::unique_ptr<Scene> m_scene = nullptr;
	std::unique_ptr<Scene> m_nextScene = nullptr;
	Editor* m_editor = nullptr;

	ENGINE_MODE m_mode;
	Manager();		// �R���X�g���N�^
	Manager(const Manager& manager);	// �R�s�[�R���X�g���N�^
	Manager& operator=(const Manager& manager);		// ������Z�q

	void Init();	// ����������
	std::list<std::string> GetAllFiles(std::string dirPath);	// �w�肳�ꂽ�t�H���_�ɂ���t�@�C�������X�g�ɂ��ĕԂ��֐�
	bool IsExistScene(std::string name);						// �w�肳�ꂽ�V�[�������邩�`�F�b�N����֐�
public:
	virtual ~Manager();		// �f�X�g���N�^

	void Uninit();
	void Update();
	void Draw();

	void CheckChangeScene();

	bool CreateScene(std::string sceneName);	// �V�[�����쐬����֐�
	bool ChangeScene(std::string sceneName);	// �V�[����ύX����֐�

	bool CheckSceneFile();	// �V�[���t�@�C�������݂��邩���`�F�b�N����֐�
	void SaveEditScene();	// �ҏW���Ă���V�[�����t�@�C���ɕۑ�����֐�
	void LoadEditScene();	// �ҏW���Ă���V�[�����t�@�C�����烍�[�h����֐�
	bool SaveScene(std::unique_ptr<Scene>& scene);	// �V�[�����t�@�C���ɕۑ�����֐�
	bool LoadScene(std::unique_ptr<Scene>& scene, std::string sceneName);	// �V�[�����t�@�C�����烍�[�h����֐�
	void SaveEditorSceneName();		// �ҏW���Ă����V�[������ۑ�����֐�
	std::string LoadEditorSceneName();		// �ҏW���Ă����V�[���������[�h����֐�

	void PlayScene();	// �V�[�������s����֐�
	void StopScene();	// �V�[���̎��s���~�߂�֐�
	void PauseScene();	// �V�[���̎��s���ꎞ��~����֐�


	static Manager* GetInstance();
	Scene* GetScene() { return m_scene.get(); }
	Editor* GetEditor() { return m_editor; }
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