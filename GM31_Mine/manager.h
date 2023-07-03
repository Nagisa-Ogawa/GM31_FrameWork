#pragma once

class Scene;	// 前方宣言

class Manager
{
private:
	// シングルトン用インスタンス
	static Manager* m_Instance;
	Scene* m_Scene;

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
	Scene* GetScene() { return m_Scene; }
};