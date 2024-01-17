#pragma once

#include "component.h"


class Transform : public Component
{
private:
	void SetChild(Transform* child) { m_childList.push_back(child); }	// �q����ݒ肷��֐�
	void DeleteChild(Transform* child) { m_childList.remove(child); }	// �q�����폜����֐�
public:
	Transform* m_parent = nullptr;
	std::list<Transform*> m_childList;

	D3DXVECTOR3 m_worldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_worldRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_worldScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_localRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXMATRIX  m_worldMatrix{};
	D3DXMATRIX m_localMatrix{};

	// �E�����x�N�g���擾
	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_localRotation.y, m_localRotation.x, m_localRotation.z);

		D3DXVECTOR3 right;
		right.x = rot._11;
		right.y = rot._12;
		right.z = rot._13;

		return right;
	}

	// ������x�N�g���擾
	D3DXVECTOR3 GetUp()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_localRotation.y, m_localRotation.x, m_localRotation.z);

		D3DXVECTOR3 up;
		up.x = rot._21;
		up.y = rot._22;
		up.z = rot._23;

		return up;
	}

	// �O�����x�N�g���擾
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_localRotation.y, m_localRotation.x, m_localRotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	void Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void Uninit() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetWorldRotationAsDegree();				// ��]�l��x�l�Ŏ擾����֐�
	D3DXVECTOR3 GetLocalRotationAsDegree();				
	D3DXMATRIX* GetWorldMatrix() { return &m_worldMatrix; }		// ���[���h�s����擾����֐�
	Transform* GetParent() { return m_parent; }		// �e�I�u�W�F�N�g��Transform���擾
	std::list<Transform*> GetChildList() { return m_childList; }	// �q�I�u�W�F�N�g�̃��X�g���擾

	void SetWorldRotationFromDegree(D3DXVECTOR3 deg);	// ��]��x�l���烉�W�A���l�ɕϊ����Z�b�g����֐�
	void SetLocalRotationFromDegree(D3DXVECTOR3 deg);	// ��]��x�l���烉�W�A���l�ɕϊ����Z�b�g����֐�
	void SetWorldMatrix(D3DXMATRIX* matrix) { m_worldMatrix = *matrix; }	// ���[���h�s����Z�b�g����֐�
	void SetParent(Transform* parent);		// �e��ݒ肷��֐�

	void MakeLocalMatrix();		// ���[�J���s����쐬����֐�
	void MakeWorldMatrix(D3DXMATRIX* parentWorldMatrix);	// ���[�J���s��Ɛe�̃��[���h�s�񂩂烏�[���h�s����쐬����֐�
	void DispInspector() override;

};
