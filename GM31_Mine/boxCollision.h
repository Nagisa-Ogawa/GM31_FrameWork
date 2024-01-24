#pragma once
#include "SerializableClass.h"
#include "component.h"

class BoxCollisionFrame;

class BoxCollision :public Component
{
private:
	D3DXVECTOR3 m_size{};
	D3DXVECTOR3 m_offset{};
	bool		m_isTrigger{};

	D3DXMATRIX m_worldMatrix{};

public:
	void Init(D3DXVECTOR3 size,D3DXVECTOR3 offset, bool isTrigger);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	bool* GetIsTrigger() { return &m_isTrigger; }
	D3DXVECTOR3 GetSize() { return m_size; }
	D3DXVECTOR3 GetOffset() { return m_offset; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetOffset(D3DXVECTOR3 offset) { m_offset = offset; }
	void SetIsTrigger(bool flag) { m_isTrigger = flag; }
	D3DXMATRIX* GetWorldMatrix();

	template <class Archive>
	void save(Archive& archive) const
	{
		Vector3 size = m_size;
		Vector3 offset = m_offset;
		archive(
			CEREAL_NVP(size),
			CEREAL_NVP(offset),
			CEREAL_NVP(m_isTrigger)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		Vector3 size, offset;
		archive(
			size,
			offset,
			m_isTrigger
		);
		m_size = D3DXVECTOR3(size.x, size.y, size.z);
		m_offset = D3DXVECTOR3(offset.x, offset.y, offset.z);
	}

};

CEREAL_REGISTER_TYPE(BoxCollision);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, BoxCollision);
