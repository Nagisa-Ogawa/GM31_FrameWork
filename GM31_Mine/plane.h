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
	void Init(const char* fileName);
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

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

