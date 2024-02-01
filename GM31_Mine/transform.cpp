#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "transform.h"
#include "dispInspector.h"



void Transform::Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	m_localPosition = position;
	m_localRotation = rotation;
	m_localScale = scale;
}

void Transform::Load()
{
	// 親IDが-1でないなら親にセット
	if (m_parentID != -1)
	{	
		// 親IDから親オブジェクトを取得
		auto parentObject = Manager::GetInstance()->GetScene()->GetGameObjectWithID(m_parentID);
		auto parent = parentObject->GetTransform();
		// 親として登録
		m_parent = parent;
		// 親側にも子供として登録
		parent->SetChild(this);
		
	}
	else {
		// 親がいないのなら一番上の親として登録
		Manager::GetInstance()->GetScene()->AddParentObject(this);
	}
}


void Transform::Uninit()
{
}

void Transform::Update()
{
}

void Transform::Draw()
{
	Renderer::SetWorldMatrix(&m_worldMatrix);
}

void Transform::DispInspector()
{
	DispComponent(this);
}

void Transform::MakeLocalMatrix()
{
	// マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_localScale.x, m_localScale.y, m_localScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_localRotation.y, m_localRotation.x, m_localRotation.z);
	D3DXMatrixTranslation(&trans, m_localPosition.x, m_localPosition.y, m_localPosition.z);
	m_localMatrix = scale * rot * trans;
}

/// <summary>
/// 親をセットする関数
/// 引数がからなら一番上の親になる
/// </summary>
/// <param name="parent"></param>
void Transform::SetParent(Transform* parent)
{
	if (m_parent == nullptr) {
		// 自分に親がいないならシーンの親リストから削除
		Manager::GetInstance()->GetScene()->DeleteParentObject(this);
	}
	else {
		// 自分に親がいるならその親から自分を削除
		m_parent->DeleteChild(this);
	}
	if (parent == nullptr) {
		// 引数の親がnullptrなら親のいないオブジェクトになる
		Manager::GetInstance()->GetScene()->AddParentObject(this);
		// IDを親なしにする
		m_parentID = -1;
	}
	else {
		// 新しい親の子供として登録
		parent->SetChild(this);
		// 新しい親を親として登録
		m_parent = parent;
		// IDを親のIDにする
		m_parentID = parent->m_gameObject->GetID();
	}
}


D3DXVECTOR3 Transform::GetWorldRotationAsDegree()
{
	D3DXVECTOR3 deg;
	deg.x = m_worldRotation.x * (180.0f / D3DX_PI);
	deg.y = m_worldRotation.y * (180.0f / D3DX_PI);
	deg.z = m_worldRotation.z * (180.0f / D3DX_PI);

	return deg;
}

D3DXVECTOR3 Transform::GetLocalRotationAsDegree()
{
	D3DXVECTOR3 deg;
	deg.x = m_localRotation.x * (180.0f / D3DX_PI);
	deg.y = m_localRotation.y * (180.0f / D3DX_PI);
	deg.z = m_localRotation.z * (180.0f / D3DX_PI);

	return deg;
}

D3DXMATRIX Transform::GetWorldScaleMatrix()
{
	D3DXVECTOR3 vec1, vec2, vec3;
	vec1 = D3DXVECTOR3(m_worldMatrix._11, m_worldMatrix._12, m_worldMatrix._13);
	vec2 = D3DXVECTOR3(m_worldMatrix._21, m_worldMatrix._22, m_worldMatrix._23);
	vec3 = D3DXVECTOR3(m_worldMatrix._31, m_worldMatrix._32, m_worldMatrix._33);
	D3DXMATRIX scaleMatrix;
	D3DXMatrixScaling(&scaleMatrix,D3DXVec3Length(&vec1),D3DXVec3Length(&vec2),D3DXVec3Length(&vec3));
	return scaleMatrix;
}

D3DXMATRIX Transform::GetWorldRotMatrix()
{
	D3DXMATRIX inverseScale = GetWorldScaleMatrix();
	D3DXMATRIX inverseTrans = GetWorldTransMatrix();
	D3DXMatrixInverse(&inverseScale, NULL, &inverseScale);
	D3DXMatrixInverse(&inverseTrans, NULL, &inverseTrans);
	D3DXMATRIX rotMatrix = inverseScale * m_worldMatrix * inverseTrans;
	return rotMatrix;
}

D3DXMATRIX Transform::GetWorldTransMatrix()
{
	D3DXMATRIX transMatrix;
	D3DXMatrixTranslation(&transMatrix, m_worldMatrix._41, m_worldMatrix._42, m_worldMatrix._43);
	return transMatrix;
}

/// <summary>
/// 回転を度値からラジアン値に変換しセットする関数
/// </summary>
/// <param name="deg">セットする度値</param>
void Transform::SetWorldRotationFromDegree(D3DXVECTOR3 deg)
{
	m_worldRotation.x = deg.x * (D3DX_PI / 180.0f);
	m_worldRotation.y = deg.y * (D3DX_PI / 180.0f);
	m_worldRotation.z = deg.z * (D3DX_PI / 180.0f);
}

void Transform::SetLocalRotationFromDegree(D3DXVECTOR3 deg)
{
	m_localRotation.x = deg.x * (D3DX_PI / 180.0f);
	m_localRotation.y = deg.y * (D3DX_PI / 180.0f);
	m_localRotation.z = deg.z * (D3DX_PI / 180.0f);
}
/// <summary>
/// ローカル行列と親のワールド行列からワールド行列を作成する関数
/// </summary>
/// <param name="parentWorldMatrix"></param>
void Transform::MakeWorldMatrix(D3DXMATRIX* parentWorldMatrix)
{
	// ローカル行列を作成
	MakeLocalMatrix();
	if (parentWorldMatrix == nullptr) {
		// 親オブジェクトがいないならローカル行列をそのままワールド行列へ
		m_worldMatrix = m_localMatrix;
	}
	else{
		// 親オブジェクトがいる場合は親のワールド変換行列と掛け合わせる
		m_worldMatrix = m_localMatrix * (*parentWorldMatrix);
	}
	// 自分の子オブジェクトのワールド行列作成関数を呼び出す
	for (Transform* transform : m_childList) {
		transform->MakeWorldMatrix(&m_worldMatrix);
	}
}
