#pragma once

#include "component.h"

// 剛体のふるまい
enum MOTION_TYPE
{
	DYNAMIC,	// アクティブ
	STATIC		// 固定
};


//-----------------------------------------------------
// 剛体属性コンポーネントクラス
//------------------------------------------------------
class RigidBody : public Component
{
private:
	D3DXVECTOR3 m_gravity = D3DXVECTOR3(0.0f, -9.8f, 0.0f);	// 重力
	D3DXMATRIX	m_inertia{};		// 慣性テンソル
	float		m_mass{};			// 質量
	float		m_restitution{};	// 反発係数
	float		m_friction{};		// 摩擦係数

	D3DXVECTOR3 m_velocity{};		// 並進速度
	D3DXVECTOR3 m_angularVelocity{};// 回転速度
	MOTION_TYPE m_motionType = MOTION_TYPE::DYNAMIC;	// 剛体のふるまい

public:
	void Init() override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	// Get系関数
	D3DXVECTOR3 GetGravity() { return m_gravity; }
	D3DXMATRIX	GetInertia() { return m_inertia; }
	float		GetMass() { return m_mass; }
	MOTION_TYPE GetMotionType() { return m_motionType; }
	D3DXVECTOR3 GetVelocity() { return m_velocity; }
	D3DXVECTOR3 GetAngularVelocity() { return m_angularVelocity; }
	// Set系関数
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

