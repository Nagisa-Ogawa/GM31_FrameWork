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
	// エディタ時用変数
	std::list<std::shared_ptr<Scene>> m_sceneList;
	Scene* m_scene = nullptr;
	Scene* m_nextScene = nullptr;
	Scene* m_editor = nullptr;

	ENGINE_MODE m_mode;
	Manager();		// コンストラクタ
	Manager(const Manager& manager);	// コピーコンストラクタ
	Manager& operator=(const Manager& manager);		// 代入演算子

	void Init();	// 初期化処理
	std::list<std::string> GetAllFiles(std::string dirPath);
public:
	virtual ~Manager();		// デストラクタ

	void Uninit();
	void Update();
	void Draw();

	bool CheckSceneFile();	// シーンファイルが存在するかをチェックする関数
	void SaveScene();	// シーンをファイルにセーブする関数
	void LoadScene();	// シーンをファイルからロードする関数

	void PlayScene();	// シーンを実行する関数
	void StopScene();	// シーンの実行を止める関数
	void PauseScene();	// シーンの実行を一時停止する関数




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