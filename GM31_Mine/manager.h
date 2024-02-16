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

class Scene;	// 前方宣言
class Editor;

class Manager
{
private:
	// シングルトン用インスタンス
	static Manager* m_instance;
	std::unique_ptr<Scene> m_scene = nullptr;
	std::unique_ptr<Scene> m_nextScene = nullptr;
	Editor* m_editor = nullptr;

	ENGINE_MODE m_mode;
	Manager();		// コンストラクタ
	Manager(const Manager& manager);	// コピーコンストラクタ
	Manager& operator=(const Manager& manager);		// 代入演算子

	void Init();	// 初期化処理
	std::list<std::string> GetAllFiles(std::string dirPath);	// 指定されたフォルダにあるファイルをリストにして返す関数
	bool IsExistScene(std::string name);						// 指定されたシーンがあるかチェックする関数
public:
	virtual ~Manager();		// デストラクタ

	void Uninit();
	void Update();
	void Draw();

	void CheckChangeScene();

	bool CreateScene(std::string sceneName);	// シーンを作成する関数
	bool ChangeScene(std::string sceneName);	// シーンを変更する関数

	bool CheckSceneFile();	// シーンファイルが存在するかをチェックする関数
	void SaveEditScene();	// 編集しているシーンをファイルに保存する関数
	void LoadEditScene();	// 編集しているシーンをファイルからロードする関数
	bool SaveScene(std::unique_ptr<Scene>& scene);	// シーンをファイルに保存する関数
	bool LoadScene(std::unique_ptr<Scene>& scene, std::string sceneName);	// シーンをファイルからロードする関数
	void SaveEditorSceneName();		// 編集していたシーン名を保存する関数
	std::string LoadEditorSceneName();		// 編集していたシーン名をロードする関数

	void PlayScene();	// シーンを実行する関数
	void StopScene();	// シーンの実行を止める関数
	void PauseScene();	// シーンの実行を一時停止する関数


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