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

