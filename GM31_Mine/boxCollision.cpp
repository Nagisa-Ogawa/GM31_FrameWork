
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "collisionManager.h"
#include "boxCollisionFrame.h"
#include "dispInspector.h"
#include "collisionMesh.h"


void BoxCollision::Init()
{
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_isTrigger = false;
	std::string name = m_gameObject->GetName() + "BoxCollisionFrame";
	m_collFrame = Manager::GetInstance()->GetScene()->GetEditor()->AddEditorObject<BoxCollisionFrame>(1, name);
	m_collFrame->Init(this);
	m_collID = m_collFrame->GetID();

	CollisionManager::GetInstance()->AddBoxCollision(this);

	m_collisionMesh = new CollisionMesh();
	// 当たり判定用メッシュデータをファイルから読み込む
	m_collisionMesh->CreateCollisionMesh("Assets\\Models\\CollisionData_Box.obj", m_size);
}

void BoxCollision::Load()
{
	m_collFrame = Manager::GetInstance()->GetScene()->GetEditor()->GetEditorObjectWithID<BoxCollisionFrame>(m_collID);
	m_collFrame->SetCollision(this);
	CollisionManager::GetInstance()->AddBoxCollision(this);

	m_collisionMesh = new CollisionMesh();
	// 当たり判定用メッシュデータをファイルから読み込む
	m_collisionMesh->CreateCollisionMesh("Assets\\Models\\CollisionData_Box.obj", m_size);

}

void BoxCollision::Uninit()
{
	// 当たり判定マネージャーのリストから削除
	CollisionManager::GetInstance()->DeleteBoxCollision(this);
	// 当たり判定表示用オブジェクトを削除
	m_collFrame->SetDestroy();

	m_collisionMesh->Uninit();
	delete m_collisionMesh;
}

void BoxCollision::Update()
{
}

void BoxCollision::Draw()
{
}

void BoxCollision::DispInspector()
{
	DispComponent(this);
}

D3DXMATRIX* BoxCollision::GetWorldMatrix()
{
	// マトリクス設定
	D3DXMATRIX scale, trans, world;
	D3DXMatrixScaling(&scale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixTranslation(&trans, m_offset.x, m_offset.y, m_offset.z);
	world = scale * trans;
	auto transform = m_gameObject->GetTransform();
	D3DXMATRIX worldRot = transform->GetWorldRotMatrix();
	D3DXMATRIX worldTrans = transform->GetWorldTransMatrix();
	m_worldMatrix = world * (worldRot * worldTrans);

	return &m_worldMatrix;
}


/// <summary>
/// 直方体(ボックス)の慣性テンソルを算出する関数
/// </summary>
/// <param name="mass">剛体の質量</param>
/// <returns>算出した慣性テンソル</returns>
D3DXMATRIX BoxCollision::CalcInertiaBox(float mass)
{
	// 直方体の慣性テンソル(3x3行列)

	//     |														|
	//     |   1/12m(y^2 + z^2)			0				0			|
	//     |														|
	// I = |		0			1/12m(z^2 + x^2)		0			|
	//     |														|
	//     |		0					0		1/12m(x^2 + y^2)	|
	//     |														|

	//-----------------------------------------------------------------
	
	// 各軸の長さの2乗を計算
	D3DXVECTOR3 sqrSize = m_size * 2.0f;
	sqrSize = D3DXVECTOR3(sqrSize.x * sqrSize.x, sqrSize.y * sqrSize.y, sqrSize.z * sqrSize.z);

	D3DXMATRIX inertia; 
	D3DXMatrixIdentity(&inertia);

	inertia.m[0][0] = (mass * (sqrSize.y + sqrSize.z)) / 12.0f;
	inertia.m[1][1] = (mass * (sqrSize.x + sqrSize.z)) / 12.0f;
	inertia.m[2][2] = (mass * (sqrSize.x + sqrSize.y)) / 12.0f;

	return inertia;
}

