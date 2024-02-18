#pragma once

#include "SerializableClass.h"
#include "component.h"

//-----------------------------------------------------
// カメラコンポーネントクラス
//------------------------------------------------------
class Camera :public Component
{
private:
	// 未使用
	D3DXVECTOR3 m_position{};
	D3DXVECTOR3 m_rotation{};
	D3DXVECTOR3 m_right{};
	D3DXVECTOR3 m_up{};

	D3DXMATRIX m_cameraMatrix{};	// カメラの行列
	D3DXMATRIX m_viewMatrix{};		// ビュー行列
	D3DXMATRIX m_projectionMatrix{};// プロジェクション行列
public:


	void Init()  override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void DispInspector() override;

	D3DXVECTOR3 GetPosition()	{ return m_position; }
	D3DXVECTOR3 GetRotation()	{ return m_rotation; }
	D3DXVECTOR3 GetUp()			{ return m_up; };

	D3DXMATRIX* GetViewMatrix()	{ return &m_viewMatrix; }
	D3DXMATRIX* GetProjectionMatrix() {return &m_projectionMatrix;}

	void SetPosition(D3DXVECTOR3 position)	{ m_position = position; }
	void SetRotation(D3DXVECTOR3 rotation)	{ m_rotation = rotation; }
	void SetUp(D3DXVECTOR3 up)				{ m_up = up; }


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
		Vector3 position, rotation;
		archive(
			cereal::base_class<Component>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(Camera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Camera);

