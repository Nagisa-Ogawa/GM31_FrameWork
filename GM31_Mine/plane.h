#pragma once

#include <string>
#include "component.h"


class Plane :public Component
{
private:
	std::string m_fileName{};
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

public:
	void Init(D3DXVECTOR3 pos,D3DXVECTOR2 size,const char* fileName);
	void Uninit();
	void Update();
	void Draw();

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_fileName)
			);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this),
			m_fileName
			);
	}

};

CEREAL_REGISTER_TYPE(Plane);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Plane);

