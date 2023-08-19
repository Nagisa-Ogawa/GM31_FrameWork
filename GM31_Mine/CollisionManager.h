#pragma once

#include <list>

class BoxCollision;
class QuadCollision;

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
	std::list<BoxCollision*> m_BoxCollList;
	std::list<QuadCollision*> m_QuadCollList;
	// トリガー用リスト
	std::list<BoxCollision*> m_BoxTriList;
	std::list<QuadCollision*> m_QuadTriList;


	// 初期化処理
	void Init();
	// 投影線分の半分の長さを取得する関数
	float CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ = 0);
public:
	// デストラクタ
	virtual ~CollisionManager();
	static CollisionManager* GetInstance();

	void AddBoxCollision(BoxCollision* coll);
	void AddQuadCollision(QuadCollision* coll);
	bool Collision_BoxToBox(BoxCollision* a, BoxCollision* b);
	bool Collision_BoxToQuad(BoxCollision* a, QuadCollision* b, float* l,D3DXVECTOR3* dir);

};