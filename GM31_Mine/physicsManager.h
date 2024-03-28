#pragma once

#include <vector>
#include "collisionPair.h"

// シミュレーション定数定義
#define MAX_RIGIDBODIES	(500)
#define MAX_PAIRS			(5000)
#define CONTACT_BIAS		(0.1f)
#define CONTACT_SLOP		(0.001f)
#define ITERATION			(10)

class Transform;
class RigidBody;
class BoxCollision;

//-----------------------------------------------------
// 物理シミュレーション管理用クラス
//------------------------------------------------------
class PhysicsManager
{
private:

	// 剛体のデータ
	Transform* m_states[MAX_RIGIDBODIES];	// 剛体の状態
	RigidBody* m_bodies[MAX_RIGIDBODIES];	// 剛体の属性
	BoxCollision* m_colliders[MAX_RIGIDBODIES];	// 剛体の形状
	int m_numRigidBodies = 0;		// 現在の剛体の数

	// 当たり判定用ペアのデータ
	int m_pairSwap;
	int m_numPairs[2];
	CollisionPair m_pairs[2][MAX_PAIRS];	// 当たった可能性のあるペア

	// 物理シミュレーション用関数
	void AddExternalForce();	// 外力を与える関数
	void BroadPhase();			// ブロードフェーズ（AABBで当たっていそうなペアを検知する)を実行する関数
	void NarrowPhase();			// ナローフェーズ (ブロードフェーズで作成したペアで当たり判定をする）を実行する関数
	void SolveConstraint();		// 拘束ソルバー (衝突している剛体を反発させる為の計算) を算出 
	void UpdateTransform();		// 剛体情報を更新

	// 当たり判定用関数
	bool Collision_AABBToAABB(D3DXVECTOR3 centerA, D3DXVECTOR3 halfA, D3DXVECTOR3 centerB, D3DXVECTOR3 halfB);

	// ソート用関数
	void SortPairByKey(CollisionPair* pairs, CollisionPair* sortBuff, int pairNum);		// 衝突ペアの配列をkeyの昇順でソートする関数

	static PhysicsManager* m_Instance;	// シングルトン用インスタンス
	PhysicsManager();	// コンストラクタ
	PhysicsManager(const PhysicsManager& manager);	// コピーコンストラクタ
	PhysicsManager& operator=(const PhysicsManager& manager);	// 代入演算子

public:

	void Init();	// 初期化処理
	void CreatePhysicsScene();
	void PhysicsSimulate();		// 物理シミュレーション
	void DestroyPhysicsScene();
	void Uninit();	// 終了処理


	virtual ~PhysicsManager();	// デストラクタ
	static PhysicsManager* GetInstance();


};
