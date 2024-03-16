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
/// �e�I�u�W�F�N�g�����[�h���鏈��
/// </summary>
void Transform::LoadParent()
{
	// �eID��-1�łȂ��Ȃ�e�ɃZ�b�g
	if (m_parentID != -1)
	{
		// �eID����e�I�u�W�F�N�g���擾
		auto parentObject = Manager::GetInstance()->GetScene()->GetGameObjectWithID(m_parentID);
		auto parent = parentObject->GetTransform();
		// �e�Ƃ��ēo�^
		m_parent = parent;
		// �e���ɂ��q���Ƃ��ēo�^
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
	// �}�g���N�X�ݒ�
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_localScale.x, m_localScale.y, m_localScale.z);
	// �N�H�[�^�j�I������]�s���
	D3DXMatrixRotationQuaternion(&rot, &m_localQuaternion);
	D3DXMatrixTranslation(&trans, m_localPosition.x, m_localPosition.y, m_localPosition.z);
	m_localMatrix = scale * rot * trans;
}

/// <summary>
/// �e���Z�b�g����֐�
/// ����������Ȃ��ԏ�̐e�ɂȂ�
/// </summary>
/// <param name="parent"></param>
void Transform::SetParent(Transform* parent)
{
	if (m_parent) {
		// �����ɐe������Ȃ炻�̐e���玩�����폜
		m_parent->DeleteChild(this);
	}
	if (parent) {
		// �V�����e�̎q���Ƃ��ēo�^
		parent->SetChild(this);
		// �V�����e��e�Ƃ��ēo�^
		m_parent = parent;
		// ID��e��ID�ɂ���
		m_parentID = parent->m_gameObject->GetID();
	}
	else {
		m_parent = nullptr;
		// ID��e�Ȃ��ɂ���
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
/// ��]��x�l���烉�W�A���l�ɕϊ����Z�b�g����֐�
/// </summary>
/// <param name="deg">�Z�b�g����x�l</param>
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
	// �N�H�[�^�j�I�����X�V
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
/// ���[�J���s��Ɛe�̃��[���h�s�񂩂烏�[���h�s����쐬����֐�
/// </summary>
/// <param name="parentWorldMatrix"></param>
void Transform::MakeWorldMatrix(D3DXMATRIX* parentWorldMatrix)
{
	// ���[�J���s����쐬
	MakeLocalMatrix();
	if (parentWorldMatrix == nullptr) {
		// �e�I�u�W�F�N�g�����Ȃ��Ȃ烍�[�J���s������̂܂܃��[���h�s���
		m_worldMatrix = m_localMatrix;
	}
	else{
		// �e�I�u�W�F�N�g������ꍇ�͐e�̃��[���h�ϊ��s��Ɗ|�����킹��
		m_worldMatrix = m_localMatrix * (*parentWorldMatrix);
	}
	// �����̎q�I�u�W�F�N�g�̃��[���h�s��쐬�֐����Ăяo��
	for (Transform* transform : m_childList) {
		transform->MakeWorldMatrix(&m_worldMatrix);
	}
}
