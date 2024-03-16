#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "transform.h"
#include "dispInspector.h"



void Transform::UpdateQuaternion()
{
	D3DXQuaternionRotationYawPitchRoll(&m_localQuaternion, m_localEulerAngle.y, m_localEulerAngle.x, m_localEulerAngle.z);
}

void Transform::Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	m_localPosition = position;
	// m_localRotation = rotation;
	m_localEulerAngle = rotation;
	m_localScale = scale;
}

void Transform::Load()
{
}

/// <summary>
/// 親オブジェクトをロードする処理
/// </summary>
void Transform::LoadParent()
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
		m_parent = nullptr;
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
	// クォータニオンを回転行列へ
	D3DXMatrixRotationQuaternion(&rot, &m_localQuaternion);
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
	if (m_parent) {
		// 自分に親がいるならその親から自分を削除
		m_parent->DeleteChild(this);
	}
	if (parent) {
		// 新しい親の子供として登録
		parent->SetChild(this);
		// 新しい親を親として登録
		m_parent = parent;
		// IDを親のIDにする
		m_parentID = parent->m_gameObject->GetID();
	}
	else {
		m_parent = nullptr;
		// IDを親なしにする
		m_parentID = -1;
	}
}


D3DXVECTOR3 Transform::GetWorldEulerAngleAsDegree()
{
	D3DXVECTOR3 deg;
	deg.x = m_worldEulerAngle.x * (180.0f / D3DX_PI);
	deg.y = m_worldEulerAngle.y * (180.0f / D3DX_PI);
	deg.z = m_worldEulerAngle.z * (180.0f / D3DX_PI);

	return deg;
}

D3DXVECTOR3 Transform::GetLocalEulerAngleAsDegree()
{
	D3DXVECTOR3 deg;
	deg.x = m_localEulerAngle.x * (180.0f / D3DX_PI);
	deg.y = m_localEulerAngle.y * (180.0f / D3DX_PI);
	deg.z = m_localEulerAngle.z * (180.0f / D3DX_PI);

	return deg;
}

D3DXVECTOR3 Transform::GetLocalEulerAngle()
{
	return m_localEulerAngle;
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
void Transform::SetWorldEulerAngleFromDegree(D3DXVECTOR3 deg)
{
	m_worldEulerAngle.x = deg.x * (D3DX_PI / 180.0f);
	m_worldEulerAngle.y = deg.y * (D3DX_PI / 180.0f);
	m_worldEulerAngle.z = deg.z * (D3DX_PI / 180.0f);
}

void Transform::SetLocalEulerAngleFromDegree(D3DXVECTOR3 deg)
{
	m_localEulerAngle.x = deg.x * (D3DX_PI / 180.0f);
	m_localEulerAngle.y = deg.y * (D3DX_PI / 180.0f);
	m_localEulerAngle.z = deg.z * (D3DX_PI / 180.0f);
	// クォータニオンを更新
	UpdateQuaternion();
}


void Transform::SetLocalEulerAngle(D3DXVECTOR3 euler)
{
	m_localEulerAngle = euler;
	UpdateQuaternion();
}

void Transform::SetWorldMatrix(D3DXMATRIX* matrix)
{
	m_worldMatrix = *matrix;
}

void Transform::SetLocalMatrix(D3DXMATRIX* matrix)
{
	m_localMatrix = *matrix;
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
