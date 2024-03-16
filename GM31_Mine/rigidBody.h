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


	//template <class Archive>
	//void save(Archive& archive) const
	//{
	//	archive(
	//		cereal::base_class<Component>(this),
	//		CEREAL_NVP(m_fileNameVS),
	//		CEREAL_NVP(m_fileNamePS)
	//	);
	//}

	//template <class Archive>
	//void load(Archive& archive)
	//{
	//	archive(
	//		cereal::base_class<Component>(this),
	//		m_fileNameVS,
	//		m_fileNamePS
	//	);
	//}

};

//CEREAL_REGISTER_TYPE(Shader);
//CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Shader);

