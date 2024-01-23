#pragma once
#include "component.h"

class BoxCollisionFrame;

class BoxCollision :public Component
{
private:
	D3DXVECTOR3 m_Size{};
	D3DXVECTOR3 m_Offset{};
	bool		m_IsTrigger{};

	D3DXMATRIX m_worldMatrix{};

public:
	void Init(D3DXVECTOR3 size,D3DXVECTOR3 offset, bool isTrigger);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	bool* GetIsTrigger() { return &m_IsTrigger; }
	D3DXVECTOR3 GetSize() { return m_Size; }
	D3DXVECTOR3 GetOffset() { return m_Offset; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetOffset(D3DXVECTOR3 offset) { m_Offset = offset; }
	void SetIsTrigger(bool flag) { m_IsTrigger = flag; }
	D3DXMATRIX* GetWorldMatrix();

	template <class Archive>
	void save(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_fileName);
		)
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_fileName);
		)
	}

};
