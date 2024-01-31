#pragma once
#include "SerializableClass.h"
#include "component.h"


class QuadCollision :public Component
{
private:
	D3DXVECTOR3 m_normal{};
	D3DXVECTOR2 m_size{};
	bool	m_isTrigger = false;
public:
	void Init(D3DXVECTOR3 normal, D3DXVECTOR2 size, bool isTrigger);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	bool* GetIsTrigger() { return &m_isTrigger; }
	D3DXVECTOR3* GetNormal() { return &m_normal; }
	D3DXVECTOR2* GetSize() { return &m_size; }

	void SetIsTrigger(bool flag) { m_isTrigger = flag; }
	void SetNormal(D3DXVECTOR3 n) { m_normal = n; }
	void SetSize(D3DXVECTOR2 s) { m_size = s; }

	template <class Archive>
	void save(Archive& archive) const
	{
		Vector2 size = m_size;
		Vector3 normal = m_normal;

		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(size),
			CEREAL_NVP(normal),
			CEREAL_NVP(m_isTrigger)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		Vector2 size;
		Vector3 normal;
		archive(
			cereal::base_class<Component>(this),
			size,
			normal,
			m_isTrigger
		);
		m_size = D3DXVECTOR2(size.x, size.y);
		m_normal = D3DXVECTOR3(normal.x, normal.y, normal.z);
	}

};

CEREAL_REGISTER_TYPE(QuadCollision);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, QuadCollision);

