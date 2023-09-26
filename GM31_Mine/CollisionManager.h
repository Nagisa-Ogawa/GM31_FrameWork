#pragma once

#include <list>

class BoxCollision;
class QuadCollision;
class SphereCollision;
class Ray;

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

	// 各コリジョン用リスト
	std::list<BoxCollision*> m_BoxCollList;
	std::list<QuadCollision*> m_QuadCollList;
	std::list<SphereCollision*> m_SphereCollList;

	// 初期化処理
	void Init();
	// 投影線分の半分の長さを取得する関数
	float CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ = 0);
	// OBBとOBBが衝突した際のOBBがもう一方のOBBのどの面と衝突しているかを検知し
	// めり込んだ部分を戻す長さと方向を算出する関数
	void GetHitBoxSurface(BoxCollision* a, BoxCollision* b, float* l, D3DXVECTOR3* dir);
public:
	// デストラクタ
	virtual ~CollisionManager();
	static CollisionManager* GetInstance();

	// コリジョンコンポーネントを取得する関数
	void AddBoxCollision(BoxCollision* coll);
	void AddQuadCollision(QuadCollision* coll);
	void AddSphereCollision(SphereCollision* coll);

	//----------------------------
	// 当たり判定関係
	//----------------------------
	// OBBとOBBの当たり判定
	bool Collision_BoxToBox(BoxCollision* a, BoxCollision* b, float* l, D3DXVECTOR3* dir);
	// OBBと板ポリゴンの当たり判定
	bool Collision_BoxToQuad(BoxCollision* a, QuadCollision* b, float* l,D3DXVECTOR3* dir);
	// OBBと平面の当たり判定
	bool Collision_BoxToQuad(BoxCollision* a, D3DXVECTOR3 bPos,D3DXVECTOR3 bSize,
						D3DXVECTOR3 bVec[2], D3DXVECTOR3 bNormal, float* l, D3DXVECTOR3* dir);
	// レイと球体の当たり判定
	bool Collision_RayToSphere(Ray* ray, SphereCollision* sphereColl, float* out_T, D3DXVECTOR3* out_HitPos);
	// レイと立方体の当たり判定
	bool Collision_RayToBox(Ray* ray, BoxCollision* boxColl, float* out_T, D3DXVECTOR3* out_HitPos);
	// スクリーン座標（クライアント座標）からローカル座標系への座標変換
	void ScreenToLocalPosition(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix,
				D3DXMATRIX* projectionMatrix, POINT mousePos, float mouseZ, D3DXVECTOR3* out_Pos);

	std::list<BoxCollision*> GetBoxCollList() { return m_BoxCollList; }
	std::list<QuadCollision*> GetQuadCollList() { return m_QuadCollList; }
	std::list<SphereCollision*> GetSphereCollList() { return m_SphereCollList; }

};