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
		// �e�����Ȃ��̂Ȃ��ԏ�̐e�Ƃ��ēo�^
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
	// �}�g���N�X�ݒ�
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_localScale.x, m_localScale.y, m_localScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_localRotation.y, m_localRotation.x, m_localRotation.z);
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
	if (m_parent == nullptr) {
		// �����ɐe�����Ȃ��Ȃ�V�[���̐e���X�g����폜
		Manager::GetInstance()->GetScene()->DeleteParentObject(this);
	}
	else {
		// �����ɐe������Ȃ炻�̐e���玩�����폜
		m_parent->DeleteChild(this);
	}
	if (parent == nullptr) {
		// �����̐e��nullptr�Ȃ�e�̂��Ȃ��I�u�W�F�N�g�ɂȂ�
		Manager::GetInstance()->GetScene()->AddParentObject(this);
		// ID��e�Ȃ��ɂ���
		m_parentID = -1;
	}
	else {
		// �V�����e�̎q���Ƃ��ēo�^
		parent->SetChild(this);
		// �V�����e��e�Ƃ��ēo�^
		m_parent = parent;
		// ID��e��ID�ɂ���
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
/// ��]��x�l���烉�W�A���l�ɕϊ����Z�b�g����֐�
/// </summary>
/// <param name="deg">�Z�b�g����x�l</param>
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
