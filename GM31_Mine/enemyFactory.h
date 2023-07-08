#pragma once

#include <list>

struct MODEL;
class  Enemy;

class EnemyFactory
{
private:
	// シングルトン用インスタンス
	static EnemyFactory* m_Instance;
	// コンストラクタ
	EnemyFactory();
	// コピーコンストラクタ
	EnemyFactory(const EnemyFactory& manager);
	// 代入演算子
	EnemyFactory& operator=(const EnemyFactory& manager);

	// 生成された弾のリスト
	std::list<Enemy*> m_EnemyList;
	// 弾オブジェクトに使用するモデル
	MODEL* m_pModel = nullptr;

	// 初期化処理
	void Init();
public:
	// デストラクタ
	virtual ~EnemyFactory();
	static EnemyFactory* GetInstance();

	// 弾を取得する関数
	Enemy* GetEnemy();
	// 弾を削除する関数
	void HideEnemy(Enemy* pEnemy);

};