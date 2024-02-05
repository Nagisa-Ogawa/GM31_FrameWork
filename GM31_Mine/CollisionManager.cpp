
#include <algorithm>
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "quadCollision.h"
#include "sphereCollision.h"
#include "polygonCollision.h"
#include "Ray.h"

CollisionManager* CollisionManager::m_Instance = NULL;

CollisionManager::CollisionManager()
{
	// なにもしない
}

CollisionManager::CollisionManager(const CollisionManager & manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
}

CollisionManager & CollisionManager::operator=(const CollisionManager & manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
	return *m_Instance;
}

void CollisionManager::Init()
{
}

void CollisionManager::Uninit()
{
	m_BoxCollList.clear();
	m_SphereCollList.clear();
	m_QuadCollList.clear();
}


CollisionManager::~CollisionManager()
{
	// インスタンスを解放
	delete m_Instance;
}

CollisionManager * CollisionManager::GetInstance()
{
	// 初めて使うときにインスタンスを生成
	// それ以降は生成したインスタンスを渡す
	if (m_Instance == NULL)
	{
		m_Instance = new CollisionManager();
		m_Instance->Init();
	}
	return m_Instance;
}

void CollisionManager::AddBoxCollision(BoxCollision * coll)
{
	if (coll == nullptr) return;
	m_BoxCollList.push_back(coll);
}

void CollisionManager::AddQuadCollision(QuadCollision * coll)
{
	if (coll == nullptr) return;
	m_QuadCollList.push_back(coll);
}

void CollisionManager::AddSphereCollision(SphereCollision* coll)
{
	if (coll == nullptr) return;
	m_SphereCollList.push_back(coll);
}

void CollisionManager::AddPolygonCollision(PolygonCollision* coll)
{
	if (coll == nullptr) return;
	m_PolygonCollList.push_back(coll);
}

//-------------------------------------
// OBBとOBBの当たり判定
//-------------------------------------
// 戻り値 : 当たったかどうか
//-------------------------------------
// 
// a       : OBB用コリジョンその1
// b       : OBB用コリジョンその2
// out_L   : めり込んだOBBを戻す距離
// out_Dir : めり込んだOBBを戻す方向
//-------------------------------------

bool CollisionManager::Collision_BoxToBox(BoxCollision* a, BoxCollision* b, float* out_L, D3DXVECTOR3* out_Dir)
{
	// 2オブジェクトの各軸を定義
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight() *   (a->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp() *      (a->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z /*/ 2.0f*/);
	D3DXVECTOR3 bVecX = b->GetGameObject()->GetTransform()->GetRight() *   (b->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 bVecY = b->GetGameObject()->GetTransform()->GetUp() *      (b->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 bVecZ = b->GetGameObject()->GetTransform()->GetForward() * (b->GetSize().z /*/ 2.0f*/);
	// 2オブジェクト間の距離
	D3DXVECTOR3 distance = ((b->GetGameObject()->GetTransform()->m_worldPosition + b->GetOffset()) -
									(a->GetGameObject()->GetTransform()->m_worldPosition + a->GetOffset()));

	// オブジェクトAのX軸が分離軸の場合
	D3DXVECTOR3 nAVecX;
	D3DXVec3Normalize(&nAVecX, &aVecX);
	// オブジェクトAの投影線分の半分
	float hpVecA = D3DXVec3Length(&aVecX);
	// オブジェクトBの投影線分の半分
	float hpVecB = CreateHalfProjectionLine(&nAVecX, &bVecX, &bVecY, &bVecZ);
	// 2つのオブジェクトの中心点間の距離を分離軸上に投影
	float dDistance = fabs(D3DXVec3Dot(&distance, &nAVecX));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}
	// オブジェクトAのY軸が分離軸の場合
	D3DXVECTOR3 nAVecY;
	D3DXVec3Normalize(&nAVecY, &aVecY);
	hpVecA = D3DXVec3Length(&aVecY);
	hpVecB = CreateHalfProjectionLine(&nAVecY, &bVecX, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &nAVecY));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}
	// オブジェクトAのZ軸が分離軸の場合
	D3DXVECTOR3 nAVecZ;
	D3DXVec3Normalize(&nAVecZ, &aVecZ);
	hpVecA = D3DXVec3Length(&aVecZ);
	hpVecB = CreateHalfProjectionLine(&nAVecZ, &bVecX, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &nAVecZ));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}
	// オブジェクトBのX軸が分離軸の場合
	D3DXVECTOR3 nBVecX;
	D3DXVec3Normalize(&nBVecX, &bVecX);
	hpVecA = CreateHalfProjectionLine(&nBVecX, &aVecX, &aVecY, &aVecZ);
	hpVecB = D3DXVec3Length(&bVecX);
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecX));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}
	// オブジェクトBのY軸が分離軸の場合
	D3DXVECTOR3 nBVecY;
	D3DXVec3Normalize(&nBVecY, &bVecY);
	hpVecA = CreateHalfProjectionLine(&nBVecY, &aVecX, &aVecY, &aVecZ);
	hpVecB = D3DXVec3Length(&bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecY));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}
	// オブジェクトBのZ軸が分離軸の場合
	D3DXVECTOR3 nBVecZ;
	D3DXVec3Normalize(&nBVecZ, &bVecZ);
	hpVecA = CreateHalfProjectionLine(&nBVecZ, &aVecX, &aVecY, &aVecZ);
	hpVecB = D3DXVec3Length(&bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecZ));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}

	// 外積分離軸 : オブジェクトAのX軸とオブジェクトBのX軸
	D3DXVECTOR3 crossVec;
	D3DXVec3Cross(&crossVec, &nAVecX, &nBVecX);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecY, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのX軸とオブジェクトBのY軸
	D3DXVec3Cross(&crossVec, &nAVecX, &nBVecY);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecY, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのX軸とオブジェクトBのZ軸
	D3DXVec3Cross(&crossVec, &nAVecX, &nBVecZ);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecY, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのY軸とオブジェクトBのX軸
	D3DXVec3Cross(&crossVec, &nAVecY, &nBVecX);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのY軸とオブジェクトBのY軸
	D3DXVec3Cross(&crossVec, &nAVecY, &nBVecY);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのY軸とオブジェクトBのZ軸
	D3DXVec3Cross(&crossVec, &nAVecY, &nBVecZ);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのZ軸とオブジェクトBのX軸
	D3DXVec3Cross(&crossVec, &nAVecZ, &nBVecX);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecY);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのZ軸とオブジェクトBのY軸
	D3DXVec3Cross(&crossVec, &nAVecZ, &nBVecY);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecY);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// 外積分離軸 : オブジェクトAのZ軸とオブジェクトBのZ軸
	D3DXVec3Cross(&crossVec, &nAVecZ, &nBVecZ);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecY);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}

	// すべてに当てはまらなかったら衝突している
	// 当たった際の引き戻す方向と長さを取得
	GetHitBoxSurface(a, b, out_L, out_Dir);

	return true;
}

//---------------------------------------------------------
// 分離軸からもう片方のOBBの投影線分の半分を求める関数
//---------------------------------------------------------
// 戻り値 : もう片方のOBBの投影線分の半分
//---------------------------------------------------------
// ssAxis : 分離軸
// vecX   : オブジェクトのX軸
// vecY   : オブジェクトのY軸
// vecZ   : オブジェクトのZ軸
//---------------------------------------------------------
float CollisionManager::CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ)
{
	float r1 = fabs(D3DXVec3Dot(sAxis, vecX));
	float r2 = fabs(D3DXVec3Dot(sAxis, vecY));
	float r3 = 0.0f;
	if (vecZ) {
		r3 = fabs(D3DXVec3Dot(sAxis, vecZ));
	}

	return r1 + r2 + r3;
}

//-------------------------------------
// OBBとOBBが衝突した際に基準とするOBBがもう一方のOBBのどの面と衝突しているかを検知し
// めり込んだ部分を戻す長さと方向を算出する関数
// //----------------------------------
// 戻り値 : void
//-------------------------------------
// a       : 基準となるOBB用コリジョン
// b       : もう一方のOBB用コリジョン
// out_L   : めり込んだOBBを戻す距離
// out_Dir : めり込んだOBBを戻す方向
//-------------------------------------
void CollisionManager::GetHitBoxSurface(BoxCollision* a, BoxCollision* b, float* out_L, D3DXVECTOR3* out_Dir)
{
	// bを6個の平面として分解
	// 各要素の変数を6個ずつ用意
	D3DXVECTOR3 bPos = b->GetGameObject()->GetTransform()->m_worldPosition + b->GetOffset();
	D3DXVECTOR3 bPoss[6] = { bPos,bPos,bPos,bPos,bPos,bPos };
	D3DXVECTOR2 bSizes[6];
	D3DXVECTOR3 bVecs[6][2];
	D3DXVECTOR3 bNormals[6];
	// 正面
	bPoss   [0] -= b->GetGameObject()->GetTransform()->GetForward() * b->GetSize().z;
	bSizes  [0] = D3DXVECTOR2(b->GetSize().x, b->GetSize().y);
	bVecs   [0][0]= b->GetGameObject()->GetTransform()->GetRight();
	bVecs   [0][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[0] = b->GetGameObject()->GetTransform()->GetForward() * -1.0f;
	// 後ろ
	bPoss   [1] += b->GetGameObject()->GetTransform()->GetForward() * b->GetSize().z;
	bSizes  [1] = D3DXVECTOR2(b->GetSize().x, b->GetSize().y);
	bVecs   [1][0] = b->GetGameObject()->GetTransform()->GetRight();
	bVecs   [1][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[1] = b->GetGameObject()->GetTransform()->GetForward();
	// 右
	bPoss   [2] += b->GetGameObject()->GetTransform()->GetRight() * b->GetSize().x;
	bSizes  [2] = D3DXVECTOR2(b->GetSize().z, b->GetSize().y);
	bVecs   [2][0] = b->GetGameObject()->GetTransform()->GetForward();
	bVecs   [2][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[2] = b->GetGameObject()->GetTransform()->GetRight();
	// 左
	bPoss   [3] -= b->GetGameObject()->GetTransform()->GetRight() * b->GetSize().x;
	bSizes  [3] = D3DXVECTOR2(b->GetSize().z, b->GetSize().y);
	bVecs   [3][0] = b->GetGameObject()->GetTransform()->GetForward() * -1.0f;
	bVecs   [3][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[3] = b->GetGameObject()->GetTransform()->GetRight() * -1.0f;
	// 上
	bPoss   [4] += b->GetGameObject()->GetTransform()->GetUp() * b->GetSize().y;
	bSizes  [4] = D3DXVECTOR2(b->GetSize().x, b->GetSize().z);
	bVecs   [4][0] = b->GetGameObject()->GetTransform()->GetRight();
	bVecs   [4][1] = b->GetGameObject()->GetTransform()->GetForward();
	bNormals[4] = b->GetGameObject()->GetTransform()->GetUp();
	// 下
	bPoss   [5] -= b->GetGameObject()->GetTransform()->GetUp() * b->GetSize().y;
	bSizes  [5] = D3DXVECTOR2(b->GetSize().x, b->GetSize().z);
	bVecs   [5][0] = b->GetGameObject()->GetTransform()->GetRight() * -1.0f;
	bVecs   [5][1] = b->GetGameObject()->GetTransform()->GetForward();
	bNormals[5] = b->GetGameObject()->GetTransform()->GetUp() * -1.0f;

	float minL = -1.0f;
	D3DXVECTOR3 minDir;
	// どの平面とaが衝突しているかをチェック
	for (int i = 0; i < 6; i++) {
		float l = -1.0f;
		D3DXVECTOR3 dir;
		// OBBと平面の当たり判定
		if (Collision_BoxToQuad(a, bPoss[i], bSizes[i], bVecs[i], bNormals[i], &l, &dir)) {
			// 衝突していたなら押し戻す長さを比較
			if (minL < 0.0f) {
				minL = l;
				minDir = dir;
			}
			else {
				if (minL > l) {
					minL = l;
					minDir = dir;
				}
			}
		}
	}
	if (minL >= 0.0f) {
		*out_L = minL;
		*out_Dir = minDir;
	}
}

//-------------------------------------
// OBBと板ポリゴンの当たり判定
// //----------------------------------
// 戻り値 : 当たったかどうか
//-------------------------------------
// a       : OBB用コリジョン
// b       : 板ポリゴン用コリジョン
// out_L   : めり込んだOBBを戻す距離
// out_Dir : めり込んだOBBを戻す方向
//-------------------------------------
bool CollisionManager::Collision_BoxToQuad(BoxCollision * a, QuadCollision * b, float* out_L, D3DXVECTOR3* out_Dir)
{
	D3DXVECTOR3 aPos = a->GetGameObject()->GetTransform()->m_worldPosition;
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight()   * (a->GetSize().x);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp()      * (a->GetSize().y);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z);

	D3DXVECTOR3 bPos = b->GetGameObject()->GetTransform()->m_worldPosition;
	D3DXVECTOR3 bVecX = b->GetGameObject()->GetTransform()->GetRight() * (b->GetSize()->x);
	D3DXVECTOR3 bVecZ = b->GetGameObject()->GetTransform()->GetForward() * (b->GetSize()->y);


	D3DXVECTOR3 n{};
	D3DXVec3Normalize(&n, b->GetNormal());

	// 2オブジェクト間の距離
	D3DXVECTOR3 distance = (b->GetGameObject()->GetTransform()->m_worldPosition -
		a->GetGameObject()->GetTransform()->m_worldPosition);

	// ボックスがz方向以外で当たっているかをチェック
	D3DXVECTOR3 nBVecX;
	D3DXVec3Normalize(&nBVecX, &bVecX);
	// オブジェクトの投影線分の半分
	float hpVecA = CreateHalfProjectionLine(&nBVecX, &aVecX, &aVecY, &aVecZ);
	// オブジェクトBの投影線分の半分
	float hpVecB = D3DXVec3Length(&bVecX);
	// 2つのオブジェクトの中心点間の距離を分離軸上に投影
	float dDistance = fabs(D3DXVec3Dot(&distance, &nBVecX));
	if (hpVecA + hpVecB <= dDistance) {
		// 衝突していない
		return false;
	}


	D3DXVECTOR3 nBVecZ;
	D3DXVec3Normalize(&nBVecZ, &bVecZ);
	// オブジェクトの投影線分の半分
	hpVecA = CreateHalfProjectionLine(&nBVecZ, &aVecX, &aVecY, &aVecZ);
	// オブジェクトBの投影線分の半分
	hpVecB = D3DXVec3Length(&bVecZ);
	// 2つのオブジェクトの中心点間の距離を分離軸上に投影
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecZ));
	if (hpVecA + hpVecB <= dDistance) {
		// 衝突していない
		return false;
	}

	// ボックスの各軸の中点から各頂点までの距離を分離軸（平面の法線）上に投影した時の長さを算出
	float r = 0.0f;
	r += fabs(D3DXVec3Dot(&aVecX, &n));
	r += fabs(D3DXVec3Dot(&aVecY, &n));
	r += fabs(D3DXVec3Dot(&aVecZ, &n));

	//　ボックスの中点から平面までの距離を分離軸（平面の法線）上に投影した時の長さを算出
	D3DXVECTOR3 dis = aPos - bPos;
	float s = fabs(D3DXVec3Dot(&dis, &n));
	// 戻す距離を計算
	if (out_L != nullptr) {
		if (s > 0)
			*out_L = r - fabs(s);
		else
			*out_L = r + fabs(s);
		*out_Dir = n;
	}
	// 衝突判定
	// ボックスの中点から各頂点までの長さ(r)がボックスと平面までの長さ(s)よりも大きいなら当たっている
	if (fabs(s)-r < 0.0f) {
		return true;
	}
	return false;
}

//-------------------------------------
// OBBと平面の当たり判定(オーバーロード)
// //----------------------------------
// 戻り値 : 当たったかどうか
//-------------------------------------
// a       : OBB用コリジョン
// bPos    : 平面の中心座標
// bSize   : 平面のサイズ
// bVec[2] : 平面のワールド座標での向き(縦と横)
// bNormal : 平面の法線ベクトル
// out_L   : めり込んだOBBを戻す距離
// out_Dir : めり込んだOBBを戻す方向
//-------------------------------------

bool CollisionManager::Collision_BoxToQuad(BoxCollision* a, D3DXVECTOR3 bPos, 
		D3DXVECTOR2 bSize, D3DXVECTOR3 bVec[2], D3DXVECTOR3 bNormal, float* out_L, D3DXVECTOR3* out_Dir)
{
	D3DXVECTOR3 aPos = a->GetGameObject()->GetTransform()->m_worldPosition;
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight() * (a->GetSize().x);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp() * (a->GetSize().y);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z);

	D3DXVECTOR3 bVecX = bVec[0] * bSize.x;
	D3DXVECTOR3 bVecZ = bVec[1] * bSize.y;


	D3DXVECTOR3 n{};
	D3DXVec3Normalize(&n, &bNormal);

	// 2オブジェクト間の距離
	D3DXVECTOR3 distance = (bPos -
		a->GetGameObject()->GetTransform()->m_worldPosition);

	// ボックスがz方向以外で当たっているかをチェック
	D3DXVECTOR3 nBVecX;
	D3DXVec3Normalize(&nBVecX, &bVecX);
	// オブジェクトの投影線分の半分
	float hpVecA = CreateHalfProjectionLine(&nBVecX, &aVecX, &aVecY, &aVecZ);
	// オブジェクトBの投影線分の半分
	float hpVecB = D3DXVec3Length(&bVecX);
	// 2つのオブジェクトの中心点間の距離を分離軸上に投影
	float dDistance = fabs(D3DXVec3Dot(&distance, &nBVecX));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}


	D3DXVECTOR3 nBVecZ;
	D3DXVec3Normalize(&nBVecZ, &bVecZ);
	// オブジェクトの投影線分の半分
	hpVecA = CreateHalfProjectionLine(&nBVecZ, &aVecX, &aVecY, &aVecZ);
	// オブジェクトBの投影線分の半分
	hpVecB = D3DXVec3Length(&bVecZ);
	// 2つのオブジェクトの中心点間の距離を分離軸上に投影
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecZ));
	if (hpVecA + hpVecB < dDistance) {
		// 衝突していない
		return false;
	}

	// ボックスの各軸の中点から各頂点までの距離を分離軸（平面の法線）上に投影した時の長さを算出
	float r = 0.0f;
	r += fabs(D3DXVec3Dot(&aVecX, &n));
	r += fabs(D3DXVec3Dot(&aVecY, &n));
	r += fabs(D3DXVec3Dot(&aVecZ, &n));

	//　ボックスの中点から平面までの距離を分離軸（平面の法線）上に投影した時の長さを算出
	D3DXVECTOR3 dis = aPos - bPos;
	float s = fabs(D3DXVec3Dot(&dis, &n));
	// 戻す距離を計算
	if (out_L != nullptr) {
		if (s > 0)
			*out_L = r - fabs(s);
		else
			*out_L = r + fabs(s);
		*out_Dir = n;
	}
	// 衝突判定
	// ボックスの中点から各頂点までの長さ(r)がボックスと平面までの長さ(s)よりも大きいなら当たっている
	if (fabs(s) - r < 0.0f) {
		return true;
	}
	return false;

}


#define PREVENT_ERROR 0.00001f		// 誤差用定数
//------------------------------------------------------------------
// レイと球体コリジョンの当たり判定
//------------------------------------------------------------------
// 戻り値 : 当たったかどうか
//------------------------------------------------------------------
// ray        : レイ用クラス
// sphereColl : 球体用コリジョン
// out_T      : 接触した際のレイの時間
// out_HitPos : 接触した際の座標
// 注意! 当たり判定は球体コリジョンのローカル座標で判定しているため
// 接触した座標を使用する場合はワールド座標へ座標変換する必要がある
//-------------------------------------------------------------------
bool CollisionManager::Collision_RayToSphere(Ray* ray, SphereCollision* sphereColl, float* out_T, D3DXVECTOR3* out_HitPos)
{
	// D3DXVECTOR3 distance = *(ray->GetStartPos()) - (sphereColl->GetGameObject()->GetTransform()->m_Position + sphereColl->GetOffset());
	float r = 1.0f;
	double a = D3DXVec3Dot(ray->GetVec(), ray->GetVec());
	double b = D3DXVec3Dot(ray->GetStartPos() ,ray->GetVec());
	double c = D3DXVec3Dot(ray->GetStartPos(), ray->GetStartPos()) - (r * r);

	// 誤差
	// aは単位ベクトルの内積のため誤差以外では必ず1になる
	if (a - PREVENT_ERROR <= 0.0f) {
		return false;
	}

	// isCollがマイナスになる場合はtは実根を持たないためレイと球は当たっていない
	float isColl = b * b - (a * c);
	if (isColl < 0.0f) {
		return false;
	}

	// 解の公式から
	float t = ( -b - sqrt(b * b - a * c)) / a;

	if (out_T) {
		*out_T = t;
	}
	if (out_HitPos) {
		*out_HitPos = ray->GetRayPos(t);
	}

	return true;
}

//-----------------------------------------------------------------------
// レイとボックスコリジョンの当たり判定
//-----------------------------------------------------------------------
// 戻り値 : 当たったかどうか
//-----------------------------------------------------------------------
// ray			: レイ用クラス
// boxColl		: ボックス用コリジョンのサイズ(boxCollision.objのサイズ)
// out_T		: 接触した際のレイの時間
// out_HitPos	: 接触した際の座標
// 注意! 当たり判定はボックスコリジョンのローカル座標で判定しているため
// 接触した座標を使用する場合はワールド座標へ座標変換する必要があります
//------------------------------------------------------------------------
bool CollisionManager::Collision_RayToBox(Ray* ray, BoxCollision* boxColl, float* out_T, D3DXVECTOR3* out_HitPos)
{
	D3DXVECTOR3 h = D3DXVECTOR3(1.0f,1.0f,1.0f);
	D3DXVECTOR3 p = *(ray->GetStartPos());
	D3DXVECTOR3 d = *(ray->GetVec());
	bool xFlag = false, yFlag = false, zFlag = false;
	float tx1 = 0.0f;
	float tx2 = 0.0f;
	float ty1 = 0.0f;
	float ty2 = 0.0f;
	float tz1 = 0.0f;
	float tz2 = 0.0f;
	// X軸方向
	if (d.x == 0.0f) {
		// -h.x<=p.x<=h.xでないなら当たっていない
		if (-h.x > p.x || p.x > h.x)
			return false;
		xFlag = true;
	}
	else {
		// tx1 = min{(h.x-p.x)/d.x , (-h.x-p.x)/d.x}
		// tx2 = max{(h.x-p.x)/d.x , (-h.x-p.x)/d.x}
		tx1 = std::min((h.x - p.x) / d.x, ( -h.x - p.x) / d.x);
		tx2 = std::max((h.x - p.x) / d.x, (-h.x - p.x) / d.x);
	}
	if (d.y == 0.0f) {
		// Y軸方向
		if (-h.y > p.y || p.y > h.y)
			return false;
		yFlag = true;
	}
	else {
		ty1 = std::min((h.y - p.y) / d.y, (-h.y - p.y) / d.y);
		ty2 = std::max((h.y - p.y) / d.y, (-h.y - p.y) / d.y);
	}
	// Z軸方向
	if (d.z == 0.0f) {
		if (-h.z > p.z || p.z > h.z)
			return false;
		zFlag = true;
	}
	else {
		tz1 = std::min((h.z - p.z) / d.z, (-h.z - p.z) / d.z);
		tz2 = std::max((h.z - p.z) / d.z, (-h.z - p.z) / d.z);
	}

	// 各軸のt1,t2からtを算出
	float t = -99999.0f;

	if (!xFlag) {
		if (tx1 >= 0.0f) {
			if (tx1 <= ty2 && tx1 <= tz2 &&
					tx1 >= ty1 && tx1 >= tz1) {
				if (t >= tx1 || t < 0.0f) {
					t = tx1;
				}
			}
		}
	}
	if (!yFlag) {
		if (ty1 >= 0.0f) {
			if (ty1 <= tx2 && ty1 <= tz2 &&
					ty1 >= tx1 && ty1 >= tz1) {
				if (t >= ty1 || t < 0.0f) {
					t = ty1;
				}
			}
		}
	}
	if (!zFlag) {
		if (tz1 >= 0.0f) {
			if (tz1 <= tx2 && tz1 <= ty2 &&
					tz1 >= tx1 && tz1 >= ty1) {
				if (t >= tz1 || t < 0.0f) {
					t = tz1;
				}
			}
		}
	}



	if (t >= 0.0f) {
		if (out_T) {
			*out_T = t;
		}
		if (out_HitPos) {
			*out_HitPos = ray->GetRayPos(t);
		}
		return true;
	}
	else {
		return false;
	}
}

bool CollisionManager::Collision_RayToPolygon(Ray* ray, PolygonCollision* polyColl, float* out_T)
{
	const float EPSILON = 1e-6f;	// 計算誤差無視用変数
	POLYGON_POSITION* polyPosArray = polyColl->GetPolygonArray();	// ポリゴンの頂点座標配列
	// ポリゴンの数だけループ
	for (int i = 0; i < polyColl->GetArrayCount(); i++) {
		POLYGON_POSITION poly = polyPosArray[i];
		D3DXVECTOR3 edgeA = poly.position[1] - poly.position[0];
		D3DXVECTOR3 edgeB = poly.position[2] - poly.position[0];

		D3DXVECTOR3 v1;
		D3DXVec3Cross(&v1, ray->GetVec(), &edgeB);
		float det = D3DXVec3Dot(&edgeA, &v1);

		// 三角ポリゴンにレイが平行に交わっている時にdet = 0になる
		// detが0に限りなく近いなら交差していない判定にする
		if (-EPSILON < det && det < EPSILON) continue;

		float invDet = 1.0f / det;
		D3DXVECTOR3 r = *(ray->GetStartPos()) - poly.position[0];
		// uが 0<= u <= 1 をではないなら当たっていない
		float u = D3DXVec3Dot(&v1, &r) * invDet;
		if (u < 0.0f || u>1.0f) continue;

		D3DXVECTOR3 v2;
		D3DXVec3Cross(&v2, &r, &edgeA);
		// vが 0 <= v <= 1 かつ u + v <= 1 でないなら当たっていない
		float v = D3DXVec3Dot(ray->GetVec(), &v2) * invDet;
		if (v < 0.0f || u + v>1.0f) continue;

		// tが 0 <= t なら当たってない
		float t = D3DXVec3Dot(&edgeB, &v2) * invDet;
		if (t < 0.0f) continue;

		// 当たっている
		*out_T = t;
		return true;
	}
	// 全てのポリゴンが当たらなかったら当たっていない
	return false;
}


//------------------------------------------------------------------
// スクリーン座標の座標をローカル座標へ変換する関数
//------------------------------------------------------------------
// 戻り値 : なし
//------------------------------------------------------------------
// worldMatrix		: ワールド変換行列
// viewMatrix		: ビュー変換行列
// projectionMatrix	: プロジェクション変換行列
// mousePos			: 変換するスクリーン座標
// mouseZ			: スクリーン座標に付け加えるz成分(0.0～1.0)
// out_Pos			: 変換後の座標
//-------------------------------------------------------------------
void CollisionManager::ScreenToLocalPosition(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, 
						D3DXMATRIX* projectionMatrix, POINT mousePos, float mouseZ, D3DXVECTOR3* out_Pos)
{
	// ビューポート行列を作成
	D3DXMATRIX vpMatrix;
	D3DXMatrixIdentity(&vpMatrix);
	D3D11_VIEWPORT vp;
	UINT numViewports = 1;
	Renderer::GetDeviceContext()->RSGetViewports(&numViewports, &vp);

	vpMatrix._11 = (float)vp.Width / 2;
	vpMatrix._22 = -1.0f * (float)(vp.Height / 2);
	vpMatrix._33 = (float)vp.MaxDepth - vp.MinDepth;
	vpMatrix._41 = (float)(vp.TopLeftX + vp.Width / 2);
	vpMatrix._42 = (float)(vp.TopLeftY + vp.Height / 2);
	vpMatrix._43 = vp.MinDepth;

	D3DXVECTOR3 world;
	world.x = (float)mousePos.x;
	world.y = (float)mousePos.y;
	world.z = mouseZ;


	D3DXMATRIX invMatrix, invViewport, invPorjection, invView, invWorld;
	// ビューポート行列の逆行列を作成
	D3DXMatrixInverse(&invViewport, 0, &vpMatrix);
	// プロジェクション行列の逆行列を作成
	D3DXMatrixInverse(&invPorjection, 0, projectionMatrix);
	// ビュー行列の逆行列
	D3DXMatrixInverse(&invView, 0, viewMatrix);
	// ワールド行列の逆行列
	D3DXMatrixInverse(&invWorld, 0, worldMatrix);
	// 全ての逆行列をかけてローカル座標系へ
	invMatrix = invViewport * invPorjection * invView * invWorld;
	// スクリーン座標をローカル座標へ
	D3DXVec3TransformCoord(&world, &world, &invMatrix);

	if (out_Pos) {
		*out_Pos = world;
	}

}

