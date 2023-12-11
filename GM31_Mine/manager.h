#pragma once

enum EDITOR_MODE
{
	EDIT,
	RUN,
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
	EDITOR_MODE m_mode;
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