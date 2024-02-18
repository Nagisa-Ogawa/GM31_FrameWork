#pragma once
#include <string>
#include "SerializableClass.h"
#include "component.h"

//-----------------------------------------------------
// スプライトコンポーネントクラス
//------------------------------------------------------
// 機能は実装完了していますが、
// まだ未実装です。
//------------------------------------------------------
class Sprite :public Component
{
private:
	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_size;
	std::string m_textureName;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

public:
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 size,const char* textureName);
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	D3DXVECTOR2* GetPosition() { return &m_position; }
	D3DXVECTOR2* GetSize() { return &m_size; }
	std::string GetTextureName() { return m_textureName; }

	void SetPosition(D3DXVECTOR2 pos) { m_position = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }
	void SetTextureName(std::string texName) { m_textureName = texName; }

	template <class Archive>
	void save(Archive& archive) const
	{
		Vector2 position = m_position;
		Vector2 size = m_size;
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(position),
			CEREAL_NVP(size),
			CEREAL_NVP(m_textureName)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		Vector2 position,size;
		archive(
			cereal::base_class<Component>(this),
			position,
			size,
			m_textureName
		);
		m_position = D3DXVECTOR2(position.x, position.y);
		m_size = D3DXVECTOR2(size.x, size.y);
	}

};

CEREAL_REGISTER_TYPE(Sprite);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Sprite);

