#pragma once

class MyImGuiManager
{
private:
	// シングルトン用インスタンス
	static MyImGuiManager* m_Instance;

	// コンストラクタ
	MyImGuiManager();
	// コピーコンストラクタ
	MyImGuiManager(const MyImGuiManager& manager);
	// 代入演算子
	MyImGuiManager& operator=(const MyImGuiManager& manager);

	// 初期化処理
	void Init();
public:
	// デストラクタ
	virtual ~MyImGuiManager();

	void Uninit();
	void Update();
	void Draw();

	static MyImGuiManager* GetInstance();
};