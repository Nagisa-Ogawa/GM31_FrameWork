
#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "quadCollision.h"
#include "CollisionManager.h"

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
	if (coll == nullptr)
		return;
	if (coll->GetIsTrigger()) {
		m_BoxTriList.push_back(coll);
	}
	else {
		m_BoxCollList.push_back(coll);
	}
}

void CollisionManager::AddQuadCollision(QuadCollision * coll)
{
	if (coll == nullptr)
		return;
	if (coll->GetIsTrigger()) {
		m_QuadTriList.push_back(coll);
	}
	else {
		m_QuadCollList.push_back(coll);
	}
}

bool CollisionManager::Collision_BoxToBox(BoxCollision* a, BoxCollision* b)
{
	// 2オブジェクトの各軸を定義
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight() *   (a->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp() *      (a->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z /*/ 2.0f*/);
	D3DXVECTOR3 bVecX = b->GetGameObject()->GetTransform()->GetRight() *   (b->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 bVecY = b->GetGameObject()->GetTransform()->GetUp() *      (b->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 bVecZ = b->GetGameObject()->GetTransform()->GetForward() * (b->GetSize().z /*/ 2.0f*/);
	// 2オブジェクト間の距離
	D3DXVECTOR3 distance = (b->GetGameObject()->GetTransform()->m_Position -
								a->GetGameObject()->GetTransform()->m_Position);

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
	return true;
}

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


bool CollisionManager::Collision_BoxToQuad(BoxCollision * a, QuadCollision * b, float* l, D3DXVECTOR3* dir)
{
	D3DXVECTOR3 aPos = a->GetGameObject()->GetTransform()->m_Position;
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight()   * (a->GetSize().x);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp()      * (a->GetSize().y);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z);

	D3DXVECTOR3 n{};
	D3DXVec3Normalize(&n, b->GetNormal());
	D3DXVECTOR3 bPos = b->GetGameObject()->GetTransform()->m_Position;

	// ボックスの各軸の中点から各頂点までの距離を分離軸（平面の法線）上に投影した時の長さを算出
	float r = 0.0f;
	r += fabs(D3DXVec3Dot(&aVecX, &n));
	r += fabs(D3DXVec3Dot(&aVecY, &n));
	r += fabs(D3DXVec3Dot(&aVecZ, &n));

	//　ボックスの中点から平面までの距離を分離軸（平面の法線）上に投影した時の長さを算出
	float s = fabs(D3DXVec3Dot(&(aPos - bPos), &n));
	// 戻す距離を計算
	if (l != nullptr) {
		if (s > 0)
			*l = r - fabs(s);
		else
			*l = r + fabs(s);
		*dir = n;
	}
	// 衝突判定
	// ボックスの中点から各頂点までの長さ(r)がボックスと平面までの長さ(s)よりも大きいなら当たっている
	if (fabs(s)-r < 0.0f) {
		return true;
	}
	return false;
}

