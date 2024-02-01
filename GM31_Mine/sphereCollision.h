#pragma once
#include "SerializableClass.h"
#include "component.h"

class SphereCollisionFrame;

class SphereCollision : public Component
{
private:
	float		m_radius{};
	D3DXVECTOR3 m_offset{};
	bool		m_isTrigger{};

	Transform* m_transform{};

public:
	void Init(float radius, D3DXVECTOR3 offset, bool isTrigger);
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	bool* GetIsTrigger() { return &m_isTrigger; }
	float* GetRadius() { return &m_radius; }
	D3DXVECTOR3* GetOffset() { return &m_offset; }
	D3DXMATRIX GetWorldMatrix();

	void SetIsTrigger(bool flag) { m_isTrigger = flag; }
	void SetRadius(float r) { m_radius = r; }
	void SetOffset(D3DXVECTOR3 offset) { m_offset = offset; }

	template <class Archive>
	void save(Archive& archive) const
	{
		Vector3 offset = m_offset;
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_radius),
			CEREAL_NVP(offset),
			CEREAL_NVP(m_isTrigger)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		Vector3 offset;
		archive(
			cereal::base_class<Component>(this),
			m_radius,
			offset,
			m_isTrigger
		);
		m_offset = D3DXVECTOR3(offset.x, offset.y, offset.z);
	}

};

CEREAL_REGISTER_TYPE(SphereCollision);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, SphereCollision);

