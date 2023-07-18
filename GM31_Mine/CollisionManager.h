#pragma once

#include <list>

class BoxCollision;

class CollisionManager
{
private:
	// シングルトン用インスタンス
	static CollisionManager* m_Instance;
	// コンストラクタ
	CollisionManager();
	// コピーコンストラクタ
	CollisionManager(const CollisionManager& manager);
	// 代入演算子
	CollisionManager& operator=(const CollisionManager& manager);

	// 当たり判定あり用リスト
	std::list<CollisionManager*> m_CollisionList;
	// トリガー用リスト
	std::list<CollisionManager*> m_TriggerList;


	// 初期化処理
	void Init();
	// 投影線分の半分の長さを取得する関数
	float CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ = 0);
public:
	// デストラクタ
	virtual ~CollisionManager();
	static CollisionManager* GetInstance();

	bool Collision_BoxToBox(BoxCollision* a, BoxCollision* b);

};