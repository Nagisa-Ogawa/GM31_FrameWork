#pragma once

#include "SerializableClass.h"
#include "component.h"


//-----------------------------------------------------
// Transform�R���|�[�l���g�N���X
//------------------------------------------------------
// WorldPosition,WorldRotation,WorldScale�͖��g�p�ł�
//------------------------------------------------------
class Transform : public Component
{
private:
	std::list<Transform*> m_childList;
	Transform* m_parent = nullptr;
	int m_parentID = -1;

	D3DXVECTOR3 m_worldEulerAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_localEulerAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMATRIX m_worldMatrix{};
	D3DXMATRIX m_localMatrix{};

	void SetChild(Transform* child) { m_childList.push_back(child); }	// �q����ݒ肷��֐�
	void DeleteChild(Transform* child) { m_childList.remove(child); }	// �q�����폜����֐�
	void UpdateQuaternion();		// �I�C���[�p�ŉ�]��ύX�����ۂɃN�H�[�^�j�I�����X�V����֐�
public:
	// ���g�p
	D3DXVECTOR3 m_worldPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_worldQuaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 m_worldScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_localQuaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);


	// �E�����x�N�g���擾
	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationQuaternion(&rot, &m_localQuaternion);

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
		D3DXMatrixRotationQuaternion(&rot, &m_localQuaternion);

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
		D3DXMatrixRotationQuaternion(&rot, &m_localQuaternion);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}

	void Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void Load() override;
	void LoadParent();
	void Uninit() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetWorldEulerAngleAsDegree();				// ��]�l��x�l�Ŏ擾����֐�
	D3DXVECTOR3 GetLocalEulerAngleAsDegree();
	D3DXVECTOR3 GetLocalEulerAngle();
	D3DXMATRIX* GetWorldMatrix() { return &m_worldMatrix; }		// ���[���h�s����擾����֐�
	D3DXMATRIX* GetLocalMatrix() { return &m_localMatrix; }
	Transform* GetParent() { return m_parent; }		// �e�I�u�W�F�N�g��Transform���擾
	std::list<Transform*> GetChildList() { return m_childList; }	// �q�I�u�W�F�N�g�̃��X�g���擾
	// ���[���h�ϊ��s��̊e�����𔲂��o���֐�
	D3DXMATRIX GetWorldScaleMatrix();
	D3DXMATRIX GetWorldRotMatrix();
	D3DXMATRIX GetWorldTransMatrix();

	void SetWorldEulerAngleFromDegree(D3DXVECTOR3 deg);	// ��]��x�l���烉�W�A���l�ɕϊ����Z�b�g����֐�
	void SetLocalEulerAngleFromDegree(D3DXVECTOR3 deg);	// ��]��x�l���烉�W�A���l�ɕϊ����Z�b�g����֐�
	void SetLocalEulerAngle(D3DXVECTOR3 euler);
	void SetWorldMatrix(D3DXMATRIX* matrix);		// ���[���h�s����Z�b�g����֐�
	void SetLocalMatrix(D3DXMATRIX* matrix);
	void SetParent(Transform* parent);		// �e��ݒ肷��֐�

	void MakeLocalMatrix();		// ���[�J���s����쐬����֐�
	void MakeWorldMatrix(D3DXMATRIX* parentWorldMatrix);	// ���[�J���s��Ɛe�̃��[���h�s�񂩂烏�[���h�s����쐬����֐�
	void DispInspector() override;

	template <class Archive>
	void save(Archive& archive) const
	{
		Vector3 worldPostion = m_worldPosition;
		Vector4 worldQuaternion = m_worldQuaternion;
		Vector3 worldEulerAngle = m_worldEulerAngle;
		Vector3 worldScale = m_worldScale;
		Vector3 localPostion = m_localPosition;
		Vector4 localQuaternion = m_localQuaternion;
		Vector3 localEulerAngle = m_localEulerAngle;
		Vector3 localScale = m_localScale;

		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_parentID),
			CEREAL_NVP(worldPostion),
			CEREAL_NVP(worldQuaternion),
			CEREAL_NVP(worldEulerAngle),
			CEREAL_NVP(worldScale),
			CEREAL_NVP(localPostion),
			CEREAL_NVP(localQuaternion),
			CEREAL_NVP(localEulerAngle),
			CEREAL_NVP(localScale)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		Vector3 worldPostion, worldEulerAngle, worldScale,  localPosition, localEulerAngle, localScale;
		Vector4  worldQuaternion, localQuaternion;
		archive(
			cereal::base_class<Component>(this),
			m_parentID,
			worldPostion,
			worldQuaternion,
			worldEulerAngle,
			worldScale,
			localPosition,
			localQuaternion,
			localEulerAngle,
			localScale
		);
		m_worldPosition = D3DXVECTOR3(worldPostion.x, worldPostion.y, worldPostion.z);
		m_worldQuaternion = D3DXQUATERNION(worldQuaternion.x, worldQuaternion.y, worldQuaternion.z, worldQuaternion.w);
		m_worldEulerAngle = D3DXVECTOR3(worldEulerAngle.x, worldEulerAngle.y, worldEulerAngle.z);
		m_worldScale = D3DXVECTOR3(worldScale.x, worldScale.y, worldScale.z);
		m_localPosition = D3DXVECTOR3(localPosition.x, localPosition.y, localPosition.z);
		m_localQuaternion = D3DXQUATERNION(localQuaternion.x, localQuaternion.y, localQuaternion.z, localQuaternion.w);
		m_localEulerAngle = D3DXVECTOR3(localEulerAngle.x, localEulerAngle.y, localEulerAngle.z);
		m_localScale = D3DXVECTOR3(localScale.x, localScale.y, localScale.z);
	}

};

CEREAL_REGISTER_TYPE(Transform);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Transform);

