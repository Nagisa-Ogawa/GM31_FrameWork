#pragma once

#include <list>
#include "bullet.h"

class MODEL;

class BulletFactory
{
private:
	// シングルトン用インスタンス
	static BulletFactory* m_Instance;
	// コンストラクタ
	BulletFactory();
	// コピーコンストラクタ
	BulletFactory(const BulletFactory& manager);
	// 代入演算子
	BulletFactory& operator=(const BulletFactory& manager);

	// 生成された弾のリスト
	std::list<Bullet*> m_BulletList;
	// 弾オブジェクトに使用するモデル
	MODEL* m_pModel = nullptr;

	// 初期化処理
	void Init();
public:
	// デストラクタ
	virtual ~BulletFactory();
	static BulletFactory* GetInstance();

	// 弾を取得する関数
	Bullet* GetBullet();

};