#pragma once

enum ENGINE_MODE
{
	EDIT,
	RUN,
	PAUSE,
};

enum GAME_MODE
{
	NONE,
	TITLE,
	INGAME,
};

class Scene;	// 前方宣言
class Editor;

class Manager
{
private:
	// シングルトン用インスタンス
	static Manager* m_instance;
	Scene* m_scene = nullptr;
	Scene* m_nextScene = nullptr;
	Scene* m_editor = nullptr;
	Scene* m_title = nullptr;
	ENGINE_MODE m_mode;
	GAME_MODE m_gameMode;
	GAME_MODE m_nextMode;
	// コンストラクタ
	Manager();
	// コピーコンストラクタ
	Manager(const Manager& manager);
	// 代入演算子
	Manager& operator=(const Manager& manager);

	// 初期化処理
	void Init();
public:
	// デストラクタ
	virtual ~Manager();

	void Uninit();
	void Update();
	void Draw();

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

	void SetGameMode(GAME_MODE mode) {
		m_nextMode = mode;
	}
};