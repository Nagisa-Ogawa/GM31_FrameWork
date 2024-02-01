#pragma once


#include "gameObject.h"

class MeshField : public GameObject
{

private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11Buffer*				m_IndexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};
	ID3D11VertexShader*			m_VertexShader{};
	ID3D11PixelShader*			m_PixelShader{};
	ID3D11InputLayout*			m_VertexLayout{};

	VERTEX_3D					m_Vertex[21][21]{};

public:
	void Init() override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	float GetHeight(D3DXVECTOR3 pos);

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}


};

CEREAL_REGISTER_TYPE(MeshField);