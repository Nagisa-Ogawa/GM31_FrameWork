#pragma once

#include "SerializableClass.h"
#include "component.h"


class EditorCamera :public Component
{
private:
	D3DXVECTOR3 m_position{};
	D3DXVECTOR3 m_rotation{};
	D3DXVECTOR3 m_right{};
	D3DXVECTOR3 m_up{};

	D3DXMATRIX m_cameraMatrix{};
	D3DXMATRIX m_viewMatrix{};
	D3DXMATRIX m_projectionMatrix{};
public:


	void Init(D3DXVECTOR3 position);
	void Uninit() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetPosition() { return m_position; }
	D3DXVECTOR3 GetRotation() { return m_rotation; }
	D3DXVECTOR3 GetUp() { return m_up; };

	D3DXMATRIX* GetViewMatrix() { return &m_viewMatrix; }
	D3DXMATRIX* GetProjectionMatrix() { return &m_projectionMatrix; }

	void SetPosition(D3DXVECTOR3 position) { m_position = position; }
	void SetRotation(D3DXVECTOR3 rotation) { m_rotation = rotation; }
	void SetUp(D3DXVECTOR3 up) { m_up = up; }

	template <class Archive>
	void save(Archive& archive) const
	{
		Vector3 postion = m_position;
		Vector3 rotation = m_rotation;

		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(postion),
			CEREAL_NVP(rotation)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		Vector3 postion, rotation;

		archive(
			cereal::base_class<Component>(this),
			postion,
			rotation
		);
		m_position = D3DXVECTOR3(postion.x, postion.y, postion.z);
		m_rotation = D3DXVECTOR3(rotation.x, rotation.y, rotation.z);
	}

};

CEREAL_REGISTER_TYPE(EditorCamera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, EditorCamera);
