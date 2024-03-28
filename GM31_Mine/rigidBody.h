#pragma once

#include "component.h"

// ���̂̂ӂ�܂�
enum MOTION_TYPE
{
	DYNAMIC,	// �A�N�e�B�u
	STATIC		// �Œ�
};


//-----------------------------------------------------
// ���̑����R���|�[�l���g�N���X
//------------------------------------------------------
class RigidBody : public Component
{
private:
	D3DXVECTOR3 m_gravity = D3DXVECTOR3(0.0f, -9.8f, 0.0f);	// �d��
	D3DXMATRIX	m_inertia{};		// �����e���\��
	float		m_mass{};			// ����
	float		m_restitution{};	// �����W��
	float		m_friction{};		// ���C�W��

	D3DXVECTOR3 m_velocity{};		// ���i���x
	D3DXVECTOR3 m_angularVelocity{};// ��]���x
	MOTION_TYPE m_motionType = MOTION_TYPE::DYNAMIC;	// ���̂̂ӂ�܂�

public:
	void Init() override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	// Get�n�֐�
	D3DXVECTOR3 GetGravity() { return m_gravity; }
	D3DXMATRIX	GetInertia() { return m_inertia; }
	float		GetMass() { return m_mass; }
	MOTION_TYPE GetMotionType() { return m_motionType; }
	D3DXVECTOR3 GetVelocity() { return m_velocity; }
	D3DXVECTOR3 GetAngularVelocity() { return m_angularVelocity; }
	// Set�n�֐�
	void SetInertia(D3DXMATRIX inertia) { m_inertia = inertia; }
	void SetMass(float mass) { m_mass = mass; }
	void SetMotionType(MOTION_TYPE type) { m_motionType = type; }
	void SetVelocity(D3DXVECTOR3 vec) { m_velocity = vec; }
	void SetAngularVelocity(D3DXVECTOR3 aVec) { m_angularVelocity = aVec; }

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component>(this)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(RigidBody);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidBody);

